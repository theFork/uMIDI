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
#include <avr/pgmspace.h>

#include "lib/background_tasks.h"
#include "lib/gpio.h"
#include "lib/leds.h"
#include "lib/midi.h"
#include "lib/serial_communication.h"
#include "lib/usb.h"

#include "config.h"
#include "app.h"
#include "wah.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

//---------------- GPIO ----------------//
struct gpio_mapping gpio_mappings[] = {
    { .pin = &gpio.header3.pin2, .type = GPIO_INPUT_PULLUP },   // Enable output
    { .pin = &gpio.header3.pin8, .type = GPIO_OUTPUT       },   // Wah LED
};
uint8_t gpio_mappings_size = sizeof(gpio_mappings)/sizeof(struct gpio_mapping);

//---------------- MIDI ----------------//
struct midi_config midi_config = {
    .event_handlers = {
        .control_change = handle_midi_cc,
        .note_off       = NULL,
        .note_on        = NULL,
        .program_change = NULL
    },
    .omni_mode  = true,
    .rx_channel = MIDI_CHANNEL_01,
    .signal_rx  = true,
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
};
uint8_t low_frequency_tasks_size = sizeof(low_frequency_tasks)/sizeof(background_task_t);

//---------------- Custom commands ----------------//
static const char cmd_string_led[] PROGMEM = "led";
static const char help_string_led[] PROGMEM = "<l> <a>\n"
    "Manipulates the two on-board LEDs:\n"
    "<l> : LED to manipulate\n"
    "      'g' = green LED\n"
    "      'r' = red LED\n"
    "<a> : LED mode / action\n"
    "      'b' = blink\n"
    "      'f' = flash\n"
    "      't' = toggle";

static const char cmd_string_duty[] PROGMEM = "duty";
static const char help_string_duty[] PROGMEM = "<d>\n"
    "Sets the PWM duty cycle:\n"
    "<d> : Duty cycle as MIDI value {0, 1, 127}";

static const char cmd_string_enable[] PROGMEM = "enable";
static const char help_string_enable[] PROGMEM = "<d>\n"
    "Enables the device:\n"
    "<d> : Boolean {'t', 'f'}";

struct serial_command serial_commands[] = {
    { .cmd_string = cmd_string_led,    .help_string = help_string_led,    .handler = &exec_led  },
    { .cmd_string = cmd_string_duty,   .help_string = help_string_duty,   .handler = &exec_duty },
    { .cmd_string = cmd_string_enable, .help_string = help_string_enable, .handler = &exec_enable },
};
uint8_t serial_commands_size = sizeof(serial_commands) / sizeof(struct serial_command);
