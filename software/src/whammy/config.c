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
#include "lib/i2c.h"
#include "lib/led_matrix.h"
#include "lib/leds.h"
#include "lib/midi.h"
#include "lib/sequencer.h"
#include "lib/serial_communication.h"
#include "lib/usb.h"
#include "lib/wave.h"

#include "config.h"
#include "cli.h"
#include "user_interface.h"
#include "whammy_controller.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

//---------------- Commands ----------------//
static const char cmd_string_ampl[] PROGMEM = "ampl";
static const char help_string_ampl[] PROGMEM = "a\n"
    "Adjust the amplitude of the wave / pitch bend:\n"
    "<a> : amplitude [0, 127]\n";
static const char cmd_string_backup[] PROGMEM = "backup";
static const char help_string_backup[] PROGMEM = "Dumps all data stored in EEPROM";
static const char cmd_string_cpy[] PROGMEM = "cpy";
static const char help_string_cpy[] PROGMEM = "t i\n"
    "Copies the selected pattern or program to the specified slot:\n"
    "<t> : 'P' for pattern or 'p' for program\n"
    "<i> : target pattern index [0, 19] or program number [0, 119]\n";
static const char cmd_string_dump[] PROGMEM = "dump";
static const char help_string_dump[] PROGMEM = "t\n"
    "Shows information about the selected pattern or program\n"
    "<t> : 'P' for pattern or 'p' for program\n";
static const char cmd_string_reset[] PROGMEM = "factoryreset";
static const char help_string_reset[] PROGMEM = "Restores all patterns to factory settings";
static const char cmd_string_mode[] PROGMEM = "mode";
static const char help_string_mode[] PROGMEM = "p\n"
    "Select control mode:\n"
    "<p> : mode\n"
    "      'N' = switch to next mode\n"
    "      'P' = switch to previous mode\n"
    "      'd'     = static detune mode\n"
    "      'l'     = limit (CC down-scaling) mode\n"
    "      'm'     = momentary pitch bend mode\n"
    "      'n'     = normal (CC forward) mode\n"
    "      'o'     = enable bypass (turn effect off)\n"
    "      'p' <n> = specified pattern n [1, 20]\n"
    "      'w' <n> = specified wave n [1, 7]\n";
static const char cmd_string_patlen[] PROGMEM = "patlen";
static const char help_string_patlen[] PROGMEM = "l\n"
    "Set the selected pattern's length:\n"
    "<l> : pattern length in steps\n";
static const char cmd_string_patmod[] PROGMEM = "patmod";
static const char help_string_patmod[] PROGMEM = "s c t d e\n"
    "Modify a step of the selected pattern:\n"
    "<s> : step index [1, length] (2 digits, zero-padded)\n"
    "<c> : MIDI channel [1, 16] (2 digits, zero-padded)\n"
    "<t> : MIDI message type\n"
    "      'c' control change\n"
    "      'f' note off\n"
    "      'n' note on\n"
    "      'p' program change\n"
    "<d> : MIDI data byte 0 (3 digits, zero-padded)\n"
    "<e> : MIDI data byte 1 (3 digits, zero-padded)\n";
static const char cmd_string_pgm[] PROGMEM = "pgm";
static const char help_string_pgm[] PROGMEM = "p\n"
    "Select or update program:\n"
    "<p> : program number [1, 120] or 'u' to overwrite the current program\n";
static const char cmd_string_speed[] PROGMEM = "speed";
static const char help_string_speed[] PROGMEM = "s\n"
    "Adjust the speed of the effect:\n"
    "<s> : bend/pattern/wave speed [0, 127]\n";
static const char cmd_string_store[] PROGMEM = "store";
static const char help_string_store[] PROGMEM = "t n d\n"
    "Stores given data in EEPROM:\n"
    "<t> : 'P' for pattern or 'p' for program\n"
    "<n> : pattern  or program number (counting from 1)\n"
    "<d> : data in hexadecimal format\n";
static const char cmd_string_tap[] PROGMEM = "tap";
static const char help_string_tap[] PROGMEM = "\nSend this command repeatedly to tap in a tempo\n";
static const char cmd_string_wipe[] PROGMEM = "wipe";
static const char help_string_wipe[] PROGMEM = "t\n"
    "Wipes the selected pattern or program\n"
    "<t> : 'P' for pattern or 'p' for program\n";
static const char cmd_string_wham[] PROGMEM = "wham";
static const char help_string_wham[] PROGMEM = "m\n"
    "Select whammy pedal mode:\n"
    "<m> : mode {1..21, 43..63}\n";

