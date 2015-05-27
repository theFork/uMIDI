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

#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

struct gpio gpio = {
    .header1 = {
        .pin2 = { &PORTA, 0, GPIO_UNUSED },
        .pin3 = { &PORTA, 1, GPIO_UNUSED },
        .pin4 = { &PORTA, 2, GPIO_UNUSED },
        .pin5 = { &PORTA, 3, GPIO_UNUSED },
        .pin6 = { &PORTA, 4, GPIO_INPUT },
        .pin7 = { &PORTA, 5, GPIO_UNUSED },
        .pin8 = { &PORTA, 6, GPIO_UNUSED },
        .pin9 = { &PORTA, 7, GPIO_UNUSED }
    },
    .header2 = {
        .pin2 = { &PORTB, 0, GPIO_UNUSED },
        .pin3 = { &PORTB, 1, GPIO_UNUSED },
        .pin4 = { &PORTB, 2, GPIO_UNUSED },
        .pin5 = { &PORTB, 3, GPIO_UNUSED },
        .pin6 = { &PORTC, 0, GPIO_UNUSED },
        .pin7 = { &PORTC, 1, GPIO_UNUSED },
        .pin8 = { &PORTC, 2, GPIO_UNUSED },
        .pin9 = { &PORTC, 3, GPIO_UNUSED }
    },
    .header3 = {
        .pin2 = { &PORTC, 4, GPIO_UNUSED },
        .pin3 = { &PORTC, 5, GPIO_UNUSED },
        .pin4 = { &PORTC, 6, GPIO_UNUSED },
        .pin5 = { &PORTC, 7, GPIO_UNUSED },
        .pin6 = { &PORTD, 0, GPIO_UNUSED },
        .pin7 = { &PORTD, 1, GPIO_UNUSED },
        .pin8 = { &PORTD, 2, GPIO_UNUSED },
        .pin9 = { &PORTD, 3, GPIO_UNUSED }
    }
};



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void initialize_gpio_module() {
    // Iterate pins in GPIO config
    struct gpio_pin *pin_pointer = (struct gpio_pin *) &gpio.header1.pin2;
    uint8_t i;
    for (i=0; i<sizeof(struct gpio)/sizeof(struct gpio_pin); i++) {
        // Configure pin
        switch (pin_pointer->type) {
        case GPIO_INPUT:
            pin_pointer->port->DIRCLR = _BV(pin_pointer->bit);
            break;
        case GPIO_OUTPUT:
            pin_pointer->port->DIRSET = _BV(pin_pointer->bit);
            break;
        case GPIO_UNUSED:
        default:
            pin_pointer->port->DIRSET = _BV(pin_pointer->bit);
            pin_pointer->port->OUT = false;
            break;
        }
        ++pin_pointer;
    }
}
