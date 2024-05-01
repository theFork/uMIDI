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

static const uint8_t characters[36][LED_MATRIX_CHAR_BITMAP_HEIGHT] = {
    {0b111,
     0b101,
     0b101,
     0b101,
     0b111},

    {0b110,
     0b010,
     0b010,
     0b010,
     0b111},

    {0b111,
     0b001,
     0b111,
     0b100,
     0b111},

    {0b111,
     0b001,
     0b111,
     0b001,
     0b111},

    {0b101,
     0b101,
     0b111,
     0b001,
     0b001},

    {0b111,
     0b100,
     0b111,
     0b001,
     0b111},

    {0b100,
     0b100,
     0b111,
     0b101,
     0b111},

    {0b111,
     0b001,
     0b010,
     0b010,
     0b010},

    {0b111,
     0b101,
     0b111,
     0b101,
     0b111},

    {0b111,
     0b101,
     0b111,
     0b001,
     0b001},

    {0b111,
     0b101,
     0b111,
     0b101,
     0b101},

    {0b111,
     0b101,
     0b110,
     0b101,
     0b111},

    {0b011,
     0b100,
     0b100,
     0b100,
     0b011},

    {0b110,
     0b101,
     0b101,
     0b101,
     0b110},

    {0b111,
     0b100,
     0b111,
     0b100,
     0b111},

    {0b111,
     0b100,
     0b111,
     0b100,
     0b100},

    {0b011,
     0b100,
     0b111,
     0b101,
     0b011},

    {0b101,
     0b101,
     0b111,
     0b101,
     0b101},

    {0b111,
     0b010,
     0b010,
     0b010,
     0b111},

    {0b001,
     0b001,
     0b001,
     0b001,
     0b110},

    {0b101,
     0b101,
     0b110,
     0b101,
     0b101},

    {0b100,
     0b100,
     0b100,
     0b100,
     0b111},

    {0b101,
     0b111,
     0b101,
     0b101,
     0b101},

    {0b101,
     0b111,
     0b111,
     0b101,
     0b101},

    {0b010,
     0b101,
     0b101,
     0b101,
     0b010},

    {0b111,
     0b101,
     0b111,
     0b100,
     0b100},

    {0b111,
     0b101,
     0b101,
     0b110,
     0b011},

    {0b111,
     0b101,
     0b110,
     0b101,
     0b101},

    {0b111,
     0b100,
     0b010,
     0b001,
     0b111},

    {0b111,
     0b010,
     0b010,
     0b010,
     0b010},

    {0b101,
     0b101,
     0b101,
     0b101,
     0b111},

    {0b101,
     0b101,
     0b101,
     0b101,
     0b010},

    {0b101,
     0b101,
     0b101,
     0b111,
     0b101},

    {0b101,
     0b101,
     0b010,
     0b101,
     0b101},

    {0b101,
     0b101,
     0b010,
     0b010,
     0b010},

    {0b111,
     0b001,
     0b010,
     0b100,
     0b111},
};


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
    led_matrix_clear(matrix);
}

void led_matrix_clear(struct led_matrix * const matrix)
{
    memset(matrix->buffer.green, 0, sizeof(matrix->buffer.green));
    memset(matrix->buffer.red,   0, sizeof(matrix->buffer.red));
}

void led_matrix_clear_area(struct led_matrix * const matrix,
                           const uint8_t x_top_left, const uint8_t y_top_left,
                           const uint8_t x_bot_right, const uint8_t y_bot_right)
{
    led_matrix_draw_rectangle(matrix, x_top_left, y_top_left, x_bot_right, y_bot_right,
                              ADAFRUIT_DISPLAY_COLOR_BLACK);
}

void led_matrix_flush(const struct led_matrix * const matrix)
{
    bool green_changed = memcmp(&matrix->last_buffer.green,
                                &matrix->buffer.green,
                                sizeof(matrix->buffer.green));
    bool red_changed = memcmp(&matrix->last_buffer.red,
                              &matrix->buffer.red,
                              sizeof(matrix->buffer.red));
    if (!green_changed && !red_changed) {
        return;
    }

    i2c_start(matrix->config.address, false);
    i2c_write_byte(0x0); // Start address: 0
    for (uint8_t i=0; i<sizeof(matrix->buffer.green); ++i) {
        i2c_write_byte(matrix->buffer.green[i]);
        i2c_write_byte(matrix->buffer.red[i]);
    }
    i2c_stop();
    memcpy((void*) &matrix->last_buffer.green, &matrix->buffer.green, sizeof(matrix->buffer.green));
    memcpy((void*) &matrix->last_buffer.red, &matrix->buffer.red, sizeof(matrix->buffer.red));
}

