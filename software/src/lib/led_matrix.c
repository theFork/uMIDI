/// \file
/// \brief  Driver for the Adafruit 8x8 bi-color LED matrix

/*
 * Copyright 2016 Sebastian Neuser
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
#include <string.h>

#include "i2c.h"
#include "led_matrix.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void init_led_matrix_module(struct led_matrix * const matrix)
{
    // Initialize LED matrix
    i2c_start(matrix->config.address, false);
    i2c_write_byte(0x21);
    i2c_stop();

    // Set blink mode and enable display
    i2c_start(matrix->config.address, false);
    i2c_write_byte(0x80 | (matrix->config.mode << 1) | 0x1);
    i2c_stop();

    // Set brightness
    i2c_start(matrix->config.address, false);
    i2c_write_byte(0xE0 | matrix->config.brightness);
    i2c_stop();

    // Initialize frame buffer
    memset(matrix->buffer.green, 0, sizeof(matrix->buffer.green));
    memset(matrix->buffer.red,   0, sizeof(matrix->buffer.red));
}

void led_matrix_flush(const struct led_matrix * const matrix)
{
    i2c_start(matrix->config.address, false);
    i2c_write_byte(0x0); // Start address: 0
    for(uint8_t i=0; i<8; i++) {
        i2c_write_byte(matrix->buffer.green[i]);
        i2c_write_byte(matrix->buffer.red[i]);
    }
    i2c_stop();
}

void led_matrix_set_pixel(struct led_matrix * const matrix,
                          const uint8_t row, uint8_t column, const enum led_matrix_color color)
{
    // Compute array index
    column = 7-column;

    // Write color
    switch (color) {
    case LED_MATRIX_COLOR_BLACK:
        matrix->buffer.green[column] &=~ _BV(row);
        matrix->buffer.red[column]   &=~ _BV(row);
        break;
    case LED_MATRIX_COLOR_GREEN:
        matrix->buffer.green[column] |=  _BV(row);
        matrix->buffer.red[column]   &=~ _BV(row);
        break;
    case LED_MATRIX_COLOR_ORANGE:
        matrix->buffer.green[column] |= _BV(row);
        matrix->buffer.red[column]   |= _BV(row);
        break;
    case LED_MATRIX_COLOR_RED:
        matrix->buffer.green[column] &=~ _BV(row);
        matrix->buffer.red[column]   |=  _BV(row);
        break;
    }
}
