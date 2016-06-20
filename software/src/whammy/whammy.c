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
#include "lib/program.h"
#include "lib/sequencer.h"
#include "lib/usb.h"
#include "lib/wave.h"

#include "config.h"
#include "whammy.h"


////////////////////////////////////////////////////////////////
//               P R I V A T E   D E F I N E S                //
////////////////////////////////////////////////////////////////



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

static union whammy_ctrl_program active_program = {
    .field.ctrl_mode = WHAMMY_CTRL_MODE_BYPASS,
};



///////////////////////////////////////////////////////////////////
// S T A T I C   F U N C T I O N S   A N D   P R O C E D U R E S //
///////////////////////////////////////////////////////////////////

/// \brief      Enters bypass (off) mode
static void enter_bypass_mode(void)
{
    stop_sequencer(&sequencer);

    usb_puts(PSTR("Enabling bypass"));
    active_program.field.ctrl_mode = WHAMMY_CTRL_MODE_BYPASS;
    send_program_change(WHAMMY_MODE_OFF);
}

/// \brief      Enters momentary (pitch bend) mode
static void enter_momentary_mode(void)
{
    stop_sequencer(&sequencer);

    usb_puts(PSTR("Entering momentary mode"));
    active_program.field.ctrl_mode = WHAMMY_CTRL_MODE_MOMENTARY;
    send_program_change(WHAMMY_MODE_OFF);
}

/// \brief      Enters pattern mode
static void enter_pattern_mode(const enum sequencer_pattern_number pattern)
{
    usb_puts(PSTR("Entering pattern mode"));
    active_program.field.ctrl_mode = WHAMMY_CTRL_MODE_PATTERN;
    active_program.field.waveform = pattern;
    set_sequencer_pattern(&sequencer, pattern);
    start_sequencer(&sequencer);
    usb_printf(PSTR("Pattern %d" USB_NEWLINE), pattern+1);
}

/// \brief      Enters wave mode
static void enter_wave_mode(const enum waveform waveform)
{
    stop_sequencer(&sequencer);

    usb_puts(PSTR("Entering wave mode"));
    active_program.field.ctrl_mode = WHAMMY_CTRL_MODE_WAVE;
    active_program.field.waveform = waveform;
    usb_printf(PSTR("Waveform %d" USB_NEWLINE), waveform);
}

/// \brief      Updates the HMI led bar to reflect the current sequencer pattern step index
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

