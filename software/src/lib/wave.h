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
 * Header for wave computations.
*/

#ifndef _WAVE_H
#define _WAVE_H


//---------------- includes ----------------//
#include <stdint.h>


//---------------- code macros ----------------//
// step size of the random wave
#define     RANDOM_WAVE_STEP_SIZE           4

// the number of levels in the stair wave
#define     STAIR_WAVE_STEPS                5


//---------------- data types ----------------//
enum direction
{
    DIRECTION_DOWN,
    DIRECTION_UP,
};

enum whammy_amplitude
{
    WHAMMY_AMPLITUDE_UNISON             = 0,
    WHAMMY_AMPLITUDE_MINOR_SECOND       = 13,
    WHAMMY_AMPLITUDE_MAJOR_SECOND       = 24,
    WHAMMY_AMPLITUDE_MINOR_THIRD        = 32,
    WHAMMY_AMPLITUDE_MAJOR_THIRD        = 42,
    WHAMMY_AMPLITUDE_PERFECT_FOURTH     = 52,
    WHAMMY_AMPLITUDE_TRITONE            = 63,
    WHAMMY_AMPLITUDE_PERFECT_FIFTH      = 74,
    WHAMMY_AMPLITUDE_MINOR_SIXTH        = 84,
    WHAMMY_AMPLITUDE_MAJOR_SIXTH        = 95,
    WHAMMY_AMPLITUDE_MINOR_SEVENTH      = 106,
    WHAMMY_AMPLITUDE_MAJOR_SEVENTH      = 116,
    WHAMMY_AMPLITUDE_OCTAVE             = 127,
    WHAMMY_AMPLITUDE_1ST_MINOR_SECOND   = 5,
    WHAMMY_AMPLITUDE_1ST_MAJOR_SECOND   = 10,
    WHAMMY_AMPLITUDE_1ST_MINOR_THIRD    = 16,
    WHAMMY_AMPLITUDE_1ST_MAJOR_THIRD    = 21,
    WHAMMY_AMPLITUDE_1ST_PERFECT_FOURTH = 26,
    WHAMMY_AMPLITUDE_1ST_TRITONE        = 32,
    WHAMMY_AMPLITUDE_1ST_PERFECT_FIFTH  = 37,
    WHAMMY_AMPLITUDE_1ST_MINOR_SIXTH    = 42,
    WHAMMY_AMPLITUDE_1ST_MAJOR_SIXTH    = 48,
    WHAMMY_AMPLITUDE_1ST_MINOR_SEVENTH  = 53,
    WHAMMY_AMPLITUDE_1ST_MAJOR_SEVENTH  = 58,
    WHAMMY_AMPLITUDE_1ST_OCTAVE         = 63,
    WHAMMY_AMPLITUDE_2ND_MINOR_SECOND   = 69,
    WHAMMY_AMPLITUDE_2ND_MAJOR_SECOND   = 74,
    WHAMMY_AMPLITUDE_2ND_MINOR_THIRD    = 79,
    WHAMMY_AMPLITUDE_2ND_MAJOR_THIRD    = 85,
    WHAMMY_AMPLITUDE_2ND_PERFECT_FOURTH = 90,
    WHAMMY_AMPLITUDE_2ND_TRITONE        = 95,
    WHAMMY_AMPLITUDE_2ND_PERFECT_FIFTH  = 100,
    WHAMMY_AMPLITUDE_2ND_MINOR_SIXTH    = 106,
    WHAMMY_AMPLITUDE_2ND_MAJOR_SIXTH    = 111,
    WHAMMY_AMPLITUDE_2ND_MINOR_SEVENTH  = 117,
    WHAMMY_AMPLITUDE_2ND_MAJOR_SEVENTH  = 122,
    WHAMMY_AMPLITUDE_2ND_OCTAVE         = 127,
};

enum waveform
{
    WAVE_OFF,
    WAVE_SINE,
    WAVE_TRIANGLE,
    WAVE_SAW_UP,
    WAVE_SAW_DOWN,
    WAVE_SQUARE,
    WAVE_STAIRS,
    WAVE_RANDOM,
    WAVE_PATTERN_01,
    WAVE_PATTERN_02,
    WAVE_PATTERN_03,
    WAVE_PATTERN_04,
    WAVE_PATTERN_05,
    WAVE_PATTERN_06,
    WAVE_PATTERN_07,
    WAVE_PATTERN_08,
};

struct wave_settings
{
    enum waveform   waveform;
    uint8_t         speed;
    uint8_t         amplitude;
    uint8_t         offset;
};

struct wave_state
{
    uint8_t         speed_prescaler;
    uint8_t         speed_counter;
    uint8_t         step_counter;
    enum direction  step_direction;
};

struct wave
{
    struct wave_settings    settings;
    struct wave_state       state;
};


//---------------- functions and procedures ----------------//
void initialize_wave(struct wave* wave, enum waveform waveform, uint8_t speed, uint8_t amplitude, uint8_t offset);
void set_speed(struct wave*, uint8_t);
void set_waveform(struct wave*, enum waveform);
uint8_t update_wave(struct wave*);


//---------------- EOF ----------------//
#endif // _WAVE_H
