/*
 * Copyright 2012-2015 Sebastian Neuser
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
 * Implementation of the PWM module.
*/

#include "gpio.h"
#include "midi.h"
#include "pwm.h"
#include "wave.h"

#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

uint16_t values[MEAN_VALUES] = { 0, };

struct wave pwm_wave;



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void apply_duty_cycle(uint8_t duty)
{
    // Scale value
    uint16_t compare_value = PWM_LED_SCALER;
    compare_value *= duty;

    // Compute mean value
    static uint8_t index = 0;
    values[index++] = compare_value;
    index %= MEAN_VALUES;

    uint32_t accumulator = 0;
    uint8_t i;
    for (i=0; i<MEAN_VALUES; i++) {
        accumulator += values[i];
    }
    accumulator /= MEAN_VALUES;

    TCC1.CCA = (uint16_t) accumulator;
}

void initialize_pwm_module(void)
{
    // Prescale clock to 500 kHz
    TCC1.CTRLA = TC_CLKSEL_DIV64_gc;

    // Select single slope PWM mode and enable OC1A output
    TCC1.CTRLB = TC_WGMODE_SINGLESLOPE_gc | TC1_CCAEN_bm;

    // Set TOP value
    TCC1.PER = PWM_LED_SCALER * MIDI_MAX_VALUE;

    // Set initial compare value to TOP
    TCC1.CCA = PWM_LED_SCALER * MIDI_MAX_VALUE;

    initialize_wave(&pwm_wave, MIDI_MAX_VALUE, WAVE_OFF, 63);
}

void update_pwm(void) {
    if (pwm_wave.settings.waveform != WAVE_OFF) {
        apply_duty_cycle(update_wave(&pwm_wave));
    }
}
