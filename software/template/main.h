/*
 * Copyright 2012-2015 Sebastian Neuser
 *
 * This file is part of the uMIDI firmware.
 *
 * The uMIDI firmware is free software: you can redistribute it and/or modify
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
 * Main header for the uMIDI firmware.
 */

#ifndef _MAIN_H
#define _MAIN_H


//---------------- includes ----------------//
#include "midi.h"

#include <stdbool.h>
#include <stdint.h>


//---------------- constants ----------------//
#define	INITIAL_PROGRAM		0


//---------------- data types ----------------//
// execution state
typedef struct {
    enum midi_state midi;
    bool            receive;
} exec_state_t;


//---------------- EOF ----------------//
#endif // _MAIN_H