struct serial_command serial_commands[] = {
    { .cmd_string = cmd_string_ampl,    .help_string = help_string_ampl,    .handler = &exec_ampl          },
    { .cmd_string = cmd_string_backup,  .help_string = help_string_backup,  .handler = &exec_backup        },
    { .cmd_string = cmd_string_cpy,     .help_string = help_string_cpy,     .handler = &exec_cpy           },
    { .cmd_string = cmd_string_dump,    .help_string = help_string_dump,    .handler = &exec_dump          },
    { .cmd_string = cmd_string_reset,   .help_string = help_string_reset,   .handler = &exec_factory_reset },
    { .cmd_string = cmd_string_mode,    .help_string = help_string_mode,    .handler = &exec_mode          },
    { .cmd_string = cmd_string_patlen,  .help_string = help_string_patlen,  .handler = &exec_patlen        },
    { .cmd_string = cmd_string_patmod,  .help_string = help_string_patmod,  .handler = &exec_patmod        },
    { .cmd_string = cmd_string_pgm,     .help_string = help_string_pgm,     .handler = &exec_pgm           },
    { .cmd_string = cmd_string_speed,   .help_string = help_string_speed,   .handler = &exec_speed         },
    { .cmd_string = cmd_string_store,   .help_string = help_string_store,   .handler = &exec_store         },
    { .cmd_string = cmd_string_tap,     .help_string = help_string_tap,     .handler = &exec_tap           },
    { .cmd_string = cmd_string_wipe,    .help_string = help_string_wipe,    .handler = &exec_wipe          },
    { .cmd_string = cmd_string_wham,    .help_string = help_string_wham,    .handler = &exec_wham          },
};
uint8_t serial_commands_size = sizeof(serial_commands) / sizeof(struct serial_command);

//---------------- GPIO ----------------//
struct gpio_mapping gpio_mappings[] = {
};
uint8_t gpio_mappings_size = sizeof(gpio_mappings)/sizeof(struct gpio_mapping);

//---------------- HMI ----------------//
struct hmi_config hmi_config = {
    .input_header = &gpio.header1,
    .output_header = NULL,
    .long_input_threashold = 15,
    .button1_short_handler = NULL,
    .button1_long_handler = NULL,
    .button1_interrupt_handler = &tap_tempo,
    .button2_short_handler = &toggle_hmi_layer,
    .button2_long_handler = &store_setup,
    .button2_interrupt_handler = NULL,
    .encoder1cw_handler = &value2_increment,
    .encoder1ccw_handler = &value2_decrement,
    .encoder1push_handler = NULL,
    .encoder2cw_handler = &value1_increment,
    .encoder2ccw_handler = &value1_decrement,
    .encoder2push_handler = NULL,
};


//---------------- I2C ----------------//
struct i2c_config i2c_config = {
    .baudrate = 400000L,
};


//---------------- LED matrix ----------------//
struct led_matrix led_matrix_l = {
    .config = {
        .address    = ADAFRUIT_DISPLAY_ADDRESS(1),
        .mode       = ADAFRUIT_DISPLAY_MODE_STATIC,
        .brightness = ADAFRUIT_DISPLAY_BRIGHTNESS_08_OF_16,
    },
};

struct led_matrix led_matrix_r = {
    .config = {
        .address    = ADAFRUIT_DISPLAY_ADDRESS(2),
        .mode       = ADAFRUIT_DISPLAY_MODE_STATIC,
        .brightness = ADAFRUIT_DISPLAY_BRIGHTNESS_08_OF_16,
    },
};


//---------------- MIDI ----------------//
struct midi_config midi_config = {
    .event_handlers = {
        .control_change = handle_midi_control_change,
        .note_off       = handle_midi_note_off,
        .note_on        = handle_midi_note_on,
        .program_change = handle_midi_program_change
    },
    .omni_mode  = true,
    .rx_channel = WHAMMY_CTRL_MIDI_CHANNEL,
    .signal_rx  = true,
    .tx_channel = WHAMMY_CTRL_MIDI_CHANNEL,
};


//---------------- State machine ----------------//
background_task_t high_frequency_tasks[] = {
    &serial_communication_task,
    &poll_hmi,
    &update_controller_value,
};
uint8_t high_frequency_tasks_size = sizeof(high_frequency_tasks)/sizeof(background_task_t);

background_task_t mid_frequency_tasks[] = {
    &usb_main_task,
};
uint8_t mid_frequency_tasks_size = sizeof(mid_frequency_tasks)/sizeof(background_task_t);

background_task_t low_frequency_tasks[] = {
    &tap_tempo_task,
    &update_displays,
    &update_leds,
};
uint8_t low_frequency_tasks_size = sizeof(low_frequency_tasks)/sizeof(background_task_t);
