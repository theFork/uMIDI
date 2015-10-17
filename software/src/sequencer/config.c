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

#include "lib/encoder.h"
#include "lib/gpio.h"
#include "lib/leds.h"
#include "lib/midi.h"
#include "lib/background_tasks.h"
#include "lib/wave.h"

#include "config.h"
#include "sequencer.h"


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
        .pin6 = { &PORTA, 4, GPIO_OUTPUT },
        .pin7 = { &PORTA, 5, GPIO_OUTPUT },
        .pin8 = { &PORTA, 6, GPIO_OUTPUT },
        .pin9 = { &PORTA, 7, GPIO_OUTPUT }
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
        .pin2 = { &PORTC, 4, GPIO_UNUSED },
        .pin3 = { &PORTC, 5, GPIO_UNUSED },
        .pin4 = { &PORTC, 6, GPIO_UNUSED },
        .pin5 = { &PORTC, 7, GPIO_UNUSED },
        .pin6 = { &PORTD, 0, GPIO_UNUSED },
        .pin7 = { &PORTD, 1, GPIO_UNUSED },
        .pin8 = { &PORTD, 2, GPIO_UNUSED },
        .pin9 = { &PORTD, 3, GPIO_UNUSED }
    }
};

//---------------- Encoder ----------------//
struct encoder_config encoder_config = {
    .inputA = &gpio_config.header2.pin2,
    .inputA = &gpio_config.header2.pin3,
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
    .waveform   = WAVE_RANDOM,
    .speed      = 60
};

struct gpio_pin* sequencer_leds[] = {
    &gpio_config.header1.pin6,
    &gpio_config.header1.pin7,
    &gpio_config.header1.pin8,
    &gpio_config.header1.pin9,
};
uint8_t sequencer_leds_size = sizeof(sequencer_leds)/sizeof(struct gpio_pin*);

//---------------- State machine ----------------//
background_task_t high_frequency_tasks[] = {
    &update_sequencer,
};
uint8_t high_frequency_tasks_size = sizeof(high_frequency_tasks)/sizeof(background_task_t);

background_task_t mid_frequency_tasks[] = {};
uint8_t mid_frequency_tasks_size = sizeof(mid_frequency_tasks)/sizeof(background_task_t);

background_task_t low_frequency_tasks[] = {
    &update_leds,
    &update_sequencer_leds,
};
uint8_t low_frequency_tasks_size = sizeof(low_frequency_tasks)/sizeof(background_task_t);
