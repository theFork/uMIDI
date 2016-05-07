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


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

//---------------- ADC ----------------//
const struct adc_config adc_config = {
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
    &update_leds,
};
uint8_t low_frequency_tasks_size = sizeof(low_frequency_tasks)/sizeof(background_task_t);

//---------------- Commands ----------------//
struct serial_command serial_commands[] = {
    {
        .cmd_string = "calibrate",
        .help_string = "Interactively calibrate the pedal",
        .handler = &exec_calibration
    },
};
uint8_t serial_commands_size = sizeof(serial_commands) / sizeof(struct serial_command);

//---------------- GPIO ----------------//
struct gpio_mapping gpio_mappings[] = {
    { .pin = &gpio.header1.pin6, .type = GPIO_INPUT },          // ADC input
    { .pin = &POWER_LED_PIN, .type = GPIO_OUTPUT },             // Power LED
    { .pin = &STATUS_LED_PIN, .type = GPIO_OUTPUT },            // Status LED
    { .pin = &ENABLE_SWITCH_PIN, .type = GPIO_INPUT_PULLUP },   // Enable switch
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
        .control_change = NULL,
        .note_off       = NULL,
        .note_on        = NULL,
        .program_change = NULL
    },
    .omni_mode  = false,
    .rx_channel = MIDI_CHANNEL_01,
    .signal_rx  = false,
    .tx_channel = MIDI_CHANNEL_01,
};
