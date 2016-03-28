/// \file
/// \brief      Program storage module implementation

/*
 * Copyright 2015 Sebastian Neuser
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

/*
 * EEPROM program storage service functions.
*/

#include <avr/eeprom.h>
#include <util/delay.h>

#include "gpio.h"
#include "program.h"



////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

/// \brief      EEPROM program storage
static uint32_t program_data_storage[PROGRAM_COUNT] EEMEM;

/// \brief      Initialization value for empty programs
static uint32_t program_initializer;

/// \brief      Current program
static struct program current_program;

/// \brief      Callback for program execution
static void (*execute_program)(uint32_t);



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void copy_current_bank_to(uint8_t target_bank)
{
    // Compute the source bank
    uint8_t sourceBank = ( (current_program.number+1) / 10 ) * 10;

    // Write programs
    uint32_t data;
    uint8_t program_index;
    for (program_index=0; program_index<10; program_index++) {
        // Bank 0 contains only 9 programs, so we have to skip certain actions
        if (program_index == 9) {
            // If bank 0 is the target, do nothing
            if (target_bank == 0) {
                break;
            }

            // If bank 0 is the source, write an empty program
            if (sourceBank == 0) {
                data = program_initializer;
            }
        }
        else {
            data = read_program_data(sourceBank + program_index);
        }

        write_program((target_bank*10) + program_index, data);
    }
}

void copy_current_program_to(uint8_t target_program)
{
    // Copy program
    write_program(target_program, current_program.data);
}

void enter_program(uint8_t number)
{
    // If the program has not changed, do nothing
    if (current_program.number == number) {
        return;
    }

    // Load program and execute program
    current_program.number = number;
    current_program.data = read_program_data(number);
    execute_program(current_program.data);
}

void init_program_module(uint32_t program_initializer_value, void (* const execute_program_callback)(uint32_t program_data))
{
    program_initializer = program_initializer_value;
    execute_program = execute_program_callback;
    current_program.number = 0;
    current_program.data = program_initializer;
}

uint32_t read_program_data(uint8_t number)
{
    return eeprom_read_dword(&program_data_storage[number]);
}

void update_current_program(uint32_t program_data)
{
    current_program.data = program_data;
    write_program(current_program.number, program_data);
}

void wipe_current_bank(void)
{
    // Write programs
    uint8_t bank = ( (current_program.number+1) / 10 ) * 10;
    uint8_t i;
    for (i=0; i<10; i++) {
        write_program(bank+i, program_initializer);
    }

    // Apply the changes
    enter_program(current_program.number);
}

void wipe_current_program(void)
{
    write_program(current_program.number, program_initializer);
    enter_program(current_program.number);
}

void write_program(uint8_t number, uint32_t data)
{
    eeprom_write_dword(&program_data_storage[number], data);
}
