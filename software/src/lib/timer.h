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
 * Header for the timer module of the uMIDI firmware
*/

#ifndef _TIMER_H
#define _TIMER_H


//---------------- includes ----------------//


//---------------- constants ----------------//
#define     F_TIMER             2000
#define     F_TASK_FAST         F_TIMER
#define     F_TASK_MID          (F_TASK_FAST / 10)
#define     F_TASK_SLOW         (F_TASK_MID / 10)


//---------------- data types ----------------//


//---------------- functions and procedures ----------------//
void configure_task_timer(void);


//---------------- EOF ----------------//
#endif // _TIMER_H

