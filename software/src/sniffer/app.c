/// \file
/// \brief      Implementation of the MIDI sniffer module

/*
 * Copyright 2015 Sebastian Neuser
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

#include <stdlib.h>
#include <string.h>

#include "lib/i2c.h"
#include "lib/led_bargraph.h"
#include "lib/led_matrix.h"
#include "lib/midi.h"
#include "lib/usb.h"

#include "app.h"
#include "config.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

bool exec_channel(const char* command)
{
    // Abort if the command is malformed
    if (strlen(command) < 6 || command[4] != ' ') {
        usb_puts(PSTR("Malformed command" USB_NEWLINE));
        return false;
    }

    enum midi_channel channel = atoi(command+5);
    if (channel == 0) {
        usb_puts(PSTR("Listening on all channels"));
        set_omni_mode(true);
    }
    else {
        usb_printf(PSTR("Listening on MIDI channel %u" USB_NEWLINE), channel);
        set_midi_rx_channel(channel-1);
        set_omni_mode(false);
    }

    return true;
}

bool exec_send(const char* command)
{
    // Abort if the command is malformed
    if (strlen(command) < 6 || command[4] != ' ') {
        usb_puts(PSTR("Malformed command" USB_NEWLINE));
        return false;
    }

    uint8_t data0 = strtol(command+5, NULL, 16);
    uint8_t data1 = strtol(command+8, NULL, 16);
    usb_printf(PSTR("Sending I2C data: %02x %02x" USB_NEWLINE), data0, data1);

    i2c_start(0x70, false);
    i2c_write_byte(data0);
    i2c_write_byte(data1);
    i2c_stop();

    return true;
}

void i2c_housekeeping(void)
{
    // Matrix
    uint8_t row    = rand() % 8;
    uint8_t column = rand() % 8;
    uint8_t color  = rand() % 4;
    led_matrix_set_pixel(&led_matrix_a, row, column, color);
    row    = rand() % 8;
    column = rand() % 8;
    color  = rand() % 4;
    led_matrix_set_pixel(&led_matrix_b, row, column, color);
    row    = rand() % 8;
    column = rand() % 8;
    color  = rand() % 4;
    led_matrix_set_pixel(&led_matrix_c, row, column, color);

    // Bar graph
    static uint8_t prescaler = 0;
    ++prescaler;
        if (prescaler < 4) {
        return;
    }
    prescaler = 0;

    static uint8_t bar_color = LED_MATRIX_COLOR_GREEN;
    static uint8_t color_prescaler = -1;
    ++color_prescaler;
    if (color_prescaler >= 24) {
        color_prescaler = 0;
        bar_color += 1;
        bar_color %= 4;
    }

    static uint8_t bar = -1;
    bar += 1;
    bar %= 24;
    led_bargraph_set_pixel(&led_bargraph, bar, bar_color);

    led_bargraph_flush(&led_bargraph);
    led_matrix_flush(&led_matrix_a);
    led_matrix_flush(&led_matrix_b);
    led_matrix_flush(&led_matrix_c);
}
