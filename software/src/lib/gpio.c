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
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void initialize_gpio_module(struct gpio_config* gpio) {
    // Iterate pins in GPIO config
    struct gpio_pin *pin_pointer = (struct gpio_pin *) &gpio->header1.pin2;
    register8_t* pin_ctrl_register;
    uint8_t i;
    for (i=0; i<sizeof(struct gpio_config)/sizeof(struct gpio_pin); i++) {
        // Configure pin
        switch (pin_pointer->type) {
        case GPIO_INPUT:
            pin_pointer->port->DIRCLR = _BV(pin_pointer->bit);
            break;

        case GPIO_INPUT_PULLDOWN:
            pin_pointer->port->DIRCLR = _BV(pin_pointer->bit);
            pin_ctrl_register = &pin_pointer->port->PIN0CTRL + pin_pointer->bit;
            *pin_ctrl_register = PORT_OPC_PULLDOWN_gc;
            break;

        case GPIO_INPUT_PULLUP:
            pin_pointer->port->DIRCLR = _BV(pin_pointer->bit);
            pin_ctrl_register = &pin_pointer->port->PIN0CTRL + pin_pointer->bit;
            *pin_ctrl_register = PORT_OPC_PULLUP_gc;
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
