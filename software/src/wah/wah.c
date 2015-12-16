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

/*
 * Implementation of the expression module.
 * @author Sebastian Neuser
*/

#include "lib/adc.h"
#include "lib/gpio.h"
#include "lib/leds.h"
#include "lib/midi.h"
#include "lib/pwm.h"
#include "lib/wave.h"

#include "config.h"
#include "wah.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

struct wave pwm_wave;



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void initialize_wah_module(void)
{
    initialize_pwm_module();
    const uint8_t speed = 120;
    const uint8_t amplitude = MIDI_MAX_VALUE;
    initialize_wave(&pwm_wave, WAVE_SINE, speed, amplitude, 0);
}

void enable_wah(bool enable)
{
    gpio_set(gpio_config.header3.pin4, enable);
}

void toggle_wah(void)
{
    static uint8_t prescaler = 0;
    ++prescaler;
    if (prescaler >= 80)  {
        prescaler = 0;
        static bool state = false;
        state = !state;
        enable_wah(state);
    }
}

void update_wah_pwm(void)
{
    if (pwm_wave.settings.waveform != WAVE_OFF) {
        apply_duty_cycle(update_wave(&pwm_wave));
    }
}

void handle_midi_cc(uint8_t controller, uint8_t value)
{
    apply_duty_cycle(value);
}
