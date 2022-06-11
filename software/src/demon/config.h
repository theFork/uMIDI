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


//---------------- constant declarations ----------------//
extern struct gpio_mapping gpio_mappings[];
extern uint8_t gpio_mappings_size;

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

