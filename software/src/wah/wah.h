/// \file
/// \brief      Wah-wah module API

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

#ifndef _WAH_H
#define _WAH_H


//---------------- includes ----------------//
#include <stdint.h>


//---------------- constants ----------------//


//---------------- data types ----------------//


//---------------- functions and procedures ----------------//
void enable_wah(bool enable);
bool exec_enable(const char* command);
bool exec_speed(const char* command);
bool exec_tap(const char* command);
bool exec_waveform(const char* command);
void handle_midi_cc(uint8_t controller, uint8_t value);
void handle_switch(void);
void init_wah_module(void);
void update_wah_pwm(void);


//---------------- EOF ----------------//
#endif // _WAH_H
