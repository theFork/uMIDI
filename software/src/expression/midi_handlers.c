/// \file
/// \brief      MIDI handling routines

/*
 * Copyright 2018 Sebastian Neuser
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

#include <stdlib.h>

#include "lib/midi.h"

#include "config.h"


////////////////////////////////////////////////////////////////
//               P R I V A T E   D E F I N E S                //
////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//              P R I V A T E   T Y P E D E F S               //
////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//          F O R W A R D   D E C L A R A T I O N S           //
////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//        P R I V A T E   I M P L E M E N T A T I O N         //
////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//         P U B L I C   I M P L E M E N T A T I O N          //
////////////////////////////////////////////////////////////////

void handle_midi_control_change(const midi_value_t controller, const midi_value_t value)
{
    send_control_change(controller, value);
}

void handle_midi_note_off(midi_value_t note, midi_value_t velocity)
{
    send_note_off(note, velocity);
}

void handle_midi_note_on(midi_value_t note, midi_value_t velocity)
{
    send_note_on(note, velocity);
}

void handle_midi_program_change(midi_value_t program)
{
    send_program_change(program);
}