bool exec_mode(const char* command)
{
    if (strlen(command) < 6 || command[4] != ' ') {
        usb_puts(PSTR("Malformed command" USB_NEWLINE));
        return false;
    }

    switch (*(command+5)) {
        case 'N':
            select_next_mode();
            return true;

        case 'P':
            select_previous_mode();
            return true;

        case 'b':
            enter_bypass_mode();
            return true;

        case 'm':
            enter_momentary_mode();
            return true;

        case 'p': {
            uint8_t number = atoi(command+7) - 1;
            if (number < SEQUENCER_PATTERNS) {
                enter_pattern_mode(number);
                return true;
            }
        }

        case 'w':
            // TODO Switch to wave mode
            return true;
    }

    usb_puts(PSTR("Unknown parameter" USB_NEWLINE));
    return false;
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

bool exec_store(const char* command)
{
    // Abort if the command is malformed
    if (strlen(command) < 11 || command[5] != ' ' || command[7] != ' '
    ||  (command[6] != 'P' && command[6] != 'p')) {
        usb_puts(PSTR("Malformed command" USB_NEWLINE));
        return false;
    }

    // Extract pattern or program number
    uint8_t number = strtol(&command[8], NULL, 16);

    if (command[6] == 'P') {
        usb_printf(PSTR("Storing pattern #%d..." USB_NEWLINE), number);
        import_pattern(number, &command[10]);
    }
    if (command[6] == 'p') {
        --number;
        usb_printf(PSTR("Storing program #%d..." USB_NEWLINE), number);
        write_program(number, strtol(&command[10], NULL, 16));
    }

    return true;
}

bool exec_tap(const char* command)
{
    register_tap();
    return true;
}

void execute_program_callback(uint32_t program_data)
{
    // Update active controller program in RAM
    active_program.word = program_data;

    // Apply Whammy controller mode
    switch (active_program.field.ctrl_mode) {
        case WHAMMY_CTRL_MODE_WAVE:
            stop_sequencer(&sequencer);
            show_led_pattern(0x00);

            // TODO Set up wave
            // active_program.speed -> speed
            // active_program.additional -> amplitude
            // active_program.waveform -> waveform
            break;

        case WHAMMY_CTRL_MODE_PATTERN:
            // Set up sequencer according to the loaded program
            set_sequencer_pattern(&sequencer, active_program.field.waveform);
            set_sequencer_speed(&sequencer, active_program.field.speed);
            start_sequencer(&sequencer);
            break;

        case WHAMMY_CTRL_MODE_MOMENTARY:
        default:
            stop_sequencer(&sequencer);
            show_led_pattern(0x00);

            send_control_change(WHAMMY_MIDI_CC_NUMBER, 0);
            break;
    }

    // Update Whammy pedal mode
    if (active_program.field.ctrl_mode == WHAMMY_CTRL_MODE_MOMENTARY) {
        send_program_change(WHAMMY_MODE_OFF);
    }
    else {
        send_program_change(active_program.field.pedal_mode);
    }
}

void handle_midi_program_change(midi_value_t program)
{
    usb_printf(PSTR("Entering program #%u" USB_NEWLINE), program+1);
    enter_program(program);
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
    enter_program(0);
    init_wave(&control_wave, active_program.field.waveform,
              active_program.field.speed, active_program.field.amplitude, 0);
}

void save_current_program(void)
{
    usb_puts(PSTR("Storing current setup"));
    update_program(active_program.word);
}

void select_next_mode(void)
{
    switch(active_program.field.ctrl_mode) {
        case WHAMMY_CTRL_MODE_BYPASS:
            enter_momentary_mode();
            break;

        case WHAMMY_CTRL_MODE_MOMENTARY:
            enter_wave_mode(WAVE_SINE);
            break;

        case WHAMMY_CTRL_MODE_WAVE:
            ++active_program.field.waveform;
            if (active_program.field.waveform > WAVE_RANDOM) {
                enter_pattern_mode(SEQUENCER_PATTERN_01);
                break;
            }
            usb_printf(PSTR("Waveform %d" USB_NEWLINE), active_program.field.waveform);
            break;

        case WHAMMY_CTRL_MODE_PATTERN:
            adjust_sequencer_pattern(&sequencer, 1);
            active_program.field.waveform = sequencer.pattern;
            if (sequencer.pattern != SEQUENCER_PATTERN_01) {
                usb_printf(PSTR("Pattern %d" USB_NEWLINE), sequencer.pattern+1);
                break;
            }
            // else fall through

        default:
            enter_bypass_mode();
            break;
    }
}

void select_previous_mode(void)
{
    switch(active_program.field.ctrl_mode) {
        case WHAMMY_CTRL_MODE_BYPASS:
            enter_pattern_mode(SEQUENCER_PATTERN_20);
            break;

        case WHAMMY_CTRL_MODE_MOMENTARY:
            enter_bypass_mode();
            break;

        case WHAMMY_CTRL_MODE_WAVE:
            --active_program.field.waveform;
            if (active_program.field.waveform == WAVE_OFF) {
                enter_momentary_mode();
                break;
            }
            usb_printf(PSTR("Waveform %d" USB_NEWLINE), active_program.field.waveform);
            break;

        case WHAMMY_CTRL_MODE_PATTERN:
            adjust_sequencer_pattern(&sequencer, -1);
            if (sequencer.pattern == SEQUENCER_PATTERN_20) {
                enter_wave_mode(WAVE_RANDOM);
                break;
            }
            usb_printf(PSTR("Pattern %d" USB_NEWLINE), sequencer.pattern+1);
            break;

        default:
            enter_bypass_mode();
            break;
    }
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
