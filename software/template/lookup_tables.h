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
 * The MIDI volume controller firmware is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the uMIDI firmware.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Lookup tables for the uMIDI firmware.
*/

#ifndef _LOOKUP_TABLES_H
#define _LOOKUP_TABLES_H


//---------------- includes ----------------//
#include <stdint.h>


//---------------- Lookup table declarations ----------------//
extern const uint16_t exp_table[];
extern const uint16_t sine_table[];


//---------------- Inline functions to read out the tables ----------------//
inline uint16_t lookup_exp(uint8_t x)
{
    return exp_table[x];
}

inline uint16_t lookup_sine(uint8_t x)
{
    return sine_table[x];
}

//---------------- EOF ----------------//
#endif // LOOKUP_TABLES_H

