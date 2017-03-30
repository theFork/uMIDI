/// \file
/// \brief  Driver for the Adafruit 24-bar bi-color LED bar graph

/*
 * Copyright 2016 Sebastian Neuser
 *
 * This file is part of the uMIDI firmware.
 *
 * the uMIDI firmware is free software: you can redistribute it and/or modify
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

#ifndef _LED_BARGRAPH_H
#define _LED_BARGRAPH_H


//---------------- includes ----------------//

#include <stdint.h>

#include "adafruit_display.h"


//---------------- constants ----------------//


//---------------- data types ----------------//

/// \brief      Frame buffer of an LED bar graph
/// \details    The buffer consists of two 8x8 bit fields that determine if the green and/or red
///             LED should be enabled. The array index determines the column, the bit offset
///             represents the row.
struct led_bargraph_buffer
{
    uint8_t green[3];                           ///< Green LED enable bits
    uint8_t red[3];                             ///< Red LED enable bits
};

/// \brief      Represents an LED bar graph
struct led_bargraph
{
    struct adafruit_display_config config;      ///< The bar graph configuration
    struct led_bargraph_buffer     buffer;      ///< The bar graph's frame buffer
};


//---------------- functions and procedures ----------------//

/// \brief      Configures the LED bar graph
/// \details    Writes all common configuration registers and turns the display on.
/// \param      bargraph
///                 the data structure of an LED bar to initialize
void init_led_bargraph_module(struct led_bargraph* bargraph);

/// \brief      Pushes data currently in the LED buffer to the display
/// \details    This function should be placed in a background task.
/// \param      bargraph
///                 the LED bar
void led_bargraph_flush(const struct led_bargraph* bargraph);

/// \brief      Sets the given color of an LED in the bar graph
/// \details    Indexing starts with 0, with (0, 0) adressing the upper left LED.
/// \param      bargraph
///                 the LED bar to manipulate
/// \param      number
///                 bar index, starting from 0
/// \param      color
///                 the color to apply
void led_bargraph_set_pixel(struct led_bargraph* bargraph, uint8_t number, enum adafruit_display_color color);


//---------------- EOF ----------------//
#endif // _LED_BARGRAPH_H
