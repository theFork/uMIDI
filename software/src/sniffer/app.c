/// \file
/// \brief      Implementation of the MIDI sniffer module

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
#include <string.h>

#include "lib/leds.h"
#include "lib/usb.h"

#include "config.h"
#include "app.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

bool exec_channel(const char* command)
{
    // Abort if the command is malformed
    if (strlen(command) < 9 || command[7] != ' ') {
        usb_puts("Malformed command" USB_NEWLINE);
        return false;
    }

    enum midi_channel channel = atoi(command+8);
    if (channel == 0) {
        usb_puts("Listening on all channels");
        set_omni_mode(true);
    }
    else {
        usb_printf("Listening on MIDI channel %u" USB_NEWLINE, channel);
        set_midi_rx_channel(channel-1);
        set_omni_mode(false);
    }

    return true;
}

/// \brief      Handler for the `led` command
bool exec_led(const char* command)
{
    // Abort if the command is malformed
    if (strlen(command) != 7 || command[3] != ' ' || command[5] != ' ') {
        usb_puts("Malformed command" USB_NEWLINE);
        return false;
    }

    // Parse LED(s) to manipulate
    enum led led;
    switch (command[4]) {
    case 'g':
        led = LED_GREEN;
        break;

    case 'r':
        led = LED_RED;
        break;

    case '!':
        led = LED_ALL;
        break;

    default:
        usb_puts("No such LED" USB_NEWLINE);
        return false;
    }

    // Parse and execute action
    switch (command[6]) {
    case 'b':
        blink_led(led, F_TASK_SLOW);
        break;

    case 'f':
        flash_led(led);
        break;

    case 't':
        toggle_led(led);
        break;

    default:
        usb_puts("No such action" USB_NEWLINE);
        return false;
    }

    return true;
}

void handle_control_change(uint8_t controller, uint8_t value)
{
    //usb_printf("CC %u %u" USB_NEWLINE, controller, value);
}

void handle_note_off(uint8_t note)
{
    //usb_printf("NOTE OFF %u" USB_NEWLINE, note);
}

void handle_note_on(uint8_t note)
{
    //usb_printf("NOTE ON %u" USB_NEWLINE, note);
}

void handle_program_change(uint8_t program)
{
    //usb_printf("PC %u %u" USB_NEWLINE, program);
}
