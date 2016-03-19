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
#include "leds.h"
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

/// \brief      Internal array of amplitudes for the WAVE_PATTERN_nn waveforms
static const uint8_t wave_patterns[20][8] PROGMEM = {
    { // WAVE_PATTERN_01
        WHAMMY_NOTE_UNISON,
        WHAMMY_NOTE_OCTAVE,
        WHAMMY_NOTE_UNISON,
        WHAMMY_NOTE_OCTAVE,
        WHAMMY_NOTE_UNISON,
        WHAMMY_NOTE_OCTAVE,
        WHAMMY_NOTE_UNISON,
        WHAMMY_NOTE_OCTAVE,
    },
    { // WAVE_PATTERN_02
        WHAMMY_NOTE_UNISON,
        WHAMMY_NOTE_1ST_PERFECT_FIFTH,
        WHAMMY_NOTE_1ST_OCTAVE,
        WHAMMY_NOTE_2ND_PERFECT_FIFTH,
        WHAMMY_NOTE_2ND_OCTAVE,
        WHAMMY_NOTE_2ND_PERFECT_FIFTH,
        WHAMMY_NOTE_1ST_OCTAVE,
        WHAMMY_NOTE_1ST_PERFECT_FIFTH,
    },
    { // WAVE_PATTERN_03
        WHAMMY_NOTE_UNISON,
        WHAMMY_NOTE_OCTAVE,
        WHAMMY_NOTE_OCTAVE,
        WHAMMY_NOTE_UNISON,
        WHAMMY_NOTE_OCTAVE,
        WHAMMY_NOTE_OCTAVE,
        WHAMMY_NOTE_UNISON,
        WHAMMY_NOTE_OCTAVE,
    },
};

static bool                     running             = false;
static uint8_t                  step_counter        = 0;

static struct wave              wave;



///////////////////////////////////////////////////////////////////
// S T A T I C   F U N C T I O N S   A N D   P R O C E D U R E S //
///////////////////////////////////////////////////////////////////

/// \brief      Computes a wave according to the specified pattern
/// \param      wave
///                 the wave
/// \return     the wave output
/*
static midi_value_t compute_wave_pattern(struct wave* wave)
{
    // Compute sample coordinates
    static uint8_t sample_index = 0;
    switch (wave->state.step_counter) {
    case 0:
    case WAVE_STEPS/2:
    case WAVE_STEPS:
        ++sample_index;
        sample_index %= 8;
    }

    // Read and return sample
    uint8_t pattern_number = wave->settings.waveform - SEQUENCER_PATTERN_01;
    return pgm_read_byte(&(wave_patterns[pattern_number][sample_index]));
}
*/



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void init_sequencer_module(void)
{
    // Configure square wave for use as sequencer clock
    init_wave(&wave, WAVE_SQUARE, 0, 1, 0);
    configure_tap_tempo_wave(&wave);
}

void toggle_sequencer(void)
{
    // If stopped, just set the "running?" flag
    if (!running) {
        running = true;
        return;
    }

    // Otherwise, reset the sequencer
    running = false;
    step_counter = 0;
    reset_wave(&wave);
}

void update_sequencer(void)
{
    static bool old_clock_state = false;
    bool new_clock_state = update_wave(&wave);
    if (new_clock_state != old_clock_state) {
        old_clock_state = new_clock_state;

        // TODO: Send MIDI message
    }
}
