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
#include <avr/pgmspace.h>

#include "gpio.h"
#include "math.h"
#include "midi.h"
#include "wave.h"
#include "sequencer.h"
#include "whammy.h"


////////////////////////////////////////////////////////////////
//               P R I V A T E   D E F I N E S                //
////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

/// \brief      Internal array of sequencer patterns
/// \details    Predefines some "factory"-programmed patterns.
static const struct sequencer_pattern patterns[20] PROGMEM = {
    { // SEQUENCER_PATTERN_01
        .frequency  = 1L<<15,
        .length     = 8,
        .steps      = {
            {
                .channel = MIDI_CHANNEL_01,
                .type = MIDI_MSG_TYPE_CONTROL_CHANGE,
                .data0 = 11,
                .data1 = WHAMMY_NOTE_UNISON,
            },
            {
                .channel = MIDI_CHANNEL_01,
                .type = MIDI_MSG_TYPE_CONTROL_CHANGE,
                .data0 = 11,
                .data1 = WHAMMY_NOTE_OCTAVE,
            },
            {
                .channel = MIDI_CHANNEL_01,
                .type = MIDI_MSG_TYPE_CONTROL_CHANGE,
                .data0 = 11,
                .data1 = WHAMMY_NOTE_UNISON,
            },
            {
                .channel = MIDI_CHANNEL_01,
                .type = MIDI_MSG_TYPE_CONTROL_CHANGE,
                .data0 = 11,
                .data1 = WHAMMY_NOTE_OCTAVE,
            },
            {
                .channel = MIDI_CHANNEL_01,
                .type = MIDI_MSG_TYPE_CONTROL_CHANGE,
                .data0 = 11,
                .data1 = WHAMMY_NOTE_UNISON,
            },
            {
                .channel = MIDI_CHANNEL_01,
                .type = MIDI_MSG_TYPE_CONTROL_CHANGE,
                .data0 = 11,
                .data1 = WHAMMY_NOTE_OCTAVE,
            },
            {
                .channel = MIDI_CHANNEL_01,
                .type = MIDI_MSG_TYPE_CONTROL_CHANGE,
                .data0 = 11,
                .data1 = WHAMMY_NOTE_UNISON,
            },
            {
                .channel = MIDI_CHANNEL_01,
                .type = MIDI_MSG_TYPE_CONTROL_CHANGE,
                .data0 = 11,
                .data1 = WHAMMY_NOTE_OCTAVE,
            },
        }
    },

    { // SEQUENCER_PATTERN_02
        .frequency  = 1L<<16,
        .length     = 8,
        .steps      = {
            {
                .channel = MIDI_CHANNEL_01,
                .type = MIDI_MSG_TYPE_CONTROL_CHANGE,
                .data0 = 11,
                .data1 = WHAMMY_NOTE_UNISON,
            },
            {
                .channel = MIDI_CHANNEL_01,
                .type = MIDI_MSG_TYPE_CONTROL_CHANGE,
                .data0 = 11,
                .data1 = WHAMMY_NOTE_1ST_PERFECT_FIFTH,
            },
            {
                .channel = MIDI_CHANNEL_01,
                .type = MIDI_MSG_TYPE_CONTROL_CHANGE,
                .data0 = 11,
                .data1 = WHAMMY_NOTE_1ST_OCTAVE,
            },
            {
                .channel = MIDI_CHANNEL_01,
                .type = MIDI_MSG_TYPE_CONTROL_CHANGE,
                .data0 = 11,
                .data1 = WHAMMY_NOTE_2ND_PERFECT_FIFTH,
            },
            {
                .channel = MIDI_CHANNEL_01,
                .type = MIDI_MSG_TYPE_CONTROL_CHANGE,
                .data0 = 11,
                .data1 = WHAMMY_NOTE_2ND_OCTAVE,
            },
            {
                .channel = MIDI_CHANNEL_01,
                .type = MIDI_MSG_TYPE_CONTROL_CHANGE,
                .data0 = 11,
                .data1 = WHAMMY_NOTE_2ND_PERFECT_FIFTH,
            },
            {
                .channel = MIDI_CHANNEL_01,
                .type = MIDI_MSG_TYPE_CONTROL_CHANGE,
                .data0 = 11,
                .data1 = WHAMMY_NOTE_1ST_OCTAVE,
            },
            {
                .channel = MIDI_CHANNEL_01,
                .type = MIDI_MSG_TYPE_CONTROL_CHANGE,
                .data0 = 11,
                .data1 = WHAMMY_NOTE_1ST_PERFECT_FIFTH,
            },
        }
    },
    { // SEQUENCER_PATTERN_03
        .frequency  = 1L<<16,
        .length     = 8,
        .steps      = {
            {
                .channel = MIDI_CHANNEL_01,
                .type = MIDI_MSG_TYPE_CONTROL_CHANGE,
                .data0 = 11,
                .data1 = WHAMMY_NOTE_UNISON,
            },
            {
                .channel = MIDI_CHANNEL_01,
                .type = MIDI_MSG_TYPE_CONTROL_CHANGE,
                .data0 = 11,
                .data1 = WHAMMY_NOTE_OCTAVE,
            },
            {
                .channel = MIDI_CHANNEL_01,
                .type = MIDI_MSG_TYPE_CONTROL_CHANGE,
                .data0 = 11,
                .data1 = WHAMMY_NOTE_OCTAVE,
            },
            {
                .channel = MIDI_CHANNEL_01,
                .type = MIDI_MSG_TYPE_CONTROL_CHANGE,
                .data0 = 11,
                .data1 = WHAMMY_NOTE_UNISON,
            },
            {
                .channel = MIDI_CHANNEL_01,
                .type = MIDI_MSG_TYPE_CONTROL_CHANGE,
                .data0 = 11,
                .data1 = WHAMMY_NOTE_OCTAVE,
            },
            {
                .channel = MIDI_CHANNEL_01,
                .type = MIDI_MSG_TYPE_CONTROL_CHANGE,
                .data0 = 11,
                .data1 = WHAMMY_NOTE_OCTAVE,
            },
            {
                .channel = MIDI_CHANNEL_01,
                .type = MIDI_MSG_TYPE_CONTROL_CHANGE,
                .data0 = 11,
                .data1 = WHAMMY_NOTE_UNISON,
            },
            {
                .channel = MIDI_CHANNEL_01,
                .type = MIDI_MSG_TYPE_CONTROL_CHANGE,
                .data0 = 11,
                .data1 = WHAMMY_NOTE_OCTAVE,
            },
        }
    },
};

