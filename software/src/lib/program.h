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
 * Header for the program module
*/

#ifndef _PROGRAM
#define _PROGRAM


//---------------- includes ----------------//
#include <stdint.h>


//---------------- constants ----------------//
#define     PROGRAM_COUNT               120


//---------------- data types ----------------//

struct program
{
    uint8_t     number;
    uint16_t    data;
};


//---------------- functions and procedures ----------------//

/// \brief      Callback for program execution
/// \details    This function is called whenever a new program is entered after configuration
///             data was read from EEPROM memory.
/// \param      program_data
///                 This is the program configuration data that was read from memory.
void init_program_module(void (*execute_callback)(uint16_t));

void copy_current_bank_to(uint8_t bank_number);

void copy_current_program_to(uint8_t target_bank);

void enter_program(uint8_t number);

uint16_t read_program_data(uint8_t number);

void update_program_data(uint8_t number, uint16_t data);

void wipe_current_program(void);

void wipe_current_bank(void);


//---------------- EOF ----------------//
#endif // _PROGRAM
