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

#include <stdint.h>

#include "adafruit_display.h"


//---------------- constants ----------------//

/// \brief  Number of vertical pixels in pre-defined character bitmaps
#define LED_MATRIX_CHAR_BITMAP_HEIGHT   5

/// \brief  Number of horizontal pixels in pre-defined character bitmaps
#define LED_MATRIX_CHAR_BITMAP_WIDTH    3

/// \brief      X- and Y-resolution of the LED matrix
#define LED_MATRIX_RESOLUTION           8


//---------------- data types ----------------//

/// \brief      Frame buffer of an LED matrix
/// \details    The buffer consists of two 8x8 bit fields that determine if the green and/or red
///             LED should be enabled. The array index determines the column, the bit offset
///             represents the row.
struct led_matrix_buffer
{
    uint8_t green[LED_MATRIX_RESOLUTION];       ///< Green LED enable bits
    uint8_t red[LED_MATRIX_RESOLUTION];         ///< Red LED enable bits
};

/// \brief      Represents an LED matrix
struct led_matrix
{
    struct adafruit_display_config config;      ///< The LED matrix configuration
    struct led_matrix_buffer       buffer;      ///< The LED matrix's frame buffer
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

/// \brief      Clears out an area of the LED matrix
/// \details    Disables green and red LEDs in a rectangle defined by two points.
/// \param      matrix
///                 the LED matrix to manipulate
/// \param      x_top_left
///                 X-coordinate of the upper left corner
/// \param      y_top_left
///                 X-coordinate of the upper left corner
/// \param      x_bot_right
///                 Y-coordinate of the lower right corner
/// \param      y_bot_right
///                 Y-coordinate of the lower right corner
void led_matrix_clear_area(struct led_matrix* matrix, uint8_t x_top_left, uint8_t y_top_left,
                                                      uint8_t x_bot_right, uint8_t y_bot_right);

/// \brief      Displays a bitmap on the LED matrix.
/// \details    Indexing starts at 0, with (0, 0) adressing the upper left LED.
/// \param      matrix
///                 the LED matrix to manipulate
/// \param      bitmap
///                 the bitmap data
/// \param      x_start
///                 column offset (from left)
/// \param      y_start
///                 row offset (from top)
/// \param      x_size
///                 number of columns in the bitmap
/// \param      y_size
///                 number of rows in the bitmap
/// \param      color
///                 the color to apply
void led_matrix_show_bitmap(struct led_matrix* matrix, const uint8_t* bitmap,
                            const uint8_t x_start, const uint8_t y_start,
                            const uint8_t x_size, const uint8_t y_size,
                            enum adafruit_display_color color);


/// \brief      Displays a character at the chosen position
/// \details    Currently, characters '0'-'9' and 'A'-'Z' are available.
///             For convenience, '0'-'9' can also be accessed by their corresponding integers, so
///             \code
///             led_matrix_show_character(matrix, 7, 0, 0, ADAFRUIT_DISPLAY_COLOR_GREEN);
///             \endcode
///             will show a green '7' at the upper left corner of the display.
/// \param      matrix
///                 the LED matrix to manipulate
/// \param      character
///                 the character to show
///                 Valid values: {0-9, '0'-'9', 'A'-'Z'}
/// \param      x_offs
///                 left offset
/// \param      y_offs
///                 top offset
/// \param      color
///                 the color to use
void led_matrix_show_character(struct led_matrix* matrix, char character,
                               uint8_t x_offs, uint8_t y_offs,
                               enum adafruit_display_color color);

/// \brief      Sets the given color of an LED chosen by its coordinates on in the matrix.
/// \details    Indexing starts at 0, with (0, 0) adressing the upper left LED.
/// \param      matrix
///                 the LED matrix to manipulate
/// \param      row
///                 row index
/// \param      column
///                 column index
/// \param      color
///                 the color to apply
void led_matrix_set_pixel(struct led_matrix* matrix, uint8_t row, uint8_t column,
                          enum adafruit_display_color color);


//---------------- EOF ----------------//
#endif // _LED_MATRIX_H
