/// \file
/// \brief      Wah-wah module implementation

/*
 * Copyright 2015 Sebastian Neuser
 *
 * This file is part of the uMIDI firmware.
 *
 * The uMIDI firmware is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The uMIDI firmware is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the uMIDI firmware.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include "lib/adc.h"
#include "lib/gpio.h"
#include "lib/leds.h"
#include "lib/math.h"
#include "lib/midi.h"
#include "lib/pwm.h"
#include "lib/serial_communication.h"
#include "lib/usb.h"
#include "lib/wave.h"

#include "config.h"
#include "wah.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

#define TAP_TEMPO_BUFFER_SIZE       8
#define TAP_TEMPO_TASK_FREQUENCY    100

static struct wave pwm_wave;

static struct linear_range pwm_range;

static bool enable_state = false;
static bool tap_arrived = false;



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

static inline uint16_t linear_function(uint8_t midi_value)
{
    return linear(&pwm_range, midi_value);
}

void enable_wah(bool enable)
{
    gpio_set(gpio_config.header3.pin4, enable);
    gpio_set(gpio_config.header3.pin8, enable);
}

bool exec_enable(const char* command)
{
    if (strlen(command) != 8) {
        usb_puts("Malformed command" USB_NEWLINE);
        return false;
    }

    enable_wah(command[7] == 't');
    return true;
}

bool exec_speed(const char* command)
{
    if (strlen(command) < 7 || command[5] != ' ') {
        usb_puts("Malformed command" USB_NEWLINE);
        return false;
    }

    uint8_t speed = atoi(command+6) % MIDI_MAX_VALUE;
    usb_printf("Setting waveform speed to %u" USB_NEWLINE, speed);
    set_speed(&pwm_wave, speed);
    return true;
}

bool exec_tap(const char* command)
{
    tap_arrived = true;
    return true;
}

bool exec_waveform(const char* command)
{
    if (strlen(command) < 7 || command[8] != ' ') {
        usb_puts("Malformed command" USB_NEWLINE);
        return false;
    }

    enum waveform waveform = pwm_wave.settings.waveform;
    if (strncmp("next", command+9, sizeof("next")) == 0) {
        ++waveform;
        waveform %= WAVE_RANDOM;
        usb_printf("Switching to next waveform (%u)" USB_NEWLINE, waveform);
        goto exec;
    }
    if (strncmp("off", command+9, sizeof("off")) == 0) {
        waveform = WAVE_OFF;
        usb_puts("Switching to pedal mode" USB_NEWLINE);
        goto exec;
    }
    if (strncmp("prev", command+9, sizeof("next")) == 0) {
        if (pwm_wave.settings.waveform == 0) {
            waveform = WAVE_RANDOM;
        }
        else {
            --waveform;
        }
        usb_printf("Switching to previous waveform (%u)" USB_NEWLINE, waveform);
        goto exec;
    }
    usb_puts("Unknown parameter" USB_NEWLINE);
    return false;

exec:
    set_waveform(&pwm_wave, waveform);
    return true;
}

void handle_midi_cc(uint8_t controller, uint8_t value)
{
    set_pwm_duty_cycle(value);
}

void handle_switch(void)
{
    if (!gpio_get(gpio_config.header3.pin6)) {
        enable_state = !enable_state;
        enable_wah(enable_state);
        _delay_ms(50);
        while (!gpio_get(gpio_config.header3.pin6)) {
            wdt_reset();
        }
    }
}

void init_wah_module(void)
{
    // Setup linear conversion function
    pwm_range.from = 550;
    pwm_range.to = PWM_MAX_DUTY-50;
    init_linear(&pwm_range);
    init_pwm_module(&linear_function);
    set_pwm_duty_cycle(pwm_range.from);

    // Setup wave module
    const uint8_t speed = 120;
    const uint8_t amplitude = MIDI_MAX_VALUE-32;
    init_wave(&pwm_wave, WAVE_OFF, speed, amplitude, 0);
}

void tap_tempo_task(void)
{
    // Skip every other invocation
    static bool skipping = false;
    skipping = !skipping;
    if (skipping) {
        return;
    }

    static uint8_t taps = 0;
    static uint8_t buffer_index = 0;

    // Increment counter
    static uint16_t counter = 0;
    ++counter;

    if (!tap_arrived) {
        if (counter < 400) {
            return;
        }

        // Reset after timeout
        set_led(LED_RED, false);
        counter = 0;
        taps = 0;
        buffer_index = 0;
        return;
    }
    tap_arrived = false;

    // Increment tap counter to buffer size
    if (taps < TAP_TEMPO_BUFFER_SIZE) {
        ++taps;
    }

    if (taps == 1) {
        set_led(LED_RED, true);
    }
    else {
        // Register tap interval with cyclic buffer
        static fixed_t tap_tempo_buffer[TAP_TEMPO_BUFFER_SIZE] = {0, };
        fixed_t tap_frequency = fixed_div(fixed_from_int(TAP_TEMPO_TASK_FREQUENCY), fixed_from_int(counter));
        tap_tempo_buffer[buffer_index] = tap_frequency;
        ++buffer_index;
        buffer_index %= TAP_TEMPO_BUFFER_SIZE;

        // Compute average
        fixed_t average = 0;
        for (int i=0; i<taps; i++) {
            average += tap_tempo_buffer[i];
        }
        average = fixed_div(average, fixed_from_int(taps));
        usb_printf("freq: 0x%04lx.%04lx Hz | %3d bpm" USB_NEWLINE,
                   average >> 16,
                   average & 0xffff,
                   fixed_to_int(average*60));
    }

    // Reset counter
    counter = 0;
}

void update_wah_pwm(void)
{
    if (pwm_wave.settings.waveform != WAVE_OFF) {
        set_pwm_duty_cycle(update_wave(&pwm_wave));
    }
}
