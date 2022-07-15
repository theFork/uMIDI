/// \file
/// \brief      MIDI handling routines

/*
 * Copyright 2018 Sebastian Neuser
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

#include "lib/midi.h"
#include "lib/program.h"
#include "lib/usb.h"

#include "config.h"
#include "user_interface.h"
#include "whammy_controller.h"


////////////////////////////////////////////////////////////////
//               P R I V A T E   D E F I N E S                //
////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//              P R I V A T E   T Y P E D E F S               //
////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//          F O R W A R D   D E C L A R A T I O N S           //
////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
// S T A T I C   F U N C T I O N S   A N D   P R O C E D U R E S //
///////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void handle_midi_control_change(const midi_value_t controller, const midi_value_t value)
{
    usb_printf(PSTR("Received MIDI control change num: %u, val: %u" USB_NEWLINE), controller, value);
    signal_midi_rx();

    uint16_t accu = value;
    switch (get_active_program().field.ctrl_mode) {
        case WHAMMY_CTRL_MODE_LIMIT:
            accu *= get_active_program().field.range;
            accu /= MIDI_MAX_VALUE;
        case WHAMMY_CTRL_MODE_NORMAL:
            break;
        default:
            return;
    }
    send_control_change(WHAMMY_MIDI_CC_NUMBER, accu);
}

void handle_midi_note_off(midi_value_t note, midi_value_t velocity)
{
    usb_printf(PSTR("Received MIDI note off num: %u, vel: %u" USB_NEWLINE), note, velocity);
    signal_midi_rx();

}

void handle_midi_note_on(midi_value_t note, midi_value_t velocity)
{
    usb_printf(PSTR("Received MIDI note on num: %u, vel: %u" USB_NEWLINE), note, velocity);
    signal_midi_rx();

    if (get_active_program().field.ctrl_mode == WHAMMY_CTRL_MODE_PATTERN) {
        if (velocity) {
            update_pattern_step();
        }
        return;
    }

    if (get_active_program().field.ctrl_mode != WHAMMY_CTRL_MODE_MOMENTARY) {
        return;
    }

    if (velocity) {
        // Enable Whammy pedal
        send_program_change(get_active_program().field.pedal_mode);
        // Pitch bend
        send_control_change(WHAMMY_MIDI_CC_NUMBER, get_active_program().field.range);
    }
    else {
        // Release pitch bend
        send_control_change(WHAMMY_MIDI_CC_NUMBER, 0);
        // Turn off Whammy pedal
        send_program_change(WHAMMY_MODE_OFF);
    }
}

void handle_midi_program_change(midi_value_t program)
{
    usb_printf(PSTR("Received MIDI program change: #%u" USB_NEWLINE), program+1);
    signal_midi_rx();
    enter_program(program);
}
