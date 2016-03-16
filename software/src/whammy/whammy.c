/// \file
/// \brief      Sequencer module implementation

/*
 * Copyright 2015 Sebastian Neuser
 *
 * This file is part of the uMIDI firmware.
 *
 * The uMIDI firmware is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The uMIDI firmware is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the uMIDI firmware.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>

#include <util/delay.h>
#include "lib/gpio.h"
#include "lib/hmi.h"
#include "lib/leds.h"
#include "lib/midi.h"
#include "lib/wave.h"
#include "lib/usb.h"

#include "config.h"
#include "whammy.h"


////////////////////////////////////////////////////////////////
//               P R I V A T E   D E F I N E S                //
////////////////////////////////////////////////////////////////

#define     STRING_NEXT     "next"
#define     STRING_PREV     "prev"



////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
// S T A T I C   F U N C T I O N S   A N D   P R O C E D U R E S //
///////////////////////////////////////////////////////////////////

/*
static void step_sequencer_leds(void)
{
    show_led_pattern(0x80 >> step_counter);
    ++step_counter;
    step_counter %= 8;
}
*/



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

bool exec_speed(const char* command)
{
    if (strlen(command) < 7 || command[5] != ' ') {
        usb_puts("Malformed command" USB_NEWLINE);
        return false;
    }

    midi_value_t speed = atoi(command+6);
    speed %= MIDI_MAX_VALUE + 1;
    usb_printf("Setting waveform speed to %u" USB_NEWLINE, speed);
    //set_speed(&wave, speed);
    return true;
}

bool exec_tap(const char* command)
{
    register_tap();
    return true;
}

bool exec_waveform(const char* command)
{
    if (strlen(command) < 13 || command[8] != ' ') {
        usb_puts("Malformed command" USB_NEWLINE);
        return false;
    }

    if (strncmp(STRING_NEXT, command+9, sizeof(STRING_NEXT)) == 0) {
        //select_next_waveform();
        return true;
    }
    if (strncmp(STRING_PREV, command+9, sizeof(STRING_PREV)) == 0) {
        //select_previous_waveform();
        return true;
    }

    usb_puts("Unknown parameter" USB_NEWLINE);
    return false;
}

void handle_control_change(uint8_t controller_number, uint8_t value)
{
}

void tap_tempo(void)
{
    register_tap();
}
