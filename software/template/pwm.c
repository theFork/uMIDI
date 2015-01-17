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

#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void apply_duty_cycle(uint8_t duty)
{
    // apply value
    TCC1.CCA = PWM_LED_SCALER * (MIDI_MAX_VALUE - duty);
}

void initialize_pwm_module(void)
{
    // Prescale clock to 125 kHz
    TCC1.CTRLA = TC_CLKSEL_DIV256_gc;

    // Select single slope PWM mode
    TCC1.CTRLB = TC_WGMODE_SINGLESLOPE_gc;

    // Set TOP value
    TCC1.PER = PWM_LED_SCALER * MIDI_MAX_VALUE;

    // Set initial compare value to TOP
    TCC1.CCA = PWM_LED_SCALER * MIDI_MAX_VALUE;

    // enable interrupts
    TCC1.INTCTRLA = TC_OVFINTLVL_LO_gc;
    TCC1.INTCTRLB = TC_CCAINTLVL_LO_gc;
}



////////////////////////////////////////////////////////////////
//                    I N T E R R U P T S                     //
////////////////////////////////////////////////////////////////

ISR(TCC1_CCA_vect)
{
    gpio_set(PWM_LED, true);
}

ISR(TCC1_OVF_vect)
{
    gpio_set(PWM_LED, false);
}
