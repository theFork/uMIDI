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
 * Register bit definitions and prototypes for the GPIO handling module.
*/

#ifndef _GPIO_H
#define _GPIO_H


//---------------- includes ----------------//
#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>


//---------------- data types ----------------//
enum gpio_type
{
    GPIO_INPUT,
    GPIO_OUTPUT,
    GPIO_UNUSED
};

struct gpio_pin
{
    PORT_t*             port;
    uint8_t             bit;
    enum gpio_type      type;
};

struct gpio_header
{
    struct gpio_pin     pin2;
    struct gpio_pin     pin3;
    struct gpio_pin     pin4;
    struct gpio_pin     pin5;
    struct gpio_pin     pin6;
    struct gpio_pin     pin7;
    struct gpio_pin     pin8;
    struct gpio_pin     pin9;
};

struct gpio
{
    struct gpio_header  header1;
    struct gpio_header  header2;
    struct gpio_header  header3;
};


//---------------- variable declarations ----------------//

extern struct gpio gpio;


//---------------- functions and procedures ----------------//
static inline void gpio_drive_high(struct gpio_pin pin) {
    pin.port->OUT |= _BV(pin.bit);
}

static inline void gpio_drive_low(struct gpio_pin pin) {
    pin.port->OUT &=~ _BV(pin.bit);
}

static inline bool gpio_get(struct gpio_pin pin) {
    return pin.port->IN & _BV(pin.bit);
}

static inline void gpio_set(struct gpio_pin pin, bool value) {
    value ? gpio_drive_high(pin) : gpio_drive_low(pin);
}

static inline void gpio_toggle(struct gpio_pin pin) {
    pin.port->OUT ^= _BV(pin.bit);
}

void initialize_gpio_module(void);


//---------------- EOF ----------------//
#endif // _GPIO_H

