/// \file
/// \brief      Program storage API
/// \details    This module takes care of loading, storing, copying and wiping program data.
///             The programs are stored in EEPROM as 32-bit dwords - providing a generic container
///             for flags, numbers - whatever is suitable for your program. It is advisable to
///             define a `union` for easier access of these components in your application code -
///             for example something like
///             \code
///             union program_data {
///                 uint32_t word;
///                 struct {
///                     bool flag0 : 1;
///                     bool flag1 : 1;
///                     bool flag2 : 1;
///                 } bit;
///             };
///             \endcode
///             The programs are grouped into twelve banks of ten programs each - leaving 1568
///             bytes of EEPROM for other application specific data.
///             Applications must implement and register a callback for program execution, which
///             does whatever is necessary when a new program is loaded.
/// \see        init_program_module

/*
 * Copyright 2015, 2016 Sebastian Neuser
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

#ifndef _PROGRAM_H
#define _PROGRAM_H


//---------------- includes ----------------//
#include <stdbool.h>
#include <stdint.h>


//---------------- constants ----------------//
/// \brief      The maximum number of program banks
#define     PROGRAM_BANK_COUNT          12

/// \brief      The maximum number of programs that can be stored
#define     PROGRAM_COUNT               120

/// \brief      The number of program per bank
#define     PROGRAMS_PER_BANK           (PROGRAM_COUNT / PROGRAM_BANK_COUNT)


//---------------- data types ----------------//
/// \brief      Data structure for program information
struct program
{
    uint8_t     number;     ///< Program number [0..#PROGRAM_COUNT-1].
    uint32_t    data;       ///< The application specific program data as a 32-bit word.
};


//---------------- functions and procedures ----------------//

/// \brief      Copies all programs in the current bank to another bank.
/// \details    Irreversibly overwrites all programs in the target bank.
/// \param      target_bank
///                 Target bank number [0..11].
void copy_current_bank_to(uint8_t target_bank);

/// \brief      Copies the current program to the specified position in memory.
/// \details    Irreversibly overwrites the target program.
/// \param      target_program
///                 Target program number [0..119].
void copy_current_program_to(uint8_t target_program);

/// \brief      Exports a program bank
/// \details    Reads the data from EEPROM and converts it to a string of hexadecimal digits.
/// \param      number
///                 the number of the program bank to export
/// \returns    the programs stored in EEPROM as a hex-string
char* export_bank(const uint8_t number);

/// \brief      Loads and executes the specified program.
/// \details    Reads the new program data from EEPROM, updates the internal state and calls the
///             registered callback function, which executes the program.
///             If the specified program number is the same as the stored current program number,
///             this function does nothing.
/// \param      number
///                 Program number [0..119].
/// \see init_program_module
void enter_program(uint8_t number);

/// \brief      Imports and stores a program bank given as a hex-string
/// \details    Overwrites stored program data!
/// \param      number
///                 the number of the program bank to import [0..11]
/// \param      data
///                 the program bank to store as a string of hexadecimal digits
/// \returns    `true` if sanity checks passed
bool import_bank(const uint8_t number, const char* data);

/// \brief      Initializes the program storage module.
/// \details    Stores the specified program data initialization word, used in the wipe functions
///             and registers the provided callback function, which is executed whenever the current
///             program data word is updated either by wiping the program or loading a new one.
/// \param      program_initializer
///                 On wiping, programs are initialized with this value.
/// \param      execute_program_callback
///                 Callback for program execution. This procedure is called with the program
///                 configuration data that was read from memory as the parameter `program_data`.
/// \see        wipe_current_program
/// \see        enter_program
void init_program_module(uint32_t program_initializer, void (*execute_program_callback)(uint32_t program_data));

/// \brief      Reads a program data word from the specified position in memory.
/// \param      number
///                 Program number [0..119].
uint32_t read_program_data(uint8_t number);

/// \brief      Updates and stores the current program
/// \param      program_data
///                 the new configuration
void update_current_program(uint32_t program_data);

/// \brief      Reinitializes all programs in the current bank.
/// \see        wipe_current_program
void wipe_current_bank(void);

/// \brief      Reinitializes the currently loaded program.
/// \details    Updates the internal program data word with the initializer, stores it and executes
///             the initialized program.
void wipe_current_program(void);

/// \brief      Stores a given program data word in the specified position in memory.
/// \param      number
///                 Program number [0..119].
/// \param      data
///                 Program data as 32-bit dword.
void write_program(uint8_t number, uint32_t data);

/// \brief      Stores a given program data word at last read position in memory.
/// \param      data
///                 Program data as 32-bit dword.
void update_program(uint32_t data);


//---------------- EOF ----------------//
#endif // _PROGRAM_H
