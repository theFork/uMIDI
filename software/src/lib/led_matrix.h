/// \file
/// \brief  Driver for the Adafruit 8x8 bi-color LED matrix

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

#ifndef _LED_MATRIX_H
#define _LED_MATRIX_H


//---------------- includes ----------------//

#include <stdbool.h>
#include <stdint.h>

#include "gpio.h"


//---------------- constants ----------------//


//---------------- data types ----------------//

/// \brief      Brightness of the LEDs
enum led_matrix_brightness
{
    LED_MATRIX_BRIGHTNESS_01_OF_16, ///< 1/16th of maximum brightness
    LED_MATRIX_BRIGHTNESS_02_OF_16, ///< 2/16th of maximum brightness
    LED_MATRIX_BRIGHTNESS_03_OF_16, ///< 3/16th of maximum brightness
    LED_MATRIX_BRIGHTNESS_04_OF_16, ///< 4/16th of maximum brightness
    LED_MATRIX_BRIGHTNESS_05_OF_16, ///< 5/16th of maximum brightness
    LED_MATRIX_BRIGHTNESS_06_OF_16, ///< 6/16th of maximum brightness
    LED_MATRIX_BRIGHTNESS_07_OF_16, ///< 7/16th of maximum brightness
    LED_MATRIX_BRIGHTNESS_08_OF_16, ///< 8/16th of maximum brightness
    LED_MATRIX_BRIGHTNESS_09_OF_16, ///< 9/16th of maximum brightness
    LED_MATRIX_BRIGHTNESS_10_OF_16, ///< 10/16th of maximum brightness
    LED_MATRIX_BRIGHTNESS_11_OF_16, ///< 11/16th of maximum brightness
    LED_MATRIX_BRIGHTNESS_12_OF_16, ///< 12/16th of maximum brightness
    LED_MATRIX_BRIGHTNESS_13_OF_16, ///< 13/16th of maximum brightness
    LED_MATRIX_BRIGHTNESS_14_OF_16, ///< 14/16th of maximum brightness
    LED_MATRIX_BRIGHTNESS_15_OF_16, ///< 15/16th of maximum brightness
    LED_MATRIX_BRIGHTNESS_16_OF_16  ///< maximum brightness
};

/// \brief      Possible LED colors
enum led_matrix_color
{
    LED_MATRIX_COLOR_BLACK,     ///< Black (LED off)
    LED_MATRIX_COLOR_GREEN,     ///< Green
    LED_MATRIX_COLOR_ORANGE,    ///< Orange
    LED_MATRIX_COLOR_RED,       ///< Red
};

/// \brief      Blinking mode of the LEDs
enum led_matrix_mode
{
    LED_MATRIX_MODE_STATIC,     ///< LEDs are turned on statically
    LED_MATRIX_MODE_BLINK_2HZ,  ///< LEDs blink with a frequency of 2 Hz
    LED_MATRIX_MODE_BLINK_1HZ,  ///< LEDs blink with a frequency of 1 Hz
    LED_MATRIX_MODE_BLINK_05HZ  ///< LEDs blink with a frequency of 0.5 Hz
};

/// \brief  Configuration of the LED matrix's I2C port
struct led_matrix_config
{
    uint8_t                     address;        ///< I2C bus address of the device
    enum led_matrix_brightness  brightness;     ///< Brightness of the LEDs
    enum led_matrix_mode        mode;           ///< Blink mode
};

/// \brief      Frame buffer of an LED matrix
/// \details    The buffer consists of two 8x8 bit fields that determine if the green and/or red
///             LED should be enabled. The array index determines the column, the bit offset
///             represents the row.
struct led_matrix_buffer
{
    uint8_t green[8];                           ///< Green LED enable bits
    uint8_t red[8];                             ///< Red LED enable bits
};

/// \brief      Represents an LED matrix
struct led_matrix
{
    struct led_matrix_config config;            ///< The LED matrix configuration
    struct led_matrix_buffer buffer;            ///< The LED matrix's frame buffer
};


//---------------- functions and procedures ----------------//

/// \brief      Configures the LED matrix
/// \details    Writes all common configuration registers and turns the display on.
/// \param      matrix
///                 the data structure of an LED matrix to initialize
void init_led_matrix_module(struct led_matrix* matrix);

/// \brief      Pushes data currently in the LED buffer to the display
/// \details    This function should be placed in a background task.
/// \param      matrix
///                 the LED matrix
void led_matrix_flush(const struct led_matrix* matrix);

/// \brief      Sets the given color of an LED chosen by its coordinates on in the matrix.
/// \details    Indexing starts with 0, with (0, 0) adressing the upper left LED.
/// \param      matrix
///                 the LED matrix to manipulate
/// \param      row
///                 row index
/// \param      column
///                 column index
/// \param      color
///                 the color to apply
void led_matrix_set_pixel(struct led_matrix* matrix, uint8_t row, uint8_t column, enum led_matrix_color color);


//---------------- EOF ----------------//
#endif // _LED_MATRIX_H
