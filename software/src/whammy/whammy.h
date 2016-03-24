/// \file
/// \brief      Whammy module API

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

#ifndef _WHAMMY_APP_H
#define _WHAMMY_APP_H


//---------------- includes ----------------//
#include <stdint.h>
#include "lib/gpio.h"
#include "lib/wave.h"


//---------------- constants ----------------//


//---------------- data types ----------------//


//---------------- functions and procedures ----------------//
bool exec_pattern(const char* command);
bool exec_speed(const char* command);
bool exec_tap(const char* command);
void decrease_speed(void);
void increase_speed(void);
void init_whammy_module(void);
void select_next_pattern(void);
void select_previous_pattern(void);
void tap_tempo(void);
void toggle_sequencing(void);

//---------------- EOF ----------------//
#endif // _WHAMMY_APP_H

