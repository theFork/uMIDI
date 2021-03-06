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

/*
 * Device configuration.
 * @author Sebastian Neuser
*/

#include <stddef.h>

#include "lib/background_tasks.h"
#include "lib/gpio.h"
#include "lib/hmi.h"
#include "lib/leds.h"
#include "lib/midi.h"
#include "lib/sequencer.h"
#include "lib/serial_communication.h"
#include "lib/usb.h"
#include "lib/wave.h"

#include "config.h"
#include "whammy.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

//---------------- Commands ----------------//
static const char cmd_string_speed[] PROGMEM = "speed";
static const char help_string_speed[] PROGMEM = "<s>\n"
    "Adjust the speed of the sequencer:\n"
    "<s> : wave speed\n";
static const char cmd_string_tap[] PROGMEM = "tap";
static const char help_string_tap[] PROGMEM = "\nSend this command repeatedly to tap in a tempo\n";
static const char cmd_string_pattern[] PROGMEM = "pattern";
static const char help_string_pattern[] PROGMEM = "<p>\n"
    "Select sequencer pattern:\n"
    "<p> : pattern\n"
    "      \"next\" = switch to next pattern\n"
    "      \"prev\" = switch to previous pattern\n";

struct serial_command serial_commands[] = {
    { .cmd_string = cmd_string_speed,   .help_string = help_string_speed,   .handler = &exec_speed   },
    { .cmd_string = cmd_string_tap,     .help_string = help_string_tap,     .handler = &exec_tap     },
    { .cmd_string = cmd_string_pattern, .help_string = help_string_pattern, .handler = &exec_pattern },
};
uint8_t serial_commands_size = sizeof(serial_commands) / sizeof(struct serial_command);

//---------------- GPIO ----------------//
struct gpio_mapping gpio_mappings[] = {
};
uint8_t gpio_mappings_size = sizeof(gpio_mappings)/sizeof(struct gpio_mapping);

//---------------- HMI ----------------//
struct hmi_config hmi_config = {
    .input_header = &gpio.header2,
    .output_header = &gpio.header1,
    .button1_handler = NULL,
    .button2_handler = &toggle_sequencing,
    .encoder1cw_handler = select_next_pattern,
    .encoder1ccw_handler = select_previous_pattern,
    .encoder1push_handler = NULL,
    .encoder2cw_handler = &increase_speed,
    .encoder2ccw_handler = &decrease_speed,
    .encoder2push_handler = &register_tap,
};

//---------------- MIDI ----------------//
struct midi_config midi_config = {
    .event_handlers = {
        .control_change = NULL,
        .note_off       = NULL,
        .note_on        = NULL,
        .program_change = NULL
    },
    .omni_mode  = true,
    .rx_channel = 1,
    .signal_rx  = true,
    .tx_channel = 1,
};

//---------------- State machine ----------------//
background_task_t high_frequency_tasks[] = {
    &serial_communication_task,
    &update_sequencer,
};
uint8_t high_frequency_tasks_size = sizeof(high_frequency_tasks)/sizeof(background_task_t);

background_task_t mid_frequency_tasks[] = {
    &usb_main_task,
    &poll_hmi,
};
uint8_t mid_frequency_tasks_size = sizeof(mid_frequency_tasks)/sizeof(background_task_t);

background_task_t low_frequency_tasks[] = {
    &tap_tempo_task,
    &update_leds,
};
uint8_t low_frequency_tasks_size = sizeof(low_frequency_tasks)/sizeof(background_task_t);
