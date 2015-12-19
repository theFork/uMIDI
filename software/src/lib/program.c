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
 * EEPROM program storage service functions.
*/

#include "gpio.h"
#include "program.h"

#include <avr/eeprom.h>
#include <util/delay.h>



////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

// EEPROM program storage
static uint16_t program_data_storage[PROGRAM_COUNT] EEMEM;

// Current program
static struct program current_program;

/// \brief      Callback for program execution
static void (*execute_program)(uint16_t);



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void init_program_module(void (*execute_callback)(uint16_t))
{
    execute_program = execute_callback;
}

void copy_current_program_to(uint8_t target_program)
{
    // Copy program
    update_program_data(target_program, current_program.data);
}

void copy_current_bank_to(uint8_t target_bank)
{
    // Compute the source bank
    uint8_t sourceBank = ( (current_program.number+1) / 10 ) * 10;

    // Write programs
    uint16_t data;
    uint8_t program_index;
    for (program_index=0; program_index<10; program_index++) {
        // bank 0 contains only 9 programs, so we have to skip certain actions
        if (program_index == 9) {
            // if bank 0 is the target, do nothing
            if (target_bank == 0) {
                break;
            }

            // bank 0 is the source
            if (sourceBank == 0) {
                data = 0;
            }
        }
        else {
            data = read_program_data(sourceBank + program_index);
        }

        update_program_data((target_bank*10) + program_index, data);
    }
}

void enter_program(uint8_t number)
{
    // If the program has not changed, do nothing
    if (current_program.number == number) {
        return;
    }

    // Load program and set outputs
    current_program.number = number;
    current_program.data = read_program_data(number);
    // TODO apply_program_data(current_program.data);
}

uint16_t read_program_data(uint8_t number)
{
    return eeprom_read_word(&program_data_storage[number]);
}

void update_program_data(uint8_t number, uint16_t data)
{
    eeprom_write_word(&program_data_storage[number], data);
}

void wipe_current_program(void)
{
    update_program_data(current_program.number, 0);
    enter_program(current_program.number);
}

void wipe_current_bank(void)
{
    // Write programs
    uint8_t bank = ( (current_program.number+1) / 10 ) * 10;
    uint8_t i;
    for (i=0; i<10; i++) {
        update_program_data(bank+i, 0);
    }

    // Apply the changes
    enter_program(current_program.number);
}
