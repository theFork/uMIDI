/// \file
/// \brief  Background task scheduler

/*
 * Copyright 2014, 2015 Sebastian Neuser
 *
 * This file is part of the uMIDI firmware.
 *
 * the uMIDI firmware is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * the uMIDI firmware is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the uMIDI firmware.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "background_tasks.h"

#include <avr/io.h>


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

/// \param      Internal array of function pointers to high frequency tasks
static background_task_t* high_frequency_tasks;

/// \param      Number of high frequency tasks
static uint8_t high_frequency_tasks_size;

/// \param      Internal array of function pointers to medium frequency tasks
static background_task_t* mid_frequency_tasks;

/// \param      Number of medium frequency tasks
static uint8_t mid_frequency_tasks_size;

/// \param      Internal array of function pointers to low frequency tasks
static background_task_t* low_frequency_tasks;

/// \param      Number of low frequency tasks
static uint8_t low_frequency_tasks_size;



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void process_background_tasks(void)
{
    if (!TCE0.INTFLAGS & TC0_OVFIF_bm) {
        return;
    }
    TCE0.INTFLAGS |= TC0_OVFIF_bm;

    ////
    // High frequency tasks
    ////
    static uint16_t prescaler_fast = 0;
    ++prescaler_fast;
    if (prescaler_fast >= F_TIMER / F_TASK_FAST) {
        prescaler_fast = 0;

        for (int i = 0; i < high_frequency_tasks_size; i++) {
            high_frequency_tasks[i]();
        }
    }

    ////
    // Mid frequency tasks
    ////
    static uint16_t prescaler_medium = 0;
    ++prescaler_medium;
    if (prescaler_medium >= F_TIMER / F_TASK_MID) {
        prescaler_medium = 0;

        for (int i = 0; i < mid_frequency_tasks_size; i++) {
            mid_frequency_tasks[i]();
        }
    }

    ////
    // Low frequency tasks
    ////
    static uint16_t prescaler_slow = 0;
    ++prescaler_slow;
    if (prescaler_slow >= F_TIMER / F_TASK_SLOW) {
        prescaler_slow = 0;

        for (int i = 0; i < low_frequency_tasks_size; i++) {
            low_frequency_tasks[i]();
        }
    }
}

void init_background_tasks(background_task_t * const high_freq_tasks, uint8_t high_freq_tasks_size,
                           background_task_t * const mid_freq_tasks, uint8_t mid_freq_tasks_size,
                           background_task_t * const low_freq_tasks, uint8_t low_freq_tasks_size)
{
    // Prescale clock to 500 kHz
    TCE0.CTRLA = TC_CLKSEL_DIV64_gc;

    // Set TOP value to achieve desired clock
    TCE0.PER = 500000 / F_TIMER - 1;

    // Save pointers to and size of task arrays
    high_frequency_tasks = high_freq_tasks;
    high_frequency_tasks_size = high_freq_tasks_size;
    mid_frequency_tasks = mid_freq_tasks;
    mid_frequency_tasks_size = mid_freq_tasks_size;
    low_frequency_tasks = low_freq_tasks;
    low_frequency_tasks_size = low_freq_tasks_size;
}