void led_matrix_draw_rectangle(struct led_matrix* matrix, uint8_t x_top_left, uint8_t y_top_left,
                                                          uint8_t x_bot_right, uint8_t y_bot_right,
                                                          enum adafruit_display_color color)
{
    uint8_t fill_pattern[LED_MATRIX_RESOLUTION] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
    uint8_t width = x_bot_right - x_top_left + 1;
    uint8_t height = y_bot_right - y_top_left + 1;
    led_matrix_show_bitmap(matrix, fill_pattern, x_top_left, y_top_left, width, height, color);
}

void led_matrix_set_pixel(struct led_matrix * const matrix,
                          const uint8_t row, uint8_t column,
                          const enum adafruit_display_color color)
{
    // Abort if an invalid column index was given
    if (column >= LED_MATRIX_RESOLUTION) {
        return;
    }

    // Compute array index
    column = 7-column;

    // Write color
    switch (color) {
    case ADAFRUIT_DISPLAY_COLOR_BLACK:
        matrix->buffer.green[column] &=~ _BV(row);
        matrix->buffer.red[column]   &=~ _BV(row);
        break;
    case ADAFRUIT_DISPLAY_COLOR_GREEN:
        matrix->buffer.green[column] |=  _BV(row);
        matrix->buffer.red[column]   &=~ _BV(row);
        break;
    case ADAFRUIT_DISPLAY_COLOR_ORANGE:
        matrix->buffer.green[column] |= _BV(row);
        matrix->buffer.red[column]   |= _BV(row);
        break;
    case ADAFRUIT_DISPLAY_COLOR_RED:
        matrix->buffer.green[column] &=~ _BV(row);
        matrix->buffer.red[column]   |=  _BV(row);
        break;
    default:
        break;
    }
}

void led_matrix_show_bitmap(struct led_matrix * const matrix, const uint8_t * const bitmap,
                            const uint8_t x_start, const uint8_t y_start,
                            const uint8_t x_size, const uint8_t y_size,
                            const enum adafruit_display_color color)
{
    for (uint8_t y=0; y<y_size; ++y) {
        for (uint8_t x=0; x<x_size; ++x) {
            if (bitmap[y] & _BV(x_size-1-x)) {
                led_matrix_set_pixel(matrix, y+y_start, x+x_start, color);
            }
            else {
                led_matrix_set_pixel(matrix, y+y_start, x+x_start, ADAFRUIT_DISPLAY_COLOR_BLACK);
            }
        }
    }
}

void led_matrix_show_character(struct led_matrix * const matrix, const char character,
                               const uint8_t x_offs, const uint8_t y_offs,
                               const enum adafruit_display_color color)
{
    // Check requested character and compute array index
    uint8_t index = character;
    if (index > 9 && character != ' ') {
        // Abort for invalid characters
        if (character < '0' || ('9' < character && character < 'A') || character > 'Z') {
            return;
        }

        // Subtract offset to digits
        index -= '0'-1;

        if (index > 9) {
            // Skip punctuation between digits and alphabetical characters
            index -= 'A'-'9';
        }
    }

    // Clear previous character and abort if whitespace was requested
    uint8_t x_end = x_offs + LED_MATRIX_CHAR_BITMAP_WIDTH;
    uint8_t y_end = y_offs + LED_MATRIX_CHAR_BITMAP_HEIGHT;
    led_matrix_clear_area(matrix, x_offs, y_offs, x_end, y_end);
    if (character == ' ') {
        return;
    }

    const uint8_t* const bitmap = characters[index];
    led_matrix_show_bitmap(matrix, bitmap, x_offs, y_offs,
                           LED_MATRIX_CHAR_BITMAP_WIDTH, LED_MATRIX_CHAR_BITMAP_HEIGHT, color);
}
