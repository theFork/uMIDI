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
    return linear(&pwm_range, midi_value);
}

void init_wah_module(void)
{
    // Setup linear conversion function
    pwm_range.from = 550;
    pwm_range.to = PWM_MAX_DUTY-50;
    init_linear(&pwm_range);
    init_pwm_module(&linear_function);

    // Setup wave module
    const uint8_t speed = 120;
    const uint8_t amplitude = MIDI_MAX_VALUE-32;
    init_wave(&pwm_wave, WAVE_OFF, speed, amplitude, 0);
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

void update_wah_pwm(void)
{
    if (pwm_wave.settings.waveform != WAVE_OFF) {
        set_pwm_duty_cycle(update_wave(&pwm_wave));
    }
}

void handle_midi_cc(uint8_t controller, uint8_t value)
{
    set_pwm_duty_cycle(value);
}
