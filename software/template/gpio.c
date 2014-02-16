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
        { &PINB, &PORTB, PB4 },
        { &PINB, &PORTB, PB3 },
        { &PINB, &PORTB, PB2 },
        { &PINB, &PORTB, PB1 },
        { &PINB, &PORTB, PB0 },
        { &PIND, &PORTD, PD7 },
        { &PIND, &PORTD, PD6 },
        { &PIND, &PORTD, PD5 }
    },
    { // portB
        { &PINC, &PORTC, PC2 },
        { &PINC, &PORTC, PC3 },
        { &PINC, &PORTC, PC1 },
        { &PINC, &PORTC, PC0 },
        { &PINB, &PORTB, PB5 }
    },
    { // portC
        { &PINB, &PORTB, PB7 },
        { &PINB, &PORTB, PB6 },
        { &PIND, &PORTD, PD4 },
        { &PIND, &PORTD, PD3 },
        { &PIND, &PORTD, PD2 },
        { &PINC, &PORTC, PC5 },
        { &PINC, &PORTC, PC4 }
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
