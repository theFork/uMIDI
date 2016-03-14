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

#include "gpio.h"
#include "leds.h"
#include "midi.h"
#include "wave.h"
#include "usb.h"
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

static uint8_t                  controller_number;
static struct wave              wave;



///////////////////////////////////////////////////////////////////
// S T A T I C   F U N C T I O N S   A N D   P R O C E D U R E S //
///////////////////////////////////////////////////////////////////

/// \brief      Computes a wave according to the specified pattern
/// \param      wave
///                 the wave
/// \return     the wave output
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

/// \brief      Constructs and prints out a nice string announcing the selected waveform
/// \param      waveform
///                 the newly selected waveform
static void echo_selected_waveform(enum waveform waveform)
{
    // Initialize string buffer
    char waveform_string[16] = "";

    switch (waveform) {
    case WAVE_SINE:
        snprintf(waveform_string, sizeof(waveform_string), "sine");
        break;
    case WAVE_TRIANGLE:
        snprintf(waveform_string, sizeof(waveform_string), "triangle");
        break;
    case WAVE_SAW_UP:
        snprintf(waveform_string, sizeof(waveform_string), "saw up");
        break;
    case WAVE_SAW_DOWN:
        snprintf(waveform_string, sizeof(waveform_string), "saw down");
        break;
    case WAVE_SQUARE:
        snprintf(waveform_string, sizeof(waveform_string), "square");
        break;
    case WAVE_STAIRS:
        snprintf(waveform_string, sizeof(waveform_string), "stairs");
        break;
    case WAVE_RANDOM:
        snprintf(waveform_string, sizeof(waveform_string), "random");
        break;
    default:
        snprintf(waveform_string, sizeof(waveform_string), "pattern %u", waveform-SEQUENCER_PATTERN_01+1);
    }

    usb_printf("Switching to waveform: %s" USB_NEWLINE, waveform_string);
}



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void select_next_waveform(void)
{
    enum waveform waveform = wave.settings.waveform;
    if (waveform == SEQUENCER_PATTERN_20) {
        waveform = WAVE_SINE;
    }
    else {
        ++waveform;
    }
    echo_selected_waveform(waveform);
    set_waveform(&wave, waveform);
}

void select_previous_waveform(void)
{
    enum waveform waveform = wave.settings.waveform;
    if (waveform == WAVE_SINE) {
        waveform = SEQUENCER_PATTERN_20;
    }
    else {
        --waveform;
    }
    echo_selected_waveform(waveform);
    set_waveform(&wave, waveform);
}

void init_sequencer_module(struct sequencer_config* config, const struct gpio_pin* leds[], uint8_t leds_size)
{
    controller_number = config->controller_number;
    init_wave(&wave, config->waveform, config->speed, MIDI_MAX_VALUE, 0);
    configure_tap_tempo_wave(&wave);
}

void start_or_stop_sequencer(void)
{
    // If stopped, just set the "running?" flag
    if (!running) {
        running = true;
        return;
    }

    // Otherwise, reset the sequencer
    running = false;
    step_counter = 0;
    show_bar_graph(step_counter);
    reset_wave(&wave);
}

void increase_speed(void)
{
    // Add one BPM to the current wave speed as long as we are below 300 BPM
    fixed_t bpm = wave.settings.frequency * 60;
    if (bpm <= fixed_from_int(300)) {
        bpm += fixed_from_int(1);
        set_frequency(&wave, bpm/60);
        usb_printf("Setting wave speed to %u BPM" USB_NEWLINE, fixed_to_int(bpm));
    }
}

void decrease_speed(void)
{
    // Subtract one BPM from the current wave speed as long as we are above 15 BPM
    fixed_t bpm = wave.settings.frequency * 60;
    if (bpm >= fixed_from_int(15)) {
        bpm -= fixed_from_int(1);
        set_frequency(&wave, bpm/60);
        usb_printf("Setting wave speed to %u BPM" USB_NEWLINE, fixed_to_int(bpm));
    }
}

void update_sequencer(void)
{
    static uint8_t old_value = 0;
    uint8_t new_value = update_wave(&wave);
    if (new_value != old_value) {
        old_value = new_value;
        send_control_change(controller_number, new_value);
    }
}
