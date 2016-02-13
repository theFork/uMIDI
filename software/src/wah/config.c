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

#include "lib/adc.h"
#include "lib/background_tasks.h"
#include "lib/gpio.h"
#include "lib/leds.h"
#include "lib/midi.h"
#include "lib/serial_communication.h"
#include "lib/usb.h"
#include "lib/wave.h"

#include "config.h"
#include "wah.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

//---------------- GPIO ----------------//
struct gpio_config gpio_config = {
    .header1 = {
        .pin2 = { &PORTA, 0, GPIO_UNUSED },
        .pin3 = { &PORTA, 1, GPIO_UNUSED },
        .pin4 = { &PORTA, 2, GPIO_UNUSED },
        .pin5 = { &PORTA, 3, GPIO_UNUSED },
        .pin6 = { &PORTA, 4, GPIO_UNUSED },
        .pin7 = { &PORTA, 5, GPIO_UNUSED },
        .pin8 = { &PORTA, 6, GPIO_UNUSED },
        .pin9 = { &PORTA, 7, GPIO_UNUSED }
    },
    .header2 = {
        .pin2 = { &PORTB, 0, GPIO_UNUSED },
        .pin3 = { &PORTB, 1, GPIO_UNUSED },
        .pin4 = { &PORTB, 2, GPIO_UNUSED },
        .pin5 = { &PORTB, 3, GPIO_UNUSED },
        .pin6 = { &PORTC, 0, GPIO_UNUSED },
        .pin7 = { &PORTC, 1, GPIO_UNUSED },
        .pin8 = { &PORTC, 2, GPIO_UNUSED },
        .pin9 = { &PORTC, 3, GPIO_UNUSED }
    },
    .header3 = {
        .pin2 = { &PORTC, 4, GPIO_OUTPUT },         // Wah LED PWM
        .pin3 = { &PORTC, 5, GPIO_UNUSED },
        .pin4 = { &PORTC, 6, GPIO_OUTPUT },         // Wah bypass relais
        .pin5 = { &PORTC, 7, GPIO_UNUSED },
        .pin6 = { &PORTD, 0, GPIO_INPUT_PULLUP },   // Toggle switch
        .pin7 = { &PORTD, 1, GPIO_UNUSED },
        .pin8 = { &PORTD, 2, GPIO_OUTPUT },         // Toggle switch LED
        .pin9 = { &PORTD, 3, GPIO_UNUSED }
    },
    .jumpers = {
        .jp2 =  { &PORTR, 0, GPIO_INPUT_PULLUP },
        .jp3 =  { &PORTR, 1, GPIO_INPUT_PULLUP },
        .jp4 =  { &PORTD, 4, GPIO_INPUT_PULLUP },
        .jp5 =  { &PORTD, 5, GPIO_INPUT_PULLUP }
    }
};

//---------------- MIDI ----------------//
struct midi_config midi_config = {
    .event_handlers = {
        .control_change = handle_midi_cc,
        .note_off       = NULL,
        .note_on        = NULL,
        .program_change = NULL
    },
    .tx_channel = 1,
};

//---------------- Background tasks ----------------//
background_task_t high_frequency_tasks[] = {
    &serial_communication_task,
    &update_wah_pwm,
};
uint8_t high_frequency_tasks_size = sizeof(high_frequency_tasks)/sizeof(background_task_t);

background_task_t mid_frequency_tasks[] = {
    &usb_main_task,
};
uint8_t mid_frequency_tasks_size = sizeof(mid_frequency_tasks)/sizeof(background_task_t);

background_task_t low_frequency_tasks[] = {
    &handle_switch,
    &tap_tempo_task,
    &update_leds,
};
uint8_t low_frequency_tasks_size = sizeof(low_frequency_tasks)/sizeof(background_task_t);

//---------------- Custom commands ----------------//
struct serial_command serial_commands[] = {
    {
        .cmd_string = "enable",
        .help_string = "<a>\n"
            "Enable or disable the effect:\n"
            "<a> : action\n"
            "      't' = enable\n"
            "       ?  = disable",
        .handler = &exec_enable
    },
    {
        .cmd_string = "speed",
        .help_string = "<s>\n"
            "Adjust the speed of the effect if in waveform mode:\n"
            "<s> : wave speed\n",
        .handler = &exec_speed
    },
    {
        .cmd_string = "tap",
        .help_string = "\nSend this command repeatedly to tap in a tempo\n",
        .handler = &exec_tap
    },
    {
        .cmd_string = "waveform",
        .help_string = "<w>\n"
            "En-/disable waveform mode and set waveform:\n"
            "<w> : waveform\n"
            "      \"next\" = switch to next waveform\n"
            "      \"prev\" = switch to previous waveform\n"
            "      \"off\"  = disable",
        .handler = &exec_waveform
    },
};
uint8_t serial_commands_size = sizeof(serial_commands) / sizeof(struct serial_command);
