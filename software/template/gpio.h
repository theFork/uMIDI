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
 * Register bit definitions and prototypes for the GPIO handling module.
*/

#ifndef _GPIO_H
#define _GPIO_H


//---------------- includes ----------------//
#include "bool.h"

#include <stdint.h>
#include <avr/io.h>


//---------------- constants ----------------//


//---------------- data types ----------------//
typedef struct {
    volatile uint8_t*   read_addr;
    volatile uint8_t*   write_addr;
    uint8_t             bit;
} gpio_pin_t;

typedef struct {
    gpio_pin_t          pin1;
    gpio_pin_t          pin2;
    gpio_pin_t          pin3;
    gpio_pin_t          pin4;
    gpio_pin_t          pin5;
    gpio_pin_t          pin6;
    gpio_pin_t          pin7;
    gpio_pin_t          pin8;
} gpio_portA_t;

typedef struct {
    gpio_pin_t          pin2;
    gpio_pin_t          pin3;
    gpio_pin_t          pin4;
    gpio_pin_t          pin5;
    gpio_pin_t          pin8;
} gpio_portB_t;

typedef struct {
    gpio_pin_t          pin2;
    gpio_pin_t          pin3;
    gpio_pin_t          pin4;
    gpio_pin_t          pin5;
    gpio_pin_t          pin6;
    gpio_pin_t          pin7;
    gpio_pin_t          pin8;
} gpio_portC_t;

typedef struct {
    gpio_portA_t        portA;
    gpio_portB_t        portB;
    gpio_portC_t        portC;
} gpio_t;


//---------------- functions and procedures ----------------//
static inline void gpio_drive_high(gpio_pin_t pin) {
    *pin.write_addr |= _BV(pin.bit);
}

static inline void gpio_drive_low(gpio_pin_t pin) {
    *pin.write_addr &=~ _BV(pin.bit);
}

static inline bool gpio_get(gpio_pin_t pin) {
    return *pin.read_addr & _BV(pin.bit);
}

static inline void gpio_set(gpio_pin_t pin, bool value) {
    value ? gpio_drive_high(pin) : gpio_drive_low(pin);
}

static inline void gpio_toggle(gpio_pin_t pin) {
    *pin.write_addr ^= _BV(pin.bit);
}

void configureGPIO(void);


//---------------- EOF ----------------//
#endif // _GPIO_H

