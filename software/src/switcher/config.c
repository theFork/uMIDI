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
#include "app.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

//---------------- GPIO ----------------//
struct gpio_config gpio_config = {
    // GPIO1: Relais hard-wired to status LEDs
    .header1 = {
        .pin2 = { &PORTA, 0, GPIO_OUTPUT }, // Tune and mute
        .pin3 = { &PORTA, 1, GPIO_OUTPUT }, // Loop 1
        .pin4 = { &PORTA, 2, GPIO_OUTPUT }, // Loop 2
        .pin5 = { &PORTA, 3, GPIO_OUTPUT }, // Loop 3
        .pin6 = { &PORTA, 4, GPIO_OUTPUT }, // Loop 4
        .pin7 = { &PORTA, 5, GPIO_OUTPUT }, // Loop 5
        .pin8 = { &PORTA, 6, GPIO_OUTPUT }, // Switch 1
        .pin9 = { &PORTA, 7, GPIO_OUTPUT }  // Switch 2
    },

    // GPIO2: Toggle switches (all low-active)
    .header2 = {
        .pin2 = { &PORTB, 0, GPIO_INPUT_PULLUP }, // Toggle Tune and mute
        .pin3 = { &PORTB, 1, GPIO_INPUT_PULLUP }, // Toggle Loop 1
        .pin4 = { &PORTB, 2, GPIO_INPUT_PULLUP }, // Toggle Loop 2
        .pin5 = { &PORTB, 3, GPIO_INPUT_PULLUP }, // Toggle Loop 3
        .pin6 = { &PORTC, 0, GPIO_INPUT_PULLUP }, // Toggle Loop 4
        .pin7 = { &PORTC, 1, GPIO_INPUT_PULLUP }, // Toggle Loop 5
        .pin8 = { &PORTC, 2, GPIO_INPUT_PULLUP }, // Toggle Switch 1
        .pin9 = { &PORTC, 3, GPIO_INPUT_PULLUP }  // Toggle Switch 2
    },

    // GPIO3: "Save" toggle switch and LED
    .header3 = {
        .pin2 = { &PORTC, 4, GPIO_OUTPUT },         // "Save" LED
        .pin3 = { &PORTC, 5, GPIO_INPUT_PULLUP },   // "Save" toggle switch
        .pin4 = { &PORTC, 6, GPIO_UNUSED },
        .pin5 = { &PORTC, 7, GPIO_UNUSED },
        .pin6 = { &PORTD, 0, GPIO_UNUSED },
        .pin7 = { &PORTD, 1, GPIO_UNUSED },
        .pin8 = { &PORTD, 2, GPIO_UNUSED },
        .pin9 = { &PORTD, 3, GPIO_UNUSED }
    }
};

//---------------- MIDI ----------------//
struct midi_config midi_config = {
    .event_handlers = {
        .program_change = handle_program_change
    },
    .tx_channel = 1,
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
struct serial_command serial_commands[] = {
    {
        .cmd_string = "led",
        .help_string = "<l> <a>\n"
                       "Manipulates the two on-board LEDs:\n"
                       "<l> : LED to manipulate\n"
                       "      'g' = green LED\n"
                       "      'r' = red LED\n"
                       "      '!' = both LEDs\n"
                       "<a> : LED mode / action\n"
                       "      'b' = blink\n"
                       "      'f' = flash\n"
                       "      't' = toggle",
        .handler = &exec_led
    },
};
uint8_t serial_commands_size = sizeof(serial_commands) / sizeof(struct serial_command);
