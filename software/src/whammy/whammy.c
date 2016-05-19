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

bool exec_patcpy(const char* command)
{
    if (strlen(command) < 8 || command[6] != ' ') {
        usb_puts(PSTR("Malformed command" USB_NEWLINE));
        return false;
    }

    uint8_t number = atoi(command+7);
    copy_pattern(sequencer.pattern, number);

    return true;
}

bool exec_patdump(const char* command)
{
    if (strlen(command) != 7) {
        usb_puts(PSTR("Malformed command" USB_NEWLINE));
        return false;
    }

    uint8_t length = get_pattern_length(sequencer.pattern);
    usb_printf(PSTR("Pattern length: %u" USB_NEWLINE), length);

    usb_puts(PSTR("Steps:" USB_NEWLINE "## Chan Type Dat0 Dat1"));
    for (uint8_t step_index=0; step_index < length; ++step_index) {
        struct sequencer_step step = get_pattern_step(sequencer.pattern, step_index);

        // Prepare pretty output strings
        char* type_string = "";
        switch (step.type) {
            case MIDI_MSG_TYPE_CONTROL_CHANGE:
                type_string = "CCH";
                break;
            case MIDI_MSG_TYPE_NOTE_OFF:
                type_string = "NOF";
                break;
            case MIDI_MSG_TYPE_NOTE_ON:
                type_string = "NON";
                break;
            case MIDI_MSG_TYPE_PROGRAM_CHANGE:
                type_string = "PCH";
                break;
            default:
                type_string = "INV";
                break;
        }
        char data1_string[5] = {'\0',};
        if (step.type != MIDI_MSG_TYPE_PROGRAM_CHANGE) {
            sprintf(data1_string, "%4u", step.data1);
        }

        usb_printf(PSTR("%2u %4u %4s %4u %s" USB_NEWLINE),
                   step_index+1, step.channel+1, type_string, step.data0, data1_string);
    }

    return true;
}

bool exec_patlen(const char* command)
{
    if (strlen(command) < 8 || command[6] != ' ') {
        usb_puts(PSTR("Malformed command" USB_NEWLINE));
        return false;
    }

    uint8_t length = atoi(command+7);
    set_pattern_length(sequencer.pattern, length);

    return true;
}

bool exec_patmod(const char* command)
{
    if (strlen(command) != 22 || command[6] != ' ' || command[9] != ' ' || command[12] != ' '
                              || command[14] != ' ' || command[18] != ' ') {
        usb_puts(PSTR("Malformed command" USB_NEWLINE));
        return false;
    }

    uint8_t step_index = atoi(command+7) - 1;
    if (step_index >= SEQUENCER_STEPS_PER_PATTERN) {
        usb_puts(PSTR("Invalid pattern index"));
        return false;
    }

    struct sequencer_step step = {0,};
    step.channel = atoi(command+10) - 1;
    switch (*(command+13)) {
        case 'c':
            step.type = MIDI_MSG_TYPE_CONTROL_CHANGE;
            break;
        case 'f':
            step.type = MIDI_MSG_TYPE_NOTE_OFF;
            break;
        case 'n':
            step.type = MIDI_MSG_TYPE_NOTE_ON;
            break;
        case 'p':
            step.type = MIDI_MSG_TYPE_PROGRAM_CHANGE;
            break;
        default:
            break;
    }
    step.data0 = atoi(command+15);
    step.data1 = atoi(command+19);

    set_pattern_step(sequencer.pattern, step_index, &step);

    return true;
}

bool exec_patsel(const char* command)
{
    if (strlen(command) < 8 || command[6] != ' ') {
        usb_puts(PSTR("Malformed command" USB_NEWLINE));
        return false;
    }

    if (strncmp(STRING_NEXT, command+7, sizeof(STRING_NEXT)) == 0) {
        select_next_pattern();
        return true;
    }
    if (strncmp(STRING_PREV, command+7, sizeof(STRING_PREV)) == 0) {
        select_previous_pattern();
        return true;
    }

    uint8_t number = atoi(command+7);
    if (number < SEQUENCER_PATTERNS) {
        set_sequencer_pattern(&sequencer, number);
        return true;
    }

    usb_puts(PSTR("Unknown parameter" USB_NEWLINE));
    return false;
}

bool exec_patwipe(const char* command)
{
    if (strlen(command) != 7) {
        usb_puts(PSTR("Malformed command" USB_NEWLINE));
        return false;
    }

    wipe_pattern(sequencer.pattern);

    return true;
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
