/// \file
/// \brief      Implementation of mathematical functions

/*
 * Copyright 2015 Simon Gansen
 *
 * This file is part of the uMIDI firmware.
 *
 * the uMIDI firmware is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * the uMIDI firmware is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the uMIDI firmware.  If not, see <http://www.gnu.org/licenses/>.
 */

//---------------- includes ----------------//
#include "math.h"
#include "midi.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void init_linear(struct linear_range* config)
{
    config->slope = (int16_t)(config->to - config->from) / MIDI_MAX_VALUE;
}

uint16_t linear(struct linear_range* config, midi_value_t midi_value)
{
    return config->slope * midi_value + config->from;
}
