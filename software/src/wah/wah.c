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

static struct wave pwm_wave;

static struct linear_range pwm_range;

static bool enable_state = false;



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

static inline uint16_t linear_function(uint8_t midi_value)
{
    return linear_from_midi(&pwm_range, midi_value);
}

static inline void toggle_wah(void)
{
    enable_wah(!enable_state);
}

void enable_wah(bool enable)
{
    enable_state = enable;
    gpio_set(gpio.header3.pin4, enable);
    gpio_set(gpio.header3.pin8, enable);
}

bool exec_enable(const char* command)
{
    if (strlen(command) != 8) {
        usb_puts(PSTR("Malformed command" USB_NEWLINE));
        return false;
    }

    enable_wah(command[7] == 't');
    return true;
}

bool exec_speed(const char* command)
{
    if (strlen(command) < 7 || command[5] != ' ') {
        usb_puts(PSTR("Malformed command" USB_NEWLINE));
        return false;
    }

    midi_value_t speed = atoi(command+6);
    speed %= MIDI_MAX_VALUE + 1;
    usb_printf(PSTR("Setting waveform speed to %u" USB_NEWLINE), speed);
    set_speed(&pwm_wave, speed);
    return true;
}

bool exec_tap(const char* command)
{
    register_tap();
    return true;
}

bool exec_waveform(const char* command)
{
    if (strlen(command) < 7 || command[8] != ' ') {
        usb_puts(PSTR("Malformed command" USB_NEWLINE));
        return false;
    }

    enum waveform waveform = pwm_wave.settings.waveform;
    if (strncmp("next", command+9, sizeof("next")) == 0) {
        ++waveform;
        waveform %= WAVE_RANDOM+1;
        usb_printf(PSTR("Switching to next waveform (%u)" USB_NEWLINE), waveform);
        goto exec;
    }
    if (strncmp("off", command+9, sizeof("off")) == 0) {
        waveform = WAVE_OFF;
        usb_puts(PSTR("Switching to pedal mode" USB_NEWLINE));
        goto exec;
    }
    if (strncmp("prev", command+9, sizeof("next")) == 0) {
        if (pwm_wave.settings.waveform == 0) {
            waveform = WAVE_RANDOM;
        }
        else {
            --waveform;
        }
        usb_printf(PSTR("Switching to previous waveform (%u)" USB_NEWLINE), waveform);
        goto exec;
    }
    usb_puts(PSTR("Unknown parameter" USB_NEWLINE));
    return false;

exec:
    set_waveform(&pwm_wave, waveform);
    return true;
}

void handle_midi_cc(midi_value_t controller, midi_value_t value)
{
    set_pwm_duty_cycle(WAH_PWM, value);
}

void handle_midi_note_off(midi_value_t note, midi_value_t velocity) {
    if (note == MIDI_NOTE_ENABLE_WAH) {
        enable_wah(false);
    }
}

void handle_midi_note_on(midi_value_t note, midi_value_t velocity) {
    if (note == MIDI_NOTE_ENABLE_WAH) {
        enable_wah(true);
    }
}

void handle_switch(void)
{
    // If the momentary switch was pressed (pull-down)
    if (!gpio_get(gpio.header3.pin6)) {
        toggle_wah();

        // De-bounce
        _delay_ms(50);
        while (!gpio_get(gpio.header3.pin6)) {
            wdt_reset();
        }
    }
}

void init_wah_module(void)
{
    // Setup linear conversion function
    pwm_range.from = 570;
    pwm_range.to = PWM_MAX_DUTY-190;
    init_linear_from_midi(&pwm_range);
    init_pwm(WAH_PWM, &linear_function);
    set_pwm_duty_cycle(WAH_PWM, pwm_range.from);

    // Setup wave module
    configure_tap_tempo_wave(&pwm_wave);
    init_wave(&pwm_wave, WAVE_OFF, 90, MIDI_MAX_VALUE-23, 71);
}

void update_wah_pwm(void)
{
    if (pwm_wave.settings.waveform != WAVE_OFF) {
        set_pwm_duty_cycle(WAH_PWM, update_wave(&pwm_wave));
    }
}
