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
#include "lib/encoder.h"
#include "lib/gpio.h"
#include "lib/leds.h"
#include "lib/midi.h"
#include "lib/serial_communication.h"
#include "lib/usb.h"
#include "lib/wave.h"

#include "config.h"
#include "sequencer.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

//---------------- GPIO ----------------//
struct gpio_mapping gpio_mappings[] = {
    { .pin = &gpio.header1.pin2, .type = GPIO_INPUT_PULLUP },
    { .pin = &gpio.header1.pin3, .type = GPIO_INPUT_PULLUP },
    { .pin = &gpio.header1.pin4, .type = GPIO_INPUT_PULLUP },
    { .pin = &gpio.header1.pin6, .type = GPIO_OUTPUT       },
    { .pin = &gpio.header1.pin7, .type = GPIO_OUTPUT       },
    { .pin = &gpio.header1.pin8, .type = GPIO_OUTPUT       },
    { .pin = &gpio.header1.pin9, .type = GPIO_OUTPUT       },
};
uint8_t gpio_mappings_size = sizeof(gpio_mappings)/sizeof(struct gpio_mapping);

//---------------- Encoder ----------------//
struct encoder_config encoder_config = {
    .inputA = &gpio.header1.pin3,
    .inputB = &gpio.header1.pin2,
    .inputSwitch = &gpio.header1.pin4,

    .cw_callback = &encoder_cw_callback,
    .ccw_callback = &encoder_ccw_callback,
    .push_callback = &encoder_push_callback,
};

//---------------- MIDI ----------------//
struct midi_config midi_config = {
    .event_handlers = {
//        .control_change = NULL,
        .control_change = &handle_control_change,
        .note_off       = NULL,
        .note_on        = NULL,
        .program_change = NULL
    },
    .tx_channel = 1,
};

//---------------- Sequencers ----------------//
struct sequencer_config sequencer_config = {
    .controller_number = 11,
    .waveform   = WAVE_PATTERN_01,
    .speed      = 60
};

const struct gpio_pin* sequencer_leds[] = {
    &gpio.header1.pin6,
    &gpio.header1.pin7,
    &gpio.header1.pin8,
    &gpio.header1.pin9,
};
uint8_t sequencer_leds_size = sizeof(sequencer_leds)/sizeof(struct gpio_pin*);

//---------------- State machine ----------------//
background_task_t high_frequency_tasks[] = {
    &serial_communication_task,
    &update_sequencer,
    (void (*)(void)) &poll_encoder,
};
uint8_t high_frequency_tasks_size = sizeof(high_frequency_tasks)/sizeof(background_task_t);

background_task_t mid_frequency_tasks[] = {
    &usb_main_task,
};
uint8_t mid_frequency_tasks_size = sizeof(mid_frequency_tasks)/sizeof(background_task_t);

background_task_t low_frequency_tasks[] = {
    &tap_tempo_task,
    &update_leds,
};
uint8_t low_frequency_tasks_size = sizeof(low_frequency_tasks)/sizeof(background_task_t);

//---------------- Custom commands ----------------//
struct serial_command serial_commands[] = {
    {
        .cmd_string = "speed",
        .help_string = "<s>\n"
            "Adjust the speed of the sequencer:\n"
            "<s> : wave speed\n",
        .handler = &exec_speed
    },
    {
        .cmd_string = "tap",
        .help_string = "\nSend this command repeatedly to tap in a tempo\n",
        .handler = &exec_tap
    },
    {
        .cmd_string = "pattern",
        .help_string = "<p>\n"
            "Select sequencer pattern:\n"
            "<p> : pattern\n"
            "      \"next\" = switch to next pattern\n"
            "      \"prev\" = switch to previous pattern\n",
        .handler = &exec_pattern
    },
};
uint8_t serial_commands_size = sizeof(serial_commands) / sizeof(struct serial_command);
