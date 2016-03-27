/// \file
/// \brief      Whammy module implementation

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
#include "lib/sequencer.h"
#include "lib/usb.h"
#include "lib/wave.h"

#include "config.h"
#include "whammy.h"


////////////////////////////////////////////////////////////////
//               P R I V A T E   D E F I N E S                //
////////////////////////////////////////////////////////////////

#define     STRING_NEXT     "next"
#define     STRING_PREV     "prev"



////////////////////////////////////////////////////////////////
//          F O R W A R D   D E C L A R A T I O N S           //
////////////////////////////////////////////////////////////////

static void sequencer_tick_handler(void);



////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

static const struct sequencer_pattern factory_patterns[] = {
    { // SEQUENCER_PATTERN_01
        .length     = 8,
        .steps      = {
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_UNISON, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_OCTAVE, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_UNISON, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_OCTAVE, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_UNISON, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_OCTAVE, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_UNISON, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_OCTAVE, },
        }
    },

    { // SEQUENCER_PATTERN_02
        .length     = 8,
        .steps      = {
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_UNISON, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_1ST_PERFECT_FIFTH, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_1ST_OCTAVE, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_2ND_PERFECT_FIFTH, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_2ND_OCTAVE, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_2ND_PERFECT_FIFTH, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_1ST_OCTAVE, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_1ST_PERFECT_FIFTH, },
        }
    },
    { // SEQUENCER_PATTERN_03
        .length     = 8,
        .steps      = {
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_UNISON, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_OCTAVE, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_OCTAVE, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_UNISON, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_OCTAVE, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_OCTAVE, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_UNISON, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_OCTAVE, },
        }
    },
};
static const uint8_t factory_patterns_size = sizeof(factory_patterns) / sizeof(struct sequencer_pattern);

static struct sequencer_channel sequencer = {
    .pattern        = SEQUENCER_PATTERN_01,
    .speed          = 40,
    .mode           = SEQUENCER_CHANNEL_MODE_CONTINUOUS,
    .running        = true,
    .tick_callback  = &sequencer_tick_handler,
};



///////////////////////////////////////////////////////////////////
// S T A T I C   F U N C T I O N S   A N D   P R O C E D U R E S //
///////////////////////////////////////////////////////////////////

static void sequencer_tick_handler(void)
{
    show_led_pattern(0x80 >> sequencer.step_index);
}



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

bool exec_backup(const char* command)
{
    // Abort if the command is malformed
    if (strlen(command) != 6) {
        usb_puts(PSTR("Malformed command" USB_NEWLINE));
        return false;
    }

    for (uint8_t i=0; i<SEQUENCER_PATTERNS; ++i) {
        usb_puts_S(export_pattern(i));
    }
    return true;
}

bool exec_factory_reset(const char* command)
{
    // Abort if the command is malformed
    if (strlen(command) != 12) {
        usb_puts(PSTR("Malformed command" USB_NEWLINE));
        return false;
    }

    init_sequencer_patterns(factory_patterns, factory_patterns_size);
    return true;
}

bool exec_pattern(const char* command)
{
    if (strlen(command) < 12 || command[7] != ' ') {
        usb_puts(PSTR("Malformed command" USB_NEWLINE));
        return false;
    }

    if (strncmp(STRING_NEXT, command+8, sizeof(STRING_NEXT)) == 0) {
        select_next_pattern();
        return true;
    }
    if (strncmp(STRING_PREV, command+8, sizeof(STRING_PREV)) == 0) {
        select_previous_pattern();
        return true;
    }

    usb_puts(PSTR("Unknown parameter" USB_NEWLINE));
    return false;
}

bool exec_speed(const char* command)
{
    if (strlen(command) < 7 || command[5] != ' ') {
        usb_puts(PSTR("Malformed command" USB_NEWLINE));
        return false;
    }

    midi_value_t speed = atoi(command+6);
    speed %= MIDI_MAX_VALUE + 1;
    usb_printf(PSTR("Setting waveform speed to %u" USB_NEWLINE), speed);
    set_sequencer_speed(&sequencer, speed);
    return true;
}

bool exec_tap(const char* command)
{
    register_tap();
    return true;
}

void decrease_speed(void)
{
    usb_printf(PSTR("Set speed to %d" USB_NEWLINE), adjust_sequencer_speed(&sequencer, -1));
}

void increase_speed(void)
{
    usb_printf(PSTR("Set speed to %d" USB_NEWLINE), adjust_sequencer_speed(&sequencer, 1));
}

void init_whammy_module(void)
{
    configure_sequencer_channel(SEQUENCER_CHANNEL_1, &sequencer);
}

void select_next_pattern(void)
{
    usb_printf(PSTR("Selected pattern %d" USB_NEWLINE), adjust_sequencer_pattern(&sequencer, 1));
}

void select_previous_pattern(void)
{
    usb_printf(PSTR("Selected pattern %d" USB_NEWLINE), adjust_sequencer_pattern(&sequencer, -1));
}

void tap_tempo(void)
{
    register_tap();
}

void toggle_sequencer_mode(void)
{
    if (sequencer.mode == SEQUENCER_CHANNEL_MODE_CONTINUOUS) {
        sequencer.mode = SEQUENCER_CHANNEL_MODE_ONE_SHOT;
    }
    else {
        sequencer.mode = SEQUENCER_CHANNEL_MODE_CONTINUOUS;
    }
    usb_printf(PSTR("Setting sequencer mode %d" USB_NEWLINE), sequencer.mode);
}

void toggle_sequencing(void)
{
    toggle_sequencer(&sequencer);
    if (!sequencer.running) {
        show_led_pattern(0x00);
    }
}
