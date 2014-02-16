/*
 * Copyright 2012-2014 Sebastian Neuser
 *
 * This file is part of the muMIDI firmware.
 *
 * The muMIDI firmware is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The muMIDI firmware is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the muMIDI firmware.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Implementation of the GPIO module.
*/

#include "gpio.h"

#include <avr/io.h>


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

gpio_t gpio = {
    { // portA
        { &PORTB, &PINB, PB4 },
        { &PORTB, &PINB, PB3 },
        { &PORTB, &PINB, PB2 },
        { &PORTB, &PINB, PB1 },
        { &PORTB, &PINB, PB0 },
        { &PORTD, &PIND, PD7 },
        { &PORTD, &PIND, PD6 },
        { &PORTD, &PIND, PD5 }
    },
    { // portB
        { &PORTC, &PINC, PC2 },
        { &PORTC, &PINC, PC3 },
        { &PORTC, &PINC, PC1 },
        { &PORTC, &PINC, PC0 },
        { &PORTB, &PINB, PB5 }
    },
    { // portC
        { &PORTB, &PINB, PB7 },
        { &PORTB, &PINB, PB6 },
        { &PORTD, &PIND, PD4 },
        { &PORTD, &PIND, PD3 },
        { &PORTD, &PIND, PD2 },
        { &PORTC, &PINC, PC5 },
        { &PORTC, &PINC, PC4 }
    }
};



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void configureGPIO() {
    // configure all GPIO pins as outputs
    DDRB = _BV(PB7) | _BV(PB6) | _BV(PB5) | _BV(PB4)
         | _BV(PB3) | _BV(PB2) | _BV(PB1) | _BV(PB0);

    DDRC = _BV(PC5) | _BV(PC4) | _BV(PC3) | _BV(PC2) | _BV(PC1) | _BV(PC0);

    DDRD = _BV(PD7) | _BV(PD6) | _BV(PD5) | _BV(PD4) | _BV(PD3) | _BV(PD2);
}
