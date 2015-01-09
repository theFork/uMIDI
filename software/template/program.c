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

// current program
program_t current_program;

// EEPROM program storage
uint16_t programs[PROGRAM_COUNT] EEMEM;



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void applyProgramData(program_data_t data)
{
    // TODO
}

void copyCurrentProgramTo(uint8_t pgm)
{
    // copy program
    updateProgramData(pgm, current_program.data.word);
}

void copyCurrentBankTo( uint8_t targetBank)
{
    // compute the source bank
    uint8_t sourceBank = ( (current_program.number+1) / 10 ) * 10;

    // write programs
    uint16_t data;
    uint8_t program_index;
    for (program_index=0; program_index<10; program_index++) {
        // bank 0 contains only 9 programs, so we have to skip certain actions
        if (program_index == 9) {
            // if bank 0 is the target, do nothing
            if (targetBank == 0) {
                break;
            }

            // bank 0 is the source
            if (sourceBank == 0) {
                data = 0;
            }
        }
        else {
            data = readProgramData(sourceBank + program_index);
        }

        updateProgramData((targetBank*10) + program_index, data);
    }
}

void enterProgram(uint8_t num)
{
    // if the program has not changed, do nothing
    if (current_program.number == num) {
        return;
    }

    // load program and set outputs
    current_program.number = num;
    current_program.data.word = readProgramData(num);
    applyProgramData(current_program.data);
}


uint16_t readProgramData(uint8_t num)
{
    return eeprom_read_word(&programs[num]);
}

void updateProgramData(uint8_t number, uint16_t data)
{
    eeprom_write_word(&programs[number], data);
}

void wipeCurrentProgram(void)
{
    updateProgramData(current_program.number, 0);
    enterProgram(current_program.number);
}

void wipeCurrentBank(void)
{
    // write programs
    uint8_t bank = ( (current_program.number+1) / 10 ) * 10;
    uint8_t i;
    for (i=0; i<10; i++) {
        updateProgramData(bank+i, 0);
    }

    // apply the changes
    enterProgram(current_program.number);
}
