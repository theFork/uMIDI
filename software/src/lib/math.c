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

void init_linear_from_midi(struct linear_range* const config)
{
    config->slope = fixed_div(fixed_from_int(config->to - config->from),
                              fixed_from_int(MIDI_MAX_VALUE));
}

void init_linear_to_midi(struct linear_range* config)
{
    config->slope = fixed_div(fixed_from_int(MIDI_MAX_VALUE),
                              fixed_from_int(config->to - config->from));
}

uint16_t linear_from_midi(const struct linear_range* const config, midi_value_t midi_value)
{
    return fixed_to_int(config->slope * input + fixed_from_int(config->from));
}

midi_value_t linear_to_midi(const struct linear_range* config, uint16_t input)
{
    return fixed_to_int(config->slope * input + fixed_from_int(config->from));
}
