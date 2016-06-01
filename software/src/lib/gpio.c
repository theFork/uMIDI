/// \file
/// \brief  GPIO configuration and service functions

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

#include <stdbool.h>
#include <stdint.h>

#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include "gpio.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

const struct gpio gpio = {
    .header1 = {
        .pin2 = { &PORTA, 0 },
        .pin3 = { &PORTA, 1 },
        .pin4 = { &PORTA, 2 },
        .pin5 = { &PORTA, 3 },
        .pin6 = { &PORTA, 4 },
        .pin7 = { &PORTA, 5 },
        .pin8 = { &PORTA, 6 },
        .pin9 = { &PORTA, 7 }
    },
    .header2 = {
        .pin2 = { &PORTB, 0 },
        .pin3 = { &PORTB, 1 },
        .pin4 = { &PORTB, 2 },
        .pin5 = { &PORTB, 3 },
        .pin6 = { &PORTC, 0 },
        .pin7 = { &PORTC, 1 },
        .pin8 = { &PORTC, 2 },
        .pin9 = { &PORTC, 3 }
    },
    .header3 = {
        .pin2 = { &PORTC, 4 },
        .pin3 = { &PORTC, 5 },
        .pin4 = { &PORTC, 6 },
        .pin5 = { &PORTC, 7 },
        .pin6 = { &PORTD, 0 },
        .pin7 = { &PORTD, 1 },
        .pin8 = { &PORTD, 2 },
        .pin9 = { &PORTD, 3 }
    },
    .jumpers = {
        .jp2 =  { &PORTR, 0 },
        .jp3 =  { &PORTR, 1 },
        .jp4 =  { &PORTD, 4 },
        .jp5 =  { &PORTD, 5 }
    }
};


////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void init_gpio_module(const struct gpio_mapping * const mappings, const uint8_t mappings_size)
{
    // Iterate pins in GPIO config and initialize default configuration
    const struct gpio_pin* pin_pointer = &gpio.header1.pin2;
    for (uint8_t i=0; i<sizeof(struct gpio)/sizeof(struct gpio_pin); ++i) {
        if (i >= 24) {
            // Configure solder jumpers as inputs
            configure_gpio_pin(pin_pointer, GPIO_INPUT_PULLUP);
        }
        else {
            // Configure headers as unused
            configure_gpio_pin(pin_pointer, GPIO_UNUSED);
        }

        ++pin_pointer;
    }

    if (mappings && mappings_size) {
        // Apply user-defined pin type mappings
        for (uint8_t i=0; i < mappings_size; ++i) {
            const struct gpio_mapping mapping = mappings[i];
            configure_gpio_pin(mapping.pin, mapping.type);
        }
    }
}

void configure_gpio_pin(const struct gpio_pin* const pin, const enum gpio_type type)
{
    // Configure pin based on the provided type
    register8_t * const pin_ctrl_register = &pin->port->PIN0CTRL + pin->bit;
    switch (type) {
    case GPIO_INPUT:
        pin->port->DIRCLR = _BV(pin->bit);
        break;

    case GPIO_INPUT_PULLUP:
        pin->port->DIRCLR = _BV(pin->bit);
        *pin_ctrl_register = PORT_OPC_PULLUP_gc;
        break;

    case GPIO_OUTPUT:
        pin->port->DIRSET = _BV(pin->bit);
        break;

    case GPIO_INPUT_PULLDOWN:
    case GPIO_UNUSED:
    default:
        pin->port->DIRCLR = _BV(pin->bit);
        *pin_ctrl_register = PORT_OPC_PULLDOWN_gc;
        break;
    }
}

bool poll_gpio_input(const struct gpio_pin pin, const enum gpio_type type)
{
    return poll_gpio_input_timeout(pin, type, 0);
}

enum gpio_input_event poll_gpio_input_timeout(const struct gpio_pin pin, const enum gpio_type type, uint8_t timeout)
{
    // Use XOR bit"mask" to invert the read input for pull-up enabled inputs
    bool type_mask;
    switch (type) {
    case GPIO_INPUT_PULLDOWN:
        type_mask = false;
        break;

    case GPIO_INPUT_PULLUP:
        type_mask = true;
        break;

    default:
        return GPIO_INPUT_EVENT_NONE;
    }

    // Abort if the input is not set
    if (!gpio_get(pin) ^ type_mask) {
        return GPIO_INPUT_EVENT_NONE;
    }

    // De-bounce
    _delay_ms(10);
    wdt_reset();

    // Wait for timeout
    static bool returning_from_long_press = false;
    uint8_t timeout_counter = 0;
    while (gpio_get(pin) ^ type_mask) {
        if (timeout == 0) {
            wdt_reset();
            continue;
        }

        if (timeout_counter == timeout) {
            returning_from_long_press = true;
            return GPIO_INPUT_EVENT_LONG;
        }

        _delay_ms(100);
        wdt_reset();
        ++timeout_counter;
    }

    if (returning_from_long_press) {
        returning_from_long_press = false;
        return GPIO_INPUT_EVENT_NONE;
    }

    return GPIO_INPUT_EVENT_SHORT;
}
