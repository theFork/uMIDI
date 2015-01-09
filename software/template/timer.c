/*
 * Copyright 2014,2015 Sebastian Neuser
 *
 * This file is part of the uMIDI firmware.
 *
 * the uMIDI firmware is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * the uMIDI firmware is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the uMIDI firmware.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * timer module implementation of the uMIDI firmware
*/

#include "adc.h"
#include "timer.h"

#include <avr/io.h>
#include <avr/interrupt.h>


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void configureTimer0(void) {
    // select CTC wave generation
    TCCR0A = _BV(WGM01);

    // configure prescaler /8
    TCCR0B = _BV(CS01);

    // set counter compare value to achieve 1ms clock
    OCR0A = 125;

    // enable the overflow interrupt
    TIMSK0 = _BV(OCIE0A);
}



////////////////////////////////////////////////////////////////
//                    I N T E R R U P T S                     //
////////////////////////////////////////////////////////////////

ISR(TIMER0_COMPA_vect) {
    // disable interrupts
    cli();

    static uint16_t prescaler = 0;
    ++prescaler;
    if (prescaler < 50) {
        goto leave;
    }
    prescaler = 0;

    triggerADC();

leave:
    // enable interrupts
    sei();
}
