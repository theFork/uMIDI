/// \file
/// \brief      API for a small collection of mathematical functions

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

#ifndef _MATH_H
#define _MATH_H

//---------------- includes -----------------//
#include <stdbool.h>
#include <avr/io.h>

#include "midi.h"


//---------------- constants ----------------//


//---------------- data types ---------------//

/// \brief      Configuration for a linear function.
struct linear_range
{
    uint16_t    from;       ///< The value that will be returned by linear() when called with midi_value = 0
    uint16_t    to;         ///< The value that will be returned by linear() whenn called with midi_value = 127
    int16_t     slope;      ///< The slope of the linear function (computed in init_linear())
};


//---------------- functions ----------------//

/// \brief      Initializes a linear function
/// \param      config
///                 the configuration of the linear function to initialize
void init_linear(struct linear_range* config);

/// \brief      Calculates a linear function from a given midi value
/// \param      config
///                 the configuration of the linear function to initialize
/// \param      midi_value
///                 the MIDI value to convert [0 .. 127]
uint16_t linear(const struct linear_range* config, midi_value_t midi_value);

#endif // _MATH_H
