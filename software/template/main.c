/*
 * Copyright 2012-2014 Sebastian Neuser
 *
 * This file is part of the muMIDI firmware.
 *
 * The muMIDI firmware is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The muMIDI firmware is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the muMIDI firmware.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Program entry point and main loop of the muMIDI firmware.
 */

#include "main.h"
#include "midi.h"
#include "gpio.h"

#include <avr/interrupt.h>
#include <avr/wdt.h>


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

// main state variable struct
exec_state_t    state = {
    IDLE,
    true
};



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

// initialization and endless loop
int main( void )
{
    // configure GPIO ports
    configureGPIO();

    // configure USART for MIDI operation
    configureUSART();

    // set watchdog for 120ms
    wdt_enable(WDTO_120MS);

    // enable interrupts
    sei();

    // main program
    while (true) {
        // handle watchdog
        if (MCUSR & _BV(WDRF)) {
        }
        wdt_reset();
    }

    return 0;
}
