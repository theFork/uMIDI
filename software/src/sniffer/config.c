/// \file
/// \brief      Device configuration

/*
 * Copyright 2015 Sebastian Neuser
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

#include <stddef.h>

#include "lib/adafruit_display.h"
#include "lib/background_tasks.h"
#include "lib/i2c.h"
#include "lib/led_bargraph.h"
#include "lib/led_matrix.h"
#include "lib/leds.h"
#include "lib/midi.h"
#include "lib/serial_communication.h"
#include "lib/usb.h"

#include "config.h"
#include "app.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

//---------------- Background tasks ----------------//
background_task_t high_frequency_tasks[] = {
    &serial_communication_task,
};
uint8_t high_frequency_tasks_size = sizeof(high_frequency_tasks)/sizeof(background_task_t);

background_task_t mid_frequency_tasks[] = {
    &usb_main_task,
};
uint8_t mid_frequency_tasks_size = sizeof(mid_frequency_tasks)/sizeof(background_task_t);

background_task_t low_frequency_tasks[] = {
    &update_leds,
    &i2c_housekeeping,
};
uint8_t low_frequency_tasks_size = sizeof(low_frequency_tasks)/sizeof(background_task_t);

//---------------- Commands ----------------//
static const char cmd_string_chan[] PROGMEM = "chan";
static const char help_string_chan[] PROGMEM = "<n>\n"
    "Sets the MIDI receive channel to sniff on:\n"
    "<n> : channel number [1..16] or 'x' for Omni mode";
static const char cmd_string_send[] PROGMEM = "send";
static const char help_string_send[] PROGMEM = "<d> <d>\n"
    "Sends arbitrary data via I2C interface:\n"
    "<d> : the data bytes to send";
struct serial_command serial_commands[] = {
    { .cmd_string = cmd_string_chan, .help_string = help_string_chan, .handler = &exec_channel },
    { .cmd_string = cmd_string_send, .help_string = help_string_send, .handler = &exec_send    },
};
uint8_t serial_commands_size = sizeof(serial_commands) / sizeof(struct serial_command);

//---------------- I2C ----------------//
struct i2c_config i2c_config = {
    .baudrate = 100000L,
};

//---------------- LED bar graph and matrices ----------------//
struct led_bargraph led_bargraph = {
    .config = {
        .address    = ADAFRUIT_DISPLAY_ADDRESS(0),
        .mode       = ADAFRUIT_DISPLAY_MODE_STATIC,
        .brightness = ADAFRUIT_DISPLAY_BRIGHTNESS_08_OF_16,
    },
};

struct led_matrix led_matrix_a = {
    .config = {
        .address    = ADAFRUIT_DISPLAY_ADDRESS(1),
        .mode       = ADAFRUIT_DISPLAY_MODE_STATIC,
        .brightness = ADAFRUIT_DISPLAY_BRIGHTNESS_08_OF_16,
    },
};

struct led_matrix led_matrix_b = {
    .config = {
        .address    = ADAFRUIT_DISPLAY_ADDRESS(2),
        .mode       = ADAFRUIT_DISPLAY_MODE_STATIC,
        .brightness = ADAFRUIT_DISPLAY_BRIGHTNESS_08_OF_16,
    },
};

struct led_matrix led_matrix_c = {
    .config = {
        .address    = ADAFRUIT_DISPLAY_ADDRESS(3),
        .mode       = ADAFRUIT_DISPLAY_MODE_STATIC,
        .brightness = ADAFRUIT_DISPLAY_BRIGHTNESS_08_OF_16,
    },
};

//---------------- MIDI ----------------//
struct midi_config midi_config = {
    .event_handlers = {
        .control_change = NULL,
        .note_off       = NULL,
        .note_on        = NULL,
        .program_change = NULL
    },
    .rx_channel = MIDI_CHANNEL_01,
    .tx_channel = MIDI_CHANNEL_01,
    .omni_mode = true
};
