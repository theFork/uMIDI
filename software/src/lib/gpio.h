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

#ifndef _GPIO_H
#define _GPIO_H


//---------------- includes ----------------//
#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>


//---------------- data types ----------------//

/// \brief      Function / type of a GPIO pin
enum gpio_type
{
    GPIO_INPUT,                     ///< The GPIO pin acts as an input
    GPIO_INPUT_PULLDOWN,            ///< The GPIO pin acts as an input and is pulled down
    GPIO_INPUT_PULLUP,              ///< The GPIO pin acts as an input and is pulled up
    GPIO_OUTPUT,                    ///< The GPIO pin acts as an output
    GPIO_UNUSED                     ///< The GPIO pin is not used
                                    ///  Unused pins are configured as inputs with pull-down
};

/// \brief      Configuration of a single GPIO pin
/// \see        iox128a4u.h
struct gpio_pin
{
    PORT_t*             port;       ///< The corresponding AVR I/O port registers as defined in the
                                    ///  AVR gcc headers
    uint8_t             bit;        ///< The corresponding bit index in the configuration registers
};

/// \brief      Maps a GPIO pin to its designated type / function
struct gpio_mapping
{
    const struct gpio_pin*  pin;        ///< GPIO pin to configure
    enum gpio_type          type;       ///< Type of the pin
};

/// \brief      Configurations of the GPIO pins contained in one 10-pin header
/// \details    The pins 1 and 10 are reserved for \f$V_{cc}\f$ and GND respectively.
struct gpio_header
{
    const struct gpio_pin     pin2;     ///< Pin 2 in the pin header on the PCB
    const struct gpio_pin     pin3;     ///< Pin 3 in the pin header on the PCB
    const struct gpio_pin     pin4;     ///< Pin 4 in the pin header on the PCB
    const struct gpio_pin     pin5;     ///< Pin 5 in the pin header on the PCB
    const struct gpio_pin     pin6;     ///< Pin 6 in the pin header on the PCB
    const struct gpio_pin     pin7;     ///< Pin 7 in the pin header on the PCB
    const struct gpio_pin     pin8;     ///< Pin 8 in the pin header on the PCB
    const struct gpio_pin     pin9;     ///< Pin 9 in the pin header on the PCB
};

/// \brief      This struct represents the four solder jumpers on the bottom side of the PCB.
struct jumpers
{
    const struct gpio_pin     jp2;      ///< JP2 on the bottom side of the PCB
    const struct gpio_pin     jp3;      ///< JP3 on the bottom side of the PCB
    const struct gpio_pin     jp4;      ///< JP4 on the bottom side of the PCB
    const struct gpio_pin     jp5;      ///< JP5 on the bottom side of the PCB
};

/// \brief      Configurations for all available GPIO pins
struct gpio
{
    const struct gpio_header  header1;  ///< Pin header GPIO1 on the PCB
    const struct gpio_header  header2;  ///< Pin header GPIO2 on the PCB
    const struct gpio_header  header3;  ///< Pin header GPIO3 on the PCB
    const struct jumpers      jumpers;  ///< Solder jumpers on the bottom side of the PCB
};


//---------------- public global variables ----------------//
/// \brief      A global structure with all available GPIO pins
/// \details    This data structure provides a nice way to address GPIO pins. The pins are grouped
///             into three headers with eight pins each and a group for the four solder jumpers -
///             exactly representing the hardware interface. You can use these aliases to configure
///             the pins you need and to get / set the state of the pin.
/// \see        configure_gpio_pin
/// \see        init_gpio_module
/// \see        gpio_get
/// \see        gpio_set
extern const struct gpio gpio;


//---------------- functions and procedures ----------------//

/// \brief      Configures a GPIO pin
/// \param      pin
///                 the GPIO pin to configure
/// \param      type
///                 the desired function / type of the pin
void configure_gpio_pin(const struct gpio_pin* pin, enum gpio_type type);

/// \brief      Initializes the GPIO module
/// \details    Configures all available GPIO pins to a standard configuration. If an array of
///             pin type mappings was supplied, those are also applied.
/// \param      mappings
///                 an array of pin type mappings or `NULL`
/// \param      mappings_size
///                 number of elements in the type mappings array or `0`
void init_gpio_module(const struct gpio_mapping mappings[], uint8_t mappings_size);

/// \brief      Polls a GPIO input pin
/// \details    Automatically de-bounces the pin if it is detected as active.
/// \param      pin
///                 the GPIO input pin to poll
/// \param      type
///                 the GPIO input type (pull-up or pull-down)
/// \returns    `true`  if the input pin reads logical 1
bool poll_gpio_input(const struct gpio_pin pin, enum gpio_type type);


//---------------- inline functions and procedures ----------------//

/// \brief      Enables a GPIO output pin
/// \param      pin
///                 the GPIO pin
static inline void gpio_drive_high(const struct gpio_pin pin) {
    pin.port->OUT |= _BV(pin.bit);
}

/// \brief      Disables a GPIO output pin
/// \param      pin
///                 the GPIO pin
static inline void gpio_drive_low(const struct gpio_pin pin) {
    pin.port->OUT &=~ _BV(pin.bit);
}

/// \brief      Reads the state of a GPIO input pin
/// \param      pin
///                 the GPIO pin
/// \return     `true` if the input pin is driven high; `false` otherwise
static inline bool gpio_get(const struct gpio_pin pin) {
    return pin.port->IN & _BV(pin.bit);
}

/// \brief      Sets a GPIO output pin to the specified state
/// \param      pin
///                 the GPIO pin
/// \param      value
///                 `true` enables the output; `false` disables it
static inline void gpio_set(const struct gpio_pin pin, bool value) {
    value ? gpio_drive_high(pin) : gpio_drive_low(pin);
}

/// \brief      Toggles a GPIO output pin
/// \param      pin
static inline void gpio_toggle(const struct gpio_pin pin) {
    pin.port->OUT ^= _BV(pin.bit);
}


//---------------- EOF ----------------//
#endif // _GPIO_H

