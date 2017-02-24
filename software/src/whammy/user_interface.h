/// \file
/// \brief      API for the user interface (using console and HMI) of the whammy controller.

/*
 * Copyright 2015, 2016 Sebastian Neuser
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

#ifndef _USER_INTERFACE_H
#define _USER_INTERFACE_H


//---------------- includes ----------------//
#include <stdint.h>
#include "lib/gpio.h"
#include "lib/wave.h"


//---------------- constants ----------------//


//---------------- data types ----------------//

enum ui_ctrl_mode
{
    UI_CTRL_MODE_BYPASS,
    UI_CTRL_MODE_DETUNE,
    UI_CTRL_MODE_MOMENTARY,
    UI_CTRL_MODE_WAVE_SINE,
    UI_CTRL_MODE_WAVE_TRIANGLE,
    UI_CTRL_MODE_WAVE_SAW_UP,
    UI_CTRL_MODE_WAVE_SAW_DOWN,
    UI_CTRL_MODE_WAVE_SQUARE,
    UI_CTRL_MODE_WAVE_STAIRS,
    UI_CTRL_MODE_WAVE_RANDOM,
    UI_CTRL_MODE_PATTERN_01,
    UI_CTRL_MODE_PATTERN_02,
    UI_CTRL_MODE_PATTERN_03,
    UI_CTRL_MODE_PATTERN_04,
    UI_CTRL_MODE_PATTERN_05,
    UI_CTRL_MODE_PATTERN_06,
    UI_CTRL_MODE_PATTERN_07,
    UI_CTRL_MODE_PATTERN_08,
    UI_CTRL_MODE_PATTERN_09,
    UI_CTRL_MODE_PATTERN_10,
    UI_CTRL_MODE_PATTERN_11,
    UI_CTRL_MODE_PATTERN_12,
    UI_CTRL_MODE_PATTERN_13,
    UI_CTRL_MODE_PATTERN_14,
    UI_CTRL_MODE_PATTERN_15,
    UI_CTRL_MODE_PATTERN_16,
    UI_CTRL_MODE_PATTERN_17,
    UI_CTRL_MODE_PATTERN_18,
    UI_CTRL_MODE_PATTERN_19,
    UI_CTRL_MODE_PATTERN_20,
};


//---------------- functions and procedures ----------------//
bool exec_ampl(const char* command);
bool exec_backup(const char* command);
bool exec_dump(const char* command);
bool exec_factory_reset(const char* command);
bool exec_mode(const char* command);
bool exec_patcpy(const char* command);
bool exec_patlen(const char* command);
bool exec_patmod(const char* command);
bool exec_patwipe(const char* command);
bool exec_pgm(const char* command);
bool exec_speed(const char* command);
bool exec_store(const char* command);
bool exec_tap(const char* command);
bool exec_wham(const char* command);
void clear_displays(void);
void cycle_hmi_layer(void);
void store_setup(void);
void update_displays(void);
void value1_decrement(void);
void value1_increment(void);
void value2_decrement(void);
void value2_increment(void);
void visualize_sequencer(uint8_t value);
void visualize_wave(uint8_t value);

//---------------- EOF ----------------//
#endif // _USER_INTERFACE_H

