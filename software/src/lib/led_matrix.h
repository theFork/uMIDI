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

#define CHARACTER_BITMAP_0      { 1, 1, 1, \
                                  1, 0, 1, \
                                  1, 0, 1, \
                                  1, 0, 1, \
                                  1, 1, 1  }

#define CHARACTER_BITMAP_1      { 1, 1, 0, \
                                  0, 1, 0, \
                                  0, 1, 0, \
                                  0, 1, 0, \
                                  1, 1, 1  }

#define CHARACTER_BITMAP_2      { 1, 1, 1, \
                                  0, 0, 1, \
                                  1, 1, 1, \
                                  1, 0, 0, \
                                  1, 1, 1  }

#define CHARACTER_BITMAP_3      { 1, 1, 1, \
                                  0, 0, 1, \
                                  1, 1, 1, \
                                  0, 0, 1, \
                                  1, 1, 1  }

#define CHARACTER_BITMAP_4      { 1, 0, 1, \
                                  1, 0, 1, \
                                  1, 1, 1, \
                                  0, 0, 1, \
                                  0, 0, 1  }

#define CHARACTER_BITMAP_5      { 1, 1, 1, \
                                  1, 0, 0, \
                                  1, 1, 1, \
                                  0, 0, 1, \
                                  1, 1, 1  }

#define CHARACTER_BITMAP_6      { 1, 0, 0, \
                                  1, 0, 0, \
                                  1, 1, 1, \
                                  1, 0, 1, \
                                  1, 1, 1  }

#define CHARACTER_BITMAP_7      { 1, 1, 1, \
                                  0, 0, 1, \
                                  0, 1, 0, \
                                  0, 1, 0, \
                                  0, 1, 0  }

#define CHARACTER_BITMAP_8      { 1, 1, 1, \
                                  1, 0, 1, \
                                  1, 1, 1, \
                                  1, 0, 1, \
                                  1, 1, 1  }

#define CHARACTER_BITMAP_9      { 1, 1, 1, \
                                  1, 0, 1, \
                                  1, 1, 1, \
                                  0, 0, 1, \
                                  0, 0, 1  }

#define CHARACTER_BITMAP_A      { 1, 1, 1, \
                                  1, 0, 1, \
                                  1, 1, 1, \
                                  1, 0, 1, \
                                  1, 0, 1  }

#define CHARACTER_BITMAP_B      { 1, 1, 1, \
                                  1, 0, 1, \
                                  1, 1, 0, \
                                  1, 0, 1, \
                                  1, 1, 1  }

#define CHARACTER_BITMAP_C      { 0, 1, 1, \
                                  1, 0, 0, \
                                  1, 0, 0, \
                                  1, 0, 0, \
                                  0, 1, 1  }

#define CHARACTER_BITMAP_D      { 1, 1, 0, \
                                  1, 0, 1, \
                                  1, 0, 1, \
                                  1, 0, 1, \
                                  1, 1, 0  }

#define CHARACTER_BITMAP_E      { 1, 1, 1, \
                                  1, 0, 0, \
                                  1, 1, 1, \
                                  1, 0, 0, \
                                  1, 1, 1  }

#define CHARACTER_BITMAP_F      { 1, 1, 1, \
                                  1, 0, 0, \
                                  1, 1, 1, \
                                  1, 0, 0, \
                                  1, 0, 0  }

#define CHARACTER_BITMAP_G      { 0, 1, 1, \
                                  1, 0, 0, \
                                  1, 1, 1, \
                                  1, 0, 1, \
                                  0, 1, 1  }

#define CHARACTER_BITMAP_H      { 1, 0, 1, \
                                  1, 0, 1, \
                                  1, 1, 1, \
                                  1, 0, 1, \
                                  1, 0, 1  }

#define CHARACTER_BITMAP_I      { 1, 1, 1, \
                                  0, 1, 0, \
                                  0, 1, 0, \
                                  0, 1, 0, \
                                  1, 1, 1  }

#define CHARACTER_BITMAP_J      { 0, 0, 1, \
                                  0, 0, 1, \
                                  0, 0, 1, \
                                  0, 0, 1, \
                                  1, 1, 0  }

#define CHARACTER_BITMAP_K      { 1, 0, 1, \
                                  1, 0, 1, \
                                  1, 1, 0, \
                                  1, 0, 1, \
                                  1, 0, 1  }

#define CHARACTER_BITMAP_L      { 1, 0, 0, \
                                  1, 0, 0, \
                                  1, 0, 0, \
                                  1, 0, 0, \
                                  1, 1, 1  }

#define CHARACTER_BITMAP_M      { 1, 0, 1, \
                                  1, 1, 1, \
                                  1, 0, 1, \
                                  1, 0, 1, \
                                  1, 0, 1  }

#define CHARACTER_BITMAP_N      { 1, 0, 1, \
                                  1, 1, 1, \
                                  1, 1, 1, \
                                  1, 0, 1, \
                                  1, 0, 1  }

#define CHARACTER_BITMAP_O      { 0, 1, 0, \
                                  1, 0, 1, \
                                  1, 0, 1, \
                                  1, 0, 1, \
                                  0, 1, 0  }

#define CHARACTER_BITMAP_P      { 1, 1, 1, \
                                  1, 0, 1, \
                                  1, 1, 1, \
                                  1, 0, 0, \
                                  1, 0, 0  }

#define CHARACTER_BITMAP_Q      { 1, 1, 1, \
                                  1, 0, 1, \
                                  1, 0, 1, \
                                  0, 1, 0, \
                                  1, 1, 0  }

#define CHARACTER_BITMAP_R      { 1, 1, 1, \
                                  1, 0, 1, \
                                  1, 1, 0, \
                                  1, 0, 1, \
                                  1, 0, 1  }

#define CHARACTER_BITMAP_S      { 1, 1, 1, \
                                  1, 0, 0, \
                                  0, 1, 0, \
                                  0, 0, 1, \
                                  1, 1, 1  }

#define CHARACTER_BITMAP_T      { 1, 1, 1, \
                                  0, 1, 0, \
                                  0, 1, 0, \
                                  0, 1, 0, \
                                  0, 1, 0  }

#define CHARACTER_BITMAP_U      { 1, 0, 1, \
                                  1, 0, 1, \
                                  1, 0, 1, \
                                  1, 0, 1, \
                                  1, 1, 1  }

#define CHARACTER_BITMAP_V      { 1, 0, 1, \
                                  1, 0, 1, \
                                  1, 0, 1, \
                                  1, 0, 1, \
                                  0, 1, 0  }

#define CHARACTER_BITMAP_W      { 1, 0, 1, \
                                  1, 0, 1, \
                                  1, 0, 1, \
                                  1, 1, 1, \
                                  1, 0, 1  }

#define CHARACTER_BITMAP_X      { 1, 0, 1, \
                                  1, 0, 1, \
                                  0, 1, 0, \
                                  1, 0, 1, \
                                  1, 0, 1  }

#define CHARACTER_BITMAP_Y      { 1, 0, 1, \
                                  1, 0, 1, \
                                  0, 1, 0, \
                                  0, 1, 0, \
                                  0, 1, 0  }

#define CHARACTER_BITMAP_Z      { 1, 1, 1, \
                                  0, 0, 1, \
                                  0, 1, 0, \
                                  1, 0, 0, \
                                  1, 1, 1  }


//---------------- data types ----------------//

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
void led_matrix_set_pixel(struct led_matrix* matrix, uint8_t row, uint8_t column, enum adafruit_display_color color);


//---------------- EOF ----------------//
#endif // _LED_MATRIX_H
