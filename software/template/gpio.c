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
 * Implementation of the GPIO module.
*/

#include "gpio.h"

#include <avr/io.h>


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

struct gpio gpio = {
    .header1 = {
        { &PORTA.IN, &PORTA.OUT, 0 },
        { &PORTA.IN, &PORTA.OUT, 1 },
        { &PORTA.IN, &PORTA.OUT, 2 },
        { &PORTA.IN, &PORTA.OUT, 3 },
        { &PORTA.IN, &PORTA.OUT, 4 },
        { &PORTA.IN, &PORTA.OUT, 5 },
        { &PORTA.IN, &PORTA.OUT, 6 },
        { &PORTA.IN, &PORTA.OUT, 7 }
    },
    .header2 = {
        { &PORTB.IN, &PORTB.OUT, 0 },
        { &PORTB.IN, &PORTB.OUT, 1 },
        { &PORTB.IN, &PORTB.OUT, 2 },
        { &PORTB.IN, &PORTB.OUT, 3 },
        { &PORTC.IN, &PORTC.OUT, 0 },
        { &PORTC.IN, &PORTC.OUT, 1 },
        { &PORTC.IN, &PORTC.OUT, 2 },
        { &PORTC.IN, &PORTC.OUT, 3 }
    },
    .header3 = {
        { &PORTC.IN, &PORTC.OUT, 4 },
        { &PORTC.IN, &PORTC.OUT, 5 },
        { &PORTC.IN, &PORTC.OUT, 6 },
        { &PORTC.IN, &PORTC.OUT, 7 },
        { &PORTD.IN, &PORTD.OUT, 0 },
        { &PORTD.IN, &PORTD.OUT, 1 },
        { &PORTD.IN, &PORTD.OUT, 2 },
        { &PORTD.IN, &PORTD.OUT, 3 }
    }
};



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void configureGPIO() {
    // Configure all GPIO pins on AVR port A as outputs
    PORTA.DIRSET = _BV(7) | _BV(6) | _BV(5) | _BV(4) | _BV(3) | _BV(2) | _BV(1) | _BV(0);

    // Configure GPIO pins 0..3 on AVR port B as outputs
    PORTB.DIRSET = _BV(3) | _BV(2) | _BV(1) | _BV(0);

    // Configure all GPIO pins on AVR port C as outputs
    PORTC.DIRSET = _BV(7) | _BV(6) | _BV(5) | _BV(4) | _BV(3) | _BV(2) | _BV(1) | _BV(0);

    // Configure GPIO pins on AVR port D as outputs
    PORTD.DIRSET = _BV(3) | _BV(2) | _BV(1) | _BV(0);
}
