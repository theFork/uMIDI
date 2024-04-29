/// \file
/// \brief      Device configuration header

/*
 * Copyright 2015 Sebastian Neuser
 *
 * This file is part of the uMIDI firmware.
 *
 * The MIDI volume controller firmware is free software: you can redistribute it and/or modify
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

#ifndef _CONFIG_H
#define _CONFIG_H


//---------------- includes ----------------//
#include <stdint.h>

#include "lib/adc.h"
#include "lib/background_tasks.h"
#include "lib/gpio.h"
#include "lib/midi.h"
#include "lib/serial_communication.h"

//--------- application specific gpio defines --------//

// Hard-wired relays/leds
#define GPIO_OUT_TUNE_MUTE  gpio.header1.pin2
#define GPIO_OUT_LOOP1      gpio.header1.pin3
#define GPIO_OUT_LOOP2      gpio.header1.pin4
#define GPIO_OUT_LOOP3      gpio.header1.pin5
#define GPIO_OUT_LOOP4      gpio.header1.pin6
#define GPIO_OUT_LOOP5      gpio.header1.pin7
#define GPIO_OUT_SWITCH1    gpio.header1.pin8
#define GPIO_OUT_SWITCH2    gpio.header1.pin9

// Toggle switches
#define GPIO_IN_TUNE_MUTE   gpio.header2.pin2
#define GPIO_IN_LOOP1       gpio.header2.pin3
#define GPIO_IN_LOOP2       gpio.header2.pin4
#define GPIO_IN_LOOP3       gpio.header2.pin5
#define GPIO_IN_LOOP4       gpio.header2.pin6
#define GPIO_IN_LOOP5       gpio.header2.pin7
#define GPIO_IN_SWITCH1     gpio.header2.pin8
#define GPIO_IN_SWITCH2     gpio.header2.pin9

// Save button (with led)
#define GPIO_OUT_SAVE_LED   gpio.header3.pin2
#define GPIO_IN_SAVE_SWITCH gpio.header3.pin3

//---------------- constant declarations ----------------//
extern struct gpio_mapping gpio_mappings[];
extern uint8_t gpio_mappings_size;

extern struct led save_led;

extern struct midi_config midi_config;

extern background_task_t high_frequency_tasks[];
extern uint8_t high_frequency_tasks_size;
extern background_task_t mid_frequency_tasks[];
extern uint8_t mid_frequency_tasks_size;
extern background_task_t low_frequency_tasks[];
extern uint8_t low_frequency_tasks_size;

extern struct serial_command serial_commands[];
extern uint8_t serial_commands_size;

//---------------- EOF ----------------//
#endif // _CONFIG_H

