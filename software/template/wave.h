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
// the number of levels in the stair wave
#define     STAIR_WAVE_STEPS                5


//---------------- data types ----------------//
enum direction
{
    DIRECTION_DOWN,
    DIRECTION_UP,
};

enum waveform
{
    WAVE_OFF,
    WAVE_SINE,
    WAVE_TRIANGLE,
    WAVE_SAW_UP,
    WAVE_SAW_DOWN,
    WAVE_SQUARE,
    WAVE_STAIRS
};

struct wave_settings
{
    enum waveform   waveform;
    uint8_t         speed;
    uint8_t         max_value;
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
void initialize_wave(struct wave*, uint8_t, enum waveform, uint8_t);
void set_speed(struct wave*, uint8_t);
void set_waveform(struct wave*, enum waveform);
uint8_t update_wave(struct wave*);


//---------------- EOF ----------------//
#endif // _WAVE_H
