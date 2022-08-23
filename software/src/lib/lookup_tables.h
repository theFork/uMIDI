/// \file
/// \brief      Lookup tables for the uMIDI firmware.
/// \details    The tables are computed with the PERL script `compute_lookup_tables.pl`, which
///             generates the file lookup_tables.c from scratch.
///             The lookup tables are declared globally and used in other modules, so this is not
///             a stand-alone module but rather a component of those modules.

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
 * The MIDI volume controller firmware is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the uMIDI firmware.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _LOOKUP_TABLES_H
#define _LOOKUP_TABLES_H


//---------------- includes ----------------//
#include <stdint.h>
#ifndef UNIT_TEST
#include <avr/pgmspace.h>
#else
#define pgm_read_word(x) (*x)
#define PROGMEM
#endif


//---------------- Lookup table declarations ----------------//

/// \brief  Resolution (y-axis) of the lookup tables in bits
extern const uint8_t  lookup_table_resolution;

/// \brief  Lookup table for an exponential function
extern const uint16_t exp_table[] PROGMEM;

/// \brief  Lookup table for the natural logaritm function
extern const uint16_t log_table[] PROGMEM;

/// \brief  Lookup table for the sine function
extern const uint16_t sine_table[] PROGMEM;


//---------------- Inline functions to read out the tables ----------------//

/// \brief      Looks up an exponential value in the corresponding table
/// \details    This inline function does *not* check array bounds!
/// \param      x
///                 index of the value in the table
/// \return     the function value
inline uint16_t lookup_exp(uint8_t x)
{
    return pgm_read_word(&(exp_table[x]));
}

/// \brief      Looks up the natural log value in the corresponding table
/// \details    This inline function does *not* check array bounds!
/// \param      x
///                 index of the value in the table
/// \return     the function value
inline uint16_t lookup_log(uint8_t x)
{
    return pgm_read_word(&(log_table[x]));
}

/// \brief      Looks up a sine function value in the corresponding table
/// \details    This inline function does *not* check array bounds!
/// \param      x
///                 index of the value in the table
/// \return     the function value
inline uint16_t lookup_sine(uint8_t x)
{
    return pgm_read_word(&(sine_table[x]));
}

//---------------- EOF ----------------//
#endif // LOOKUP_TABLES_H

