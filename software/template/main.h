/*
 * Copyright 2012, 2013 Sebastian Neuser
 *
 * This file is part of the muMIDI firmware.
 *
 * The muMIDI firmware is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The muMIDI firmware is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the muMIDI firmware.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Main header for the muMIDI firmware.
 *
 * @author Sebastian Neuser
 */

#ifndef _MIDIWAH_H
#define _MIDIWAH_H


//---------------- includes ----------------//
#include <stdint.h>

#include "bool.h"
#include "midi.h"


//---------------- constants ----------------//
#define	INITIAL_PROGRAM		0


//---------------- data types ----------------//
// execution state
typedef struct {
    midi_state_t    midi;
    bool            receive;
} exec_state_t;


//---------------- EOF ----------------//
#endif // _MIDIWAH_H
