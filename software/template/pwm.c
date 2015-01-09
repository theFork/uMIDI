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

extern gpio_t gpio;



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void applyDutyCycle(uint8_t duty)
{
    // apply value
    OCR1A = PWM_LED_SCALER * (MIDI_MAX_VALUE - duty);
}

void configurePWM(void)
{
    // configure timer 1: fast PWM mode, 125 kHz
    TCCR1A = _BV(WGM11);
    TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS11);

    // set TOP
    ICR1 = PWM_LED_SCALER * MIDI_MAX_VALUE;
    OCR1A = PWM_LED_SCALER * MIDI_MAX_VALUE;

    // enable interrupts
    TIMSK1 = _BV(OCIE1A) | _BV(TOIE1);
}



////////////////////////////////////////////////////////////////
//                    I N T E R R U P T S                     //
////////////////////////////////////////////////////////////////

ISR(TIMER1_COMPA_vect) {
    gpio_set(PWM_LED, true);
}

ISR(TIMER1_OVF_vect) {
    gpio_set(PWM_LED, false);
}
