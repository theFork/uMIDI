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

#include "lib/midi.h"
#include "lib/usb.h"

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
