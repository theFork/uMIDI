/// \file
/// \brief  Waveform function generator

/*
 * Copyright 2012-2015 Sebastian Neuser
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

#include "midi.h"
#include "wave.h"
#include "lookup_tables.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

/// \brief      Internal array of amplitudes for the WAVE_PATTERN_nn waveforms
static uint8_t wave_patterns[16][16] = {
    { // WAVE_PATTERN_01
        WHAMMY_AMPLITUDE_UNISON,
        WHAMMY_AMPLITUDE_MINOR_SECOND,
        WHAMMY_AMPLITUDE_MAJOR_SECOND,
        WHAMMY_AMPLITUDE_MINOR_THIRD,
        WHAMMY_AMPLITUDE_MAJOR_THIRD,
        WHAMMY_AMPLITUDE_PERFECT_FOURTH,
        WHAMMY_AMPLITUDE_TRITONE,
        WHAMMY_AMPLITUDE_PERFECT_FIFTH,
        WHAMMY_AMPLITUDE_MINOR_SIXTH,
        WHAMMY_AMPLITUDE_MAJOR_SIXTH,
        WHAMMY_AMPLITUDE_MINOR_SEVENTH,
        WHAMMY_AMPLITUDE_MAJOR_SEVENTH,
        WHAMMY_AMPLITUDE_OCTAVE,
        WHAMMY_AMPLITUDE_OCTAVE,
        WHAMMY_AMPLITUDE_OCTAVE,
        WHAMMY_AMPLITUDE_OCTAVE,
    },
    { // WAVE_PATTERN_02
        WHAMMY_AMPLITUDE_UNISON,
        WHAMMY_AMPLITUDE_MINOR_THIRD,
        WHAMMY_AMPLITUDE_PERFECT_FOURTH,
        WHAMMY_AMPLITUDE_TRITONE,
        WHAMMY_AMPLITUDE_PERFECT_FIFTH,
        WHAMMY_AMPLITUDE_MINOR_SEVENTH,
        WHAMMY_AMPLITUDE_OCTAVE,
        WHAMMY_AMPLITUDE_MINOR_SEVENTH,
        WHAMMY_AMPLITUDE_OCTAVE,
        WHAMMY_AMPLITUDE_MINOR_SEVENTH,
        WHAMMY_AMPLITUDE_PERFECT_FIFTH,
        WHAMMY_AMPLITUDE_TRITONE,
        WHAMMY_AMPLITUDE_PERFECT_FOURTH,
        WHAMMY_AMPLITUDE_MINOR_THIRD,
        WHAMMY_AMPLITUDE_UNISON,
        WHAMMY_AMPLITUDE_MINOR_THIRD,
    },
    { // WAVE_PATTERN_03
        WHAMMY_AMPLITUDE_UNISON,
        WHAMMY_AMPLITUDE_1ST_MINOR_THIRD,
        WHAMMY_AMPLITUDE_1ST_PERFECT_FIFTH,
        WHAMMY_AMPLITUDE_1ST_MINOR_SEVENTH,
        WHAMMY_AMPLITUDE_1ST_OCTAVE,
        WHAMMY_AMPLITUDE_2ND_MINOR_THIRD,
        WHAMMY_AMPLITUDE_2ND_PERFECT_FIFTH,
        WHAMMY_AMPLITUDE_2ND_MINOR_SEVENTH,
        WHAMMY_AMPLITUDE_2ND_OCTAVE,
        WHAMMY_AMPLITUDE_2ND_MINOR_SEVENTH,
        WHAMMY_AMPLITUDE_2ND_PERFECT_FIFTH,
        WHAMMY_AMPLITUDE_2ND_MINOR_THIRD,
        WHAMMY_AMPLITUDE_1ST_OCTAVE,
        WHAMMY_AMPLITUDE_1ST_MINOR_SEVENTH,
        WHAMMY_AMPLITUDE_1ST_PERFECT_FIFTH,
        WHAMMY_AMPLITUDE_1ST_MINOR_THIRD,
    },
};



/////////////////////////////////////////////////////////////////////
//  S T A T I C   F U N C T I O N S   A N D   P R O C E D U R E S  //
/////////////////////////////////////////////////////////////////////

/// \brief      Increments or decrements the step counter depending on its the current direction
/// \details    When the top or bottom value is reached, the direction is switched automatically.
///             The step counter consequently follows a triangle wave.
/// \param      wave
///                 the wave
static void advance_step_counter(struct wave* wave)
{
    switch (wave->state.step_direction) {
        case DIRECTION_DOWN:
            // Decrement counter; switch direction at min value
            if (--wave->state.step_counter <= 0) {
                wave->state.step_direction = DIRECTION_UP;
            }
            break;

        case DIRECTION_UP:
            // Increment counter; switch direction at max value
            if (++wave->state.step_counter >= MIDI_MAX_VALUE) {
                wave->state.step_direction = DIRECTION_DOWN;
            }
            break;

        default:
            break;
    }
}

/// \brief      Derives a rising ramp from the step counter
/// \param      wave
///                 the wave
/// \return     the wave output
static midi_value_t compute_ramp(struct wave* wave)
{
    switch (wave->state.step_direction) {
        case DIRECTION_DOWN:
            return MIDI_MAX_VALUE - wave->state.step_counter / 2;

        case DIRECTION_UP:
            return wave->state.step_counter / 2;

        default:
            return 0;
    }
}

/// \brief      Computes a random wave
/// \details    Note that this uses static variables, so only one random wave can be computed!
/// \param      wave
///                 the wave
/// \return     the wave output
static midi_value_t compute_random_wave(struct wave* wave)
{
    static midi_value_t current_value = 0;

    // Check if step counter has changed
    static midi_value_t old_step_counter = 0;
    if (wave->state.step_counter == old_step_counter) {
        return current_value;
    }
    old_step_counter = wave->state.step_counter;

    // Adjust output randomly but not beyond MIDI value range
    if (rand() % 2 && current_value <= MIDI_MAX_VALUE - RANDOM_WAVE_STEP_SIZE) {
        current_value += RANDOM_WAVE_STEP_SIZE;
    }
    else if (current_value >= RANDOM_WAVE_STEP_SIZE) {
        current_value -= RANDOM_WAVE_STEP_SIZE;
    }
    return current_value;
}

/// \brief      Computes a saw down wave
/// \param      wave
///                 the wave
/// \return     the wave output
static midi_value_t compute_saw_down_wave(struct wave* wave)
{
    return MIDI_MAX_VALUE - compute_ramp(wave);
}

/// \brief      Computes a saw up wave
/// \param      wave
///                 the wave
/// \return     the wave output
static midi_value_t compute_saw_up_wave(struct wave* wave)
{
    return compute_ramp(wave);
}

/// \brief      Computes a sine wave through a lookup table
/// \param      wave
///                 the wave
/// \return     the wave output
static midi_value_t compute_sine_wave(struct wave* wave)
{
    switch (wave->state.step_direction) {
        case DIRECTION_DOWN:
            return MIDI_MAX_VALUE - lookup_sine(wave->state.step_counter);

        case DIRECTION_UP:
            return lookup_sine(wave->state.step_counter);

        default:
            return 0;
    }
}

/// \brief      Computes a square wave
/// \param      wave
///                 the wave
/// \return     the wave output
static midi_value_t compute_square_wave(struct wave* wave)
{
    return (wave->state.step_direction == DIRECTION_UP) ? MIDI_MAX_VALUE : 0;
}

/// \brief      Computes a square wave
/// \param      wave
///                 the wave
/// \return     the wave output
static midi_value_t compute_stairs_wave(struct wave* wave)
{
    uint8_t step_size = MIDI_MAX_VALUE / STAIR_WAVE_STEPS;

    // Reinitialize step counter at edge values
    uint8_t *counter = &(wave->state.step_counter);
    if (*counter == 0) {
        *counter = step_size;
    }
    else if (*counter > step_size * STAIR_WAVE_STEPS) {
        *counter = step_size * STAIR_WAVE_STEPS - step_size;
        wave->state.step_direction = DIRECTION_DOWN;
    }

    // Return quantized counter
    return (*counter / step_size) * step_size;
}

/// \brief      Computes a triangle wave
/// \param      wave
///                 the wave
/// \return     the wave output
static midi_value_t compute_triangle_wave(struct wave* wave)
{
    return wave->state.step_counter;
}

/// \brief      Computes a wave according to the specified pattern
/// \param      wave
///                 the wave
/// \return     the wave output
static midi_value_t compute_wave_pattern(struct wave* wave)
{
    // Compute sample coordinates
    uint8_t pattern_number = wave->settings.waveform - WAVE_PATTERN_01;
    uint8_t quantization = MIDI_MAX_VALUE / 16;
    uint8_t sample_index = compute_ramp(wave) / quantization;

    // Reset step counter after last sample
    if (sample_index >= 16) {
        wave->state.step_counter = 0;
    }
    return wave_patterns[pattern_number][sample_index];
}



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void init_wave(struct wave* wave, enum waveform waveform, midi_value_t speed, midi_value_t amplitude, midi_value_t offset)
{
    wave->settings.amplitude = amplitude;
    wave->settings.offset = offset;
    set_speed(wave, speed);
    set_waveform(wave, waveform);
}

void set_speed(struct wave *wave, midi_value_t speed)
{
    wave->settings.speed = speed;
    wave->state.speed_prescaler = (MIDI_MAX_VALUE - speed) / 4;
}

void set_waveform(struct wave *wave, enum waveform waveform)
{
    wave->settings.waveform = waveform;

    // Reset state
    wave->state.speed_counter = 0;
    wave->state.step_counter = 0;
    wave->state.step_direction = DIRECTION_UP;
}

midi_value_t update_wave(struct wave *wave)
{
    // Increment speed counter and reset it if the prescaler was reached
    ++wave->state.speed_counter;
    if (wave->state.speed_counter >= wave->state.speed_prescaler) {
        wave->state.speed_counter = 0;

        // Advance step counter
        advance_step_counter(wave);
    }

    // Compute and return wave value
    uint16_t output;
    switch (wave->settings.waveform) {
        case WAVE_OFF:
            return 0;

        case WAVE_RANDOM:
            output = compute_random_wave(wave);
            break;

        case WAVE_SAW_DOWN:
            output = compute_saw_down_wave(wave);
            break;

        case WAVE_SAW_UP:
            output = compute_saw_up_wave(wave);
            break;

        case WAVE_SINE:
            output = compute_sine_wave(wave);
            break;

        case WAVE_SQUARE:
            output = compute_square_wave(wave);
            break;

        case WAVE_STAIRS:
            output = compute_stairs_wave(wave);
            break;

        case WAVE_TRIANGLE:
            output = compute_triangle_wave(wave);
            break;

        default:
            output = compute_wave_pattern(wave);
    }

    // Amplify and add offset
    output *= wave->settings.amplitude;
    output /= MIDI_MAX_VALUE;
    output += wave->settings.offset;
    return output;
}
