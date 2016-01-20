/// \file
/// \brief  Background task scheduler

/*
 * Copyright 2014, 2015 Sebastian Neuser
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

#ifndef _BACKGROUND_TASKS_H
#define _BACKGROUND_TASKS_H


//---------------- includes ----------------//
#include <stdint.h>


//---------------- constants ----------------//

/// \brief      Frequency of the CPU timer used in by the task scheduler in [Hz]
#define     F_TIMER             2000

/// \brief      High priority task frequency in [Hz]
#define     F_TASK_FAST         F_TIMER

/// \brief      Medium priority task frequency in [Hz]
#define     F_TASK_MID          (F_TASK_FAST / 10)

/// \brief      Low priority task frequency in [Hz]
#define     F_TASK_SLOW         (F_TASK_MID / 10)


//---------------- data types ----------------//

/// \brief      Data type for background task pointers
typedef void (*background_task_t)(void);


//---------------- functions and procedures ----------------//

/// \brief      Invokes registered background tasks if certain conditions are met
/// \details    This procedure must be called in the main loop of the program. If the CPU timer
///             interrupt flag of CPU timer 0 is active, it is reset and the prescaler counters are
///             incremented. If one of the prescalers reaches its target value, it is reset as well
///             and the corresponding tasks are executed one by one.
/// \see        F_TASK_FAST
/// \see        F_TASK_MID
/// \see        F_TASK_SLOW
void process_background_tasks(void);

/// \brief      Initializes the background task scheduler
/// \details    Configures CPU timer 0 to provide the main clock for the background task scheduler
///             and registers the tasks to be invoked cyclically.
///             The tasks are grouped into three levels of priority. High priority tasks are
///             executed more frequently.
/// \param      high_freq_tasks
///                 an array of function pointers to high frequency tasks
/// \param      high_freq_tasks_size
///                 number of high frequency tasks
/// \param      mid_freq_tasks
///                 an array of function pointers to medium frequency tasks
/// \param      mid_freq_tasks_size
///                 number of medium frequency tasks
/// \param      low_freq_tasks
///                 an array of function pointers to low frequency tasks
/// \param      low_freq_tasks_size
///                 number of low frequency tasks
/// \see        F_TASK_FAST
/// \see        F_TASK_MID
/// \see        F_TASK_SLOW
void init_background_tasks(background_task_t high_freq_tasks[], uint8_t high_freq_tasks_size,
                           background_task_t mid_freq_tasks[], uint8_t mid_freq_tasks_size,
                           background_task_t low_freq_tasks[], uint8_t low_freq_tasks_size);


//---------------- EOF ----------------//
#endif // _BACKGROUND_TASKS_H

