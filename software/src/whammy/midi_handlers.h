/// \file
/// \brief      Header for MIDI handling routines

/*
 * Copyright 2018 Sebastian Neuser
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

#ifndef _MIDI_HANDLERS_H
#define _MIDI_HANDLERS_H


//---------------- includes ----------------//
#include <stdint.h>


//---------------- constants ----------------//


//---------------- data types ----------------//


//---------------- functions and procedures ----------------//
void handle_midi_control_change(midi_value_t controller, midi_value_t value);
void handle_midi_note_off(midi_value_t note, midi_value_t velocity);
void handle_midi_note_on(midi_value_t note, midi_value_t velocity);
void handle_midi_program_change(midi_value_t program);


//---------------- EOF ----------------//
#endif // _MIDI_HANDLERS_H

