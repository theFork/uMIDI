/// \file
/// \brief      Device configuration

/*
 * Copyright 2016 Simon Gansen
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

#include "lib/background_tasks.h"
#include "lib/gpio.h"
#include "lib/leds.h"
#include "lib/midi.h"
#include "lib/serial_communication.h"
#include "lib/usb.h"

#include "config.h"
#include "switcher.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

//---------------- GPIO ----------------//
struct gpio_mapping gpio_mappings[] = {
    // GPIO1: Relais hard-wired to status LEDs
    { .pin = &gpio.header1.pin2, .type = GPIO_OUTPUT       },   // Tune and mute
    { .pin = &gpio.header1.pin3, .type = GPIO_OUTPUT       },   // Loop 1
    { .pin = &gpio.header1.pin4, .type = GPIO_OUTPUT       },   // Loop 2
    { .pin = &gpio.header1.pin5, .type = GPIO_OUTPUT       },   // Loop 3
    { .pin = &gpio.header1.pin6, .type = GPIO_OUTPUT       },   // Loop 4
    { .pin = &gpio.header1.pin7, .type = GPIO_OUTPUT       },   // Loop 5
    { .pin = &gpio.header1.pin8, .type = GPIO_OUTPUT       },   // Switch 1
    { .pin = &gpio.header1.pin9, .type = GPIO_OUTPUT       },   // Switch 2

    // GPIO2: Toggle switches (all low-active)
    { .pin = &gpio.header2.pin2, .type = GPIO_INPUT_PULLUP },   // Toggle Tune and mute
    { .pin = &gpio.header2.pin3, .type = GPIO_INPUT_PULLUP },   // Toggle Loop 1
    { .pin = &gpio.header2.pin4, .type = GPIO_INPUT_PULLUP },   // Toggle Loop 2
    { .pin = &gpio.header2.pin5, .type = GPIO_INPUT_PULLUP },   // Toggle Loop 3
    { .pin = &gpio.header2.pin6, .type = GPIO_INPUT_PULLUP },   // Toggle Loop 4
    { .pin = &gpio.header2.pin7, .type = GPIO_INPUT_PULLUP },   // Toggle Loop 5
    { .pin = &gpio.header2.pin8, .type = GPIO_INPUT_PULLUP },   // Toggle Switch 1
    { .pin = &gpio.header2.pin9, .type = GPIO_INPUT_PULLUP },   // Toggle Switch 2

    // GPIO3: "Save" toggle switch and LED
    { .pin = &gpio.header3.pin3, .type = GPIO_INPUT_PULLUP },   // "Save" toggle switch
};
uint8_t gpio_mappings_size = sizeof(gpio_mappings)/sizeof(struct gpio_mapping);

//---------------- LEDs ----------------//
struct led save_led = {
    .pin = &gpio.header3.pin2,
};

//---------------- MIDI ----------------//
struct midi_config midi_config = {
    .event_handlers = {
        .program_change = handle_program_change,
        .control_change = NULL,
        .note_on = handle_note_on,
        .note_off = handle_note_off,
    },
    .omni_mode  = false,
    .rx_channel = MIDI_CHANNEL_01,
    .signal_rx  = false,
    .tx_channel = MIDI_CHANNEL_01,
};

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
    &poll_switches,
};
uint8_t low_frequency_tasks_size = sizeof(low_frequency_tasks)/sizeof(background_task_t);

//---------------- Custom commands ----------------//
static const char cmd_string_backup[] PROGMEM = "backup";
static const char help_string_backup[] PROGMEM = "Backs up all programs, one bank per line";
static const char cmd_string_load[] PROGMEM = "load";
static const char help_string_load[] PROGMEM = "<n>\n"
    "Loads the specified program:\n"
    "<n> : program number";
static const char cmd_string_relay[] PROGMEM = "rel";
static const char help_string_relay[] PROGMEM = "<r> <s>\n"
    "Manipulates the switchers relays:\n"
    "<r> : Relay to manipulate\n"
    "      'tm'  = Tune/Mute\n"
    "      'l1' = Loop 1\n"
    "      'l2' = Loop 2\n"
    "      'l3' = Loop 3\n"
    "      'l4' = Loop 4\n"
    "      'l5' = Loop 5\n"
    "      's1' = Switch 1\n"
    "      's2' = Switch 2\n"
    "<s> : Relay status\n"
    "      'a' = activate relay\n"
    "      'd' = deactivate relay";
static const char cmd_string_restore[] PROGMEM = "restore";
static const char help_string_restore[] PROGMEM = "<n> <d>\n"
    "Restores a bank given as a hex-string to EEPROM:\n"
    "<n> : Number of the bank to restore,\n"
    "<d> : Program bank data";
static const char cmd_string_save[] PROGMEM = "save";
static const char help_string_save[] PROGMEM = "Saves the current program configuration";

static const char cmd_string_toggle[] PROGMEM = "toggle";
static const char help_string_toggle[] PROGMEM = "<i> <b>\n"
    "Configures a toggle output for the current program:\n"
    "<i> : Toggle output id [0-3]\n"
    "      '0' = Tune/Mute\n"
    "      '1' = Switch 1\n"
    "      '2' = Switch 2\n"
    "      '3' = Reserved...\n"
    "<b> : Output behaviour\n"
    "      'a' = activate output\n"
    "      'd' = deactivate output";

struct serial_command serial_commands[] = {
    { .cmd_string = cmd_string_backup , .help_string = help_string_backup,  .handler = &exec_backup  },
    { .cmd_string = cmd_string_load,    .help_string = help_string_load,    .handler = &exec_load    },
    { .cmd_string = cmd_string_relay,   .help_string = help_string_relay,   .handler = &exec_relay   },
    { .cmd_string = cmd_string_restore, .help_string = help_string_restore, .handler = &exec_restore },
    { .cmd_string = cmd_string_save,    .help_string = help_string_save,    .handler = &exec_save    },
    { .cmd_string = cmd_string_toggle,  .help_string = help_string_toggle,  .handler = &exec_toggle  },
};
uint8_t serial_commands_size = sizeof(serial_commands) / sizeof(struct serial_command);
