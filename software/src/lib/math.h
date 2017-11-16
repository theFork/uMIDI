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
#include <stdint.h>

#include "midi.h"


//---------------- constants ----------------//
/// \brief      Width of the fractional part of fixed point numbers
#define FIXED_POINT     16


//---------------- data types ---------------//
/// \brief      Datatype for fixed point numbers
typedef uint32_t        fixed_t;

/// \brief      Datatype for accumulators in fixed point number arithmetic functions
typedef uint64_t        fixed_accu_t;

/// \brief      Configuration for a linear scaling function.
struct linear_range
{
    uint16_t    from;       ///< The lowest input (#linear_to_midi) / output (#linear_from_midi) value
    uint16_t    to;         ///< The highest input (#linear_to_midi) / output (#linear_from_midi) value
    fixed_t     slope;      ///< The slope of the linear function (computed in init_linear_...())
};


//---------------- prototypes ----------------//

/// \brief      Initializes a linear function to map from MIDI values to an arbitrary integer range
/// \param      config
///                 the configuration of the linear function to initialize
void init_linear_from_midi(struct linear_range* config);

/// \brief      Initializes a linear function to map from an arbitrary integer range to MIDI values
/// \param      config
///                 the configuration of the linear function to initialize
void init_linear_to_midi(struct linear_range* config);

/// \brief      Yields an integer in the configured range that corresponds to the given MIDI value
/// \param      config
///                 the configuration of the linear function
/// \param      midi_value
///                 the MIDI value to convert [0,127]
/// \returns    the input scaled to the configured range
uint16_t linear_from_midi(const struct linear_range* config, midi_value_t midi_value);

/// \brief      Yields the MIDI value [0,127] from a given integer in the configured range
/// \param      config
///                 the configuration of the linear function
/// \param      input
///                 the integer to convert to a MIDI value
/// \returns    the input scaled to the MIDI range [0,127]
midi_value_t linear_to_midi(const struct linear_range* config, uint16_t input);


//---------------- inline functions ----------------//

/// \brief      Converts an integer to a fixed point number
/// \param      value
///                 the integer value to convert
/// \returns    the value as fixed point number
static inline fixed_t fixed_from_int(const uint16_t value)
{
    fixed_t result = value;
    result <<= FIXED_POINT;
    return result;
}

/// \brief      Performs a division of two fixed point numbers
/// \details    Note: You do not have to use this function when you want to divide a fixed point
///             number by a "normal" integer, because the division done in this function cancels out
///             the multiplication in fixed_from_int().
/// \param      a
///                 the dividend
/// \param      b
///                 the divisor
/// \returns    the quotient as fixed point number
static inline fixed_t fixed_div(const fixed_t a, const fixed_t b)
{
    fixed_accu_t accu = a;
    accu <<= FIXED_POINT;
    accu /= b;
    return (fixed_t) accu;
}

/// \brief      Multiplies two fixed point numbers
/// \details    Note: You do not have to use this function when one of the two operands you want to
///             multiply is a "normal" integer, because the division done in this function and the
///             multiplication in fixed_from_int() cancel out.
/// \param      a
///                 the first operand
/// \param      b
///                 the second operand
/// \returns    the product as fixed point number
static inline fixed_t fixed_mpy(const fixed_t a, const fixed_t b)
{
    fixed_accu_t accu = a;
    accu *= b;
    accu >>= FIXED_POINT;
    return (fixed_t) accu;
}

/// \brief      Converts fixed point number to an integer
/// \param      value
///                 the fixed point number to convert
/// \returns    the rounded integer part of the fixed point number
static inline uint16_t fixed_to_int(fixed_t value)
{
    // Round
    if (value & 0x00008000) {
        value += 0x00008000;
    }

    // Drop fracional part
    value >>= FIXED_POINT;
    return (uint16_t) value;
}

#endif // _MATH_H
