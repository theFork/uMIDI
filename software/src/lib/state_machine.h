/*
 * Copyright 2014,2015 Sebastian Neuser
 *
 * This file is part of the uMIDI firmware.
 *
 * The MIDI volume controller firmware is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The MIDI volume controller firmware is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the uMIDI firmware.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Header for the state machine module of the uMIDI firmware
*/

#ifndef _STATE_MACHINE_H
#define _STATE_MACHINE_H


//---------------- includes ----------------//
#include <stdint.h>


//---------------- constants ----------------//
#define     F_TIMER             2000
#define     F_TASK_FAST         F_TIMER
#define     F_TASK_MID          (F_TASK_FAST / 10)
#define     F_TASK_SLOW         (F_TASK_MID / 10)


//---------------- data types ----------------//
typedef void (*state_machine_task_t)(void);


//---------------- functions and procedures ----------------//
void handle_state_machine(void);
void initialize_state_machine(state_machine_task_t high_freq_tasks[], uint8_t high_freq_tasks_size,
                              state_machine_task_t mid_freq_tasks[], uint8_t mid_freq_tasks_size,
                              state_machine_task_t low_freq_tasks[], uint8_t low_freq_tasks_size);


//---------------- EOF ----------------//
#endif // _STATE_MACHINE_H