/// \brief      This flag indicates if the sequencer is currently running
/// \see        toggle_sequencer
static bool             running             = false;

/// \brief      This counter points to the next sequencer step in the pattern
static uint8_t          step_index          = 0;

/// \brief      Stores the sequencer pattern index
static enum sequencer_pattern_number active_pattern = SEQUENCER_PATTERN_02;

/// \brief      An internal waveform used as a clock source for the sequencer
static struct wave      wave;



///////////////////////////////////////////////////////////////////
// S T A T I C   F U N C T I O N S   A N D   P R O C E D U R E S //
///////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

enum sequencer_pattern_number adjust_sequencer_pattern(struct sequencer_channel * const channel, const int8_t difference)
{
    // TODO: Implement
    return 0;
}

midi_value_t adjust_sequencer_speed(struct sequencer_channel * const channel, const int8_t difference)
{
    // TODO: Implement
    return 0;
}

void configure_sequencer_channel(const enum sequencer_channel_number number, struct sequencer_channel * const channel)
{
    // TODO: Implement
}

void init_sequencer_module(void)
{
    // Configure square wave for use as sequencer clock
    init_wave(&wave, WAVE_SQUARE, 0, 1, 0);
    set_frequency(&wave, pgm_read_dword(&patterns[1].frequency));
    configure_tap_tempo_wave(&wave);
}

void set_sequencer_pattern(struct sequencer_channel * const channel, const enum sequencer_pattern_number pattern)
{
    // TODO: Implement
}

void set_sequencer_speed(struct sequencer_channel * const channel, const midi_value_t speed)
{
    // TODO: Implement
}

void start_sequencer(struct sequencer_channel * const channel)
{
    // TODO: Implement
}

void stop_sequencer(struct sequencer_channel * const channel)
{
    // TODO: Implement
}

void toggle_sequencer(struct sequencer_channel * const channel)
{
    // If stopped, just set the "running?" flag
    if (!running) {
        running = true;
        return;
    }

    // Otherwise, reset the sequencer
    running = false;
    step_index = 0;
    reset_wave(&wave);
}

void update_sequencer(void)
{
    // See if the sequencer clock ticked
    static bool old_clock_state = true;
    bool new_clock_state = update_wave(&wave);
    if (new_clock_state != old_clock_state) {
        old_clock_state = new_clock_state;

        // Skip low->high transition
        if (new_clock_state) {
            return;
        }

        // Read MIDI message from program space and send it
        send_midi_message(pgm_read_byte(&patterns[active_pattern].steps[step_index].channel),
                          pgm_read_byte(&patterns[active_pattern].steps[step_index].type),
                          pgm_read_byte(&patterns[active_pattern].steps[step_index].data0),
                          pgm_read_byte(&patterns[active_pattern].steps[step_index].data1));

        // Cyclically implement step index
        ++step_index;
        step_index %= patterns[active_pattern].length;
    }
}
