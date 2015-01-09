/*
 * Copyright 2012-2015 Sebastian Neuser
 *
 * This file is part of the MIDI switcher firmware.
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
 * along with the MIDI switcher firmware.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * header for the program module
*/

#ifndef _PROGRAM
#define _PROGRAM


//---------------- includes ----------------//
#include <stdint.h>


//---------------- constants ----------------//
#define     PROGRAM_COUNT               120


//---------------- data types ----------------//
union program_data
{
    uint16_t word;
    struct {
        // TODO
    } channels;
};

struct program
{
    uint8_t             number;
    union program_data  data;
};


//---------------- functions and procedures ----------------//
void applyProgramData(union program_data);
void copyCurrentBankTo(uint8_t);
void copyCurrentProgramTo(uint8_t);
void enterProgram(uint8_t);
uint16_t readProgramData(uint8_t);
void updateProgramData(uint8_t, uint16_t);
void wipeCurrentProgram(void);
void wipeCurrentBank(void);


//---------------- EOF ----------------//
#endif // _PROGRAM
