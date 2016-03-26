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

#ifndef _WHAMMY_H
#define _WHAMMY_H


//---------------- includes ----------------//
#include <stdint.h>
#include "lib/gpio.h"
#include "lib/wave.h"


//---------------- constants ----------------//


//---------------- data types ----------------//

/// \brief      Amplitudes that represent certain intervals on a DigiTech Whammy pedal
enum whammy_note
{
    WHAMMY_NOTE_UNISON              = 0,    ///< Unison, valid in every mode
    WHAMMY_NOTE_MINOR_SECOND        = 13,   ///< Minor second in 1oct mode
    WHAMMY_NOTE_MAJOR_SECOND        = 24,   ///< Major second in 1oct mode
    WHAMMY_NOTE_MINOR_THIRD         = 32,   ///< Minor third in 1oct mode
    WHAMMY_NOTE_MAJOR_THIRD         = 42,   ///< Major third in 1oct mode
    WHAMMY_NOTE_PERFECT_FOURTH      = 52,   ///< Perfect fourth in 1oct mode
    WHAMMY_NOTE_TRITONE             = 63,   ///< Tritone in 1oct mode
    WHAMMY_NOTE_PERFECT_FIFTH       = 74,   ///< Perfect fifth in 1oct mode
    WHAMMY_NOTE_MINOR_SIXTH         = 84,   ///< Minor sixth in 1oct mode
    WHAMMY_NOTE_MAJOR_SIXTH         = 95,   ///< Major sixth in 1oct mode
    WHAMMY_NOTE_MINOR_SEVENTH       = 106,  ///< Minor seventh in 1oct mode
    WHAMMY_NOTE_MAJOR_SEVENTH       = 116,  ///< Major seventh in 1oct mode
    WHAMMY_NOTE_OCTAVE              = 127,  ///< Octave in 1oct mode
    WHAMMY_NOTE_1ST_MINOR_SECOND    = 5,    ///< First minor second in 2oct mode
    WHAMMY_NOTE_1ST_MAJOR_SECOND    = 10,   ///< First major second in 2oct mode
    WHAMMY_NOTE_1ST_MINOR_THIRD     = 16,   ///< First minor third in 2oct mode
    WHAMMY_NOTE_1ST_MAJOR_THIRD     = 21,   ///< First major third in 2oct mode
    WHAMMY_NOTE_1ST_PERFECT_FOURTH  = 26,   ///< First perfect fourth in 2oct mode
    WHAMMY_NOTE_1ST_TRITONE         = 32,   ///< First tritone in 2oct mode
    WHAMMY_NOTE_1ST_PERFECT_FIFTH   = 37,   ///< First perfect fifth in 2oct mode
    WHAMMY_NOTE_1ST_MINOR_SIXTH     = 42,   ///< First minor sixth in 2oct mode
    WHAMMY_NOTE_1ST_MAJOR_SIXTH     = 48,   ///< First major sixth in 2oct mode
    WHAMMY_NOTE_1ST_MINOR_SEVENTH   = 53,   ///< First minor seventh in 2oct mode
    WHAMMY_NOTE_1ST_MAJOR_SEVENTH   = 58,   ///< First major seventh in 2oct mode
    WHAMMY_NOTE_1ST_OCTAVE          = 63,   ///< First octave in 2oct mode
    WHAMMY_NOTE_2ND_MINOR_SECOND    = 69,   ///< Second minor second in 2oct mode
    WHAMMY_NOTE_2ND_MAJOR_SECOND    = 74,   ///< Second major second in 2oct mode
    WHAMMY_NOTE_2ND_MINOR_THIRD     = 79,   ///< Second minor third in 2oct mode
    WHAMMY_NOTE_2ND_MAJOR_THIRD     = 85,   ///< Second major third in 2oct mode
    WHAMMY_NOTE_2ND_PERFECT_FOURTH  = 90,   ///< Second perfect fourth in 2oct mode
    WHAMMY_NOTE_2ND_TRITONE         = 95,   ///< Second tritone in 2oct mode
    WHAMMY_NOTE_2ND_PERFECT_FIFTH   = 100,  ///< Second perfect fifth in 2oct mode
    WHAMMY_NOTE_2ND_MINOR_SIXTH     = 106,  ///< Second minor sixth in 2oct mode
    WHAMMY_NOTE_2ND_MAJOR_SIXTH     = 111,  ///< Second major sixth in 2oct mode
    WHAMMY_NOTE_2ND_MINOR_SEVENTH   = 117,  ///< Second minor seventh in 2oct mode
    WHAMMY_NOTE_2ND_MAJOR_SEVENTH   = 122,  ///< Second major seventh in 2oct mode
    WHAMMY_NOTE_2ND_OCTAVE          = 127,  ///< Second octave in 2oct mode
};


//---------------- functions and procedures ----------------//
bool exec_backup(const char* command);
bool exec_factory_reset(const char* command);
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
#endif // _WHAMMY_H

