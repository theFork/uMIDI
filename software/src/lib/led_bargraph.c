/// \file
/// \brief  Driver for the Adafruit 24-bar bi-color LED bar graph

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
#include "led_bargraph.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void init_led_bargraph_module(struct led_bargraph * const bargraph)
{
    // Initialize LED bargraph
    i2c_start(bargraph->config.address, false);
    i2c_write_byte(0x21);
    i2c_stop();

    // Set blink mode and enable display
    i2c_start(bargraph->config.address, false);
    i2c_write_byte(0x80 | (bargraph->config.mode << 1) | 0x1);
    i2c_stop();

    // Set brightness
    i2c_start(bargraph->config.address, false);
    i2c_write_byte(0xE0 | bargraph->config.brightness);
    i2c_stop();

    // Initialize frame buffer
    memset(bargraph->buffer.green, 0, sizeof(bargraph->buffer.green));
    memset(bargraph->buffer.red,   0, sizeof(bargraph->buffer.red));
}

void led_bargraph_flush(const struct led_bargraph * const bargraph)
{
    i2c_start(bargraph->config.address, false);
    i2c_write_byte(0x0); // Start address: 0
    for (uint8_t i=0; i<sizeof(bargraph->buffer.green); ++i) {
        i2c_write_byte(bargraph->buffer.green[i]);
        i2c_write_byte(bargraph->buffer.red[i]);
    }
    i2c_stop();
}

void led_bargraph_set_pixel(struct led_bargraph * const bargraph,
                            uint8_t bar,
                            const enum adafruit_display_color color)
{
    // Compute coordinates
    bar = 23-bar;

    uint8_t column = 0;
    if (bar < 12) {
        column = bar / 4;
    }
    else {
        column = (bar - 12) / 4;
    }

    uint8_t row = bar % 4;
    if (bar >= 12) {
        row += 4;
    }

    // Write color
    switch (color) {
    case ADAFRUIT_DISPLAY_COLOR_BLACK:
        bargraph->buffer.green[column] &=~ _BV(row);
        bargraph->buffer.red[column]   &=~ _BV(row);
        break;
    case ADAFRUIT_DISPLAY_COLOR_GREEN:
        bargraph->buffer.green[column] &=~ _BV(row);
        bargraph->buffer.red[column]   |=  _BV(row);
        break;
    case ADAFRUIT_DISPLAY_COLOR_ORANGE:
        bargraph->buffer.green[column] |= _BV(row);
        bargraph->buffer.red[column]   |= _BV(row);
        break;
    case ADAFRUIT_DISPLAY_COLOR_RED:
        bargraph->buffer.green[column] |=  _BV(row);
        bargraph->buffer.red[column]   &=~ _BV(row);
        break;
    }
}
