/// \file
/// \brief      Device configuration

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

#include <stddef.h>

#include "lib/adc.h"
#include "lib/background_tasks.h"
#include "lib/gpio.h"
#include "lib/leds.h"
#include "lib/midi.h"
#include "lib/serial_communication.h"
#include "lib/usb.h"

#include "config.h"
#include "expression.h"
#include "midi_handlers.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

//---------------- ADC ----------------//
struct adc_config adc_config = {
    .mode       = ADC_MODE_UNSIGNED,
    .prescaler  = ADC_PRESCALER_DIV512_gc,
};

const struct adc_conversion_config expression_conversion = {
    .channel            = ADC_CHANNEL_0,
    .input              = ADC_INPUT_4,
    .callback_unsigned  = &update_expression_value,
};

//---------------- Background tasks ----------------//
background_task_t high_frequency_tasks[] = {
    &serial_communication_task,
};
uint8_t high_frequency_tasks_size = sizeof(high_frequency_tasks)/sizeof(background_task_t);

background_task_t mid_frequency_tasks[] = {
    &trigger_expression_conversion,
    &usb_main_task,
};
uint8_t mid_frequency_tasks_size = sizeof(mid_frequency_tasks)/sizeof(background_task_t);

background_task_t low_frequency_tasks[] = {
    &handle_enable_switch,
    &handle_mode_select_switch,
    &update_leds,
};
uint8_t low_frequency_tasks_size = sizeof(low_frequency_tasks)/sizeof(background_task_t);

//---------------- Commands ----------------//
static const char cmd_string_cal[] PROGMEM = "cal";
static const char help_string_cal[] PROGMEM = "<m>\n"
    "Calibrate the pedal (in the following order):\n"
    "<m> : \"adc\" calibrate the ADC offset (pedal to min position!)\n"
    "      \"min\" set the minimum registered ADC value\n"
    "      \"max\" set the maximum registered ADC value\n"
    "      \"get\" show currently active values\n"
    "      \"sav\" save active values to EEPROM\n";
static const char cmd_string_ctrl[] PROGMEM = "ctrl";
static const char help_string_ctrl[] PROGMEM = "<a> [m] [n]\n"
    "Read or set the MIDI CC number:\n"
    "<a> : \"get\" show configured control/note numbers\n"
    "      \"set\" set the control/note number <m> to <n>\n"
    "<m> : \"ccc\" CC-only mode CC number\n"
    "      \"ncc\" NOTE-and-CC mode CC number\n"
    "      \"ncn\" NOTE-and-CC mode NOTE number\n"
    "<n> : the control/note number [0..127]\n";
static const char cmd_string_echo[] PROGMEM = "echo";
static const char help_string_echo[] PROGMEM = "<v>\n"
    "Switch expression value console output on / off\n"
    "<v> : \"on\" or \"off\"\n";
static const char cmd_string_mode[] PROGMEM = "mode";
static const char help_string_mode[] PROGMEM = "<a> [m]\n"
    "Read or set mode:\n"
    "<a> : \"get\" show current mode\n"
    "      \"set\" enter mode (requires <m>)\n"
    "      \"sav\" save current mode as default\n"
    "<m> : \"cco\" CC-only mode\n"
    "      \"ncc\" NOTE-and-CC mode\n";
static const char cmd_string_mute[] PROGMEM = "mute";
static const char help_string_mute[] PROGMEM = "<v>\n"
    "Mute CC message transmission when status LED is off.\n"
    "Should only be enabled if there is an enable switch.\n"
    "This setting is immediately stored to EEPROM.\n"
    "<m> : \"on\"  mute if not enabled\n"
    "      \"off\" always transmit\n"
    "      \"get\" show current setting\n";
static const char cmd_string_sw[] PROGMEM = "sw";
static const char help_string_sw[] PROGMEM = "<t>\n"
    "Configure enable switch type\n"
    "<t> : \"lat\" latching\n"
    "      \"mom\" momentary\n"
    "      \"get\" show current setting\n";

struct serial_command serial_commands[] = {
    { .cmd_string = cmd_string_cal,  .help_string = help_string_cal,  .handler = &exec_cal  },
    { .cmd_string = cmd_string_ctrl, .help_string = help_string_ctrl, .handler = &exec_ctrl },
    { .cmd_string = cmd_string_echo, .help_string = help_string_echo, .handler = &exec_echo },
    { .cmd_string = cmd_string_mode, .help_string = help_string_mode, .handler = &exec_mode },
    { .cmd_string = cmd_string_mute, .help_string = help_string_mute, .handler = &exec_mute },
    { .cmd_string = cmd_string_sw,   .help_string = help_string_sw,   .handler = &exec_sw },
};
uint8_t serial_commands_size = sizeof(serial_commands) / sizeof(struct serial_command);

//---------------- GPIO ----------------//
struct gpio_mapping gpio_mappings[] = {
    { .pin = &gpio.header1.pin6, .type = GPIO_INPUT },          // ADC input
    { .pin = &POWER_LED_PIN, .type = GPIO_OUTPUT },             // Power LED
    { .pin = &STATUS_LED_PIN, .type = GPIO_OUTPUT },            // Status LED
    { .pin = &ENABLE_SWITCH_PIN, .type = GPIO_INPUT_PULLUP },   // Enable switch
    { .pin = &MODE_SELECT_PIN, .type = GPIO_INPUT_PULLUP },     // Mode select switch
};
uint8_t gpio_mappings_size = sizeof(gpio_mappings)/sizeof(struct gpio_mapping);

//---------------- LEDS ----------------//
struct led power_led = {
    .pin = &POWER_LED_PIN,
};

struct led status_led = {
    .pin = &STATUS_LED_PIN,
};

//---------------- MIDI ----------------//
struct midi_config midi_config = {
    .event_handlers = {
        .control_change = handle_midi_control_change,
        .note_off       = handle_midi_note_off,
        .note_on        = handle_midi_note_on,
        .program_change = handle_midi_program_change,
    },
    .omni_mode  = false,
    .rx_channel = MIDI_CHANNEL_01,
    .signal_rx  = false,
    .tx_channel = MIDI_CHANNEL_01,
};
