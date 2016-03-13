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
#include <avr/pgmspace.h>

#include "background_tasks.h"
#include "leds.h"
#include "lookup_tables.h"
#include "midi.h"
#include "program.h"
#include "wave.h"
#include "whammy.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

/// \brief      Internal array of amplitudes for the WAVE_PATTERN_nn waveforms
static const uint8_t wave_patterns[PROGRAM_COUNT][8] PROGMEM = {
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
        WHAMMY_NOTE_MINOR_THIRD,
        WHAMMY_NOTE_PERFECT_FOURTH,
        WHAMMY_NOTE_TRITONE,
        WHAMMY_NOTE_PERFECT_FIFTH,
        WHAMMY_NOTE_MINOR_SEVENTH,
        WHAMMY_NOTE_OCTAVE,
        WHAMMY_NOTE_TRITONE,
    },
};

/// \brief      This flag indicates if a tempo tap occurred
static bool tap_arrived = false;

/// \brief      Points to the waveform whose speed can be tapped in
static struct wave* tap_tempo_wave = NULL;



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
            if (++wave->state.step_counter >= WAVE_STEPS) {
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
    uint16_t ramp_value = wave->state.step_counter / 2;
    ramp_value *= MIDI_MAX_VALUE;
    ramp_value /= WAVE_STEPS;
    switch (wave->state.step_direction) {
        case DIRECTION_DOWN:
            return MIDI_MAX_VALUE - ramp_value;

        case DIRECTION_UP:
            return ramp_value;

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
            return MIDI_MAX_VALUE - lookup_sine(wave->state.step_counter % WAVE_STEPS);

        case DIRECTION_UP:
            return lookup_sine(wave->state.step_counter % WAVE_STEPS);

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

/// \brief      Computes a stairs wave
/// \param      wave
///                 the wave
/// \return     the wave output
static midi_value_t compute_stairs_wave(struct wave* wave)
{
    uint8_t step_size = WAVE_STEPS / STAIR_WAVE_STEPS;

    // Compute quantized counter
    uint16_t stair_value = (wave->state.step_counter / step_size) * step_size;

    // Add one stair when we're counting up
    if (wave->state.step_direction == DIRECTION_UP) {
        stair_value += step_size;
    }

    // Scale to MIDI_MAX_VALUE
    stair_value *= MIDI_MAX_VALUE;
    stair_value /= WAVE_STEPS;

    return stair_value;
}

/// \brief      Computes a triangle wave
/// \param      wave
///                 the wave
/// \return     the wave output
static midi_value_t compute_triangle_wave(struct wave* wave)
{
    return ((uint16_t) wave->state.step_counter) * MIDI_MAX_VALUE / WAVE_STEPS;
}

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
    uint8_t pattern_number = wave->settings.waveform - WAVE_PATTERN_01;
    return pgm_read_byte(&(wave_patterns[pattern_number][sample_index]));
}



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void configure_tap_tempo_wave(struct wave * const wave)
{
    tap_tempo_wave = wave;
}

void init_wave(struct wave * const wave, enum waveform waveform, midi_value_t speed, midi_value_t amplitude, midi_value_t offset)
{
    wave->settings.amplitude = amplitude;
    wave->settings.offset = offset;
    set_speed(wave, speed);
    set_waveform(wave, waveform);
}

void register_tap(void)
{
    tap_arrived = true;
}

void reset_wave(struct wave * const wave)
{
    wave->state.speed_counter = 0;
    wave->state.step_counter = 0;
    wave->state.step_direction = DIRECTION_UP;
}

void set_frequency(struct wave * const wave, fixed_t frequency)
{
    wave->settings.frequency = frequency;
    const fixed_t max_frequency = fixed_from_int(F_TASK_FAST/(2*WAVE_STEPS));
    wave->state.speed_prescaler = fixed_to_int(fixed_div(max_frequency, wave->settings.frequency));
}

void set_speed(struct wave * const wave, midi_value_t speed)
{
    // Scale range from [0..127] to [15..300] bpm
    fixed_t freq = fixed_from_int(speed);
    freq /= 127;
    freq *= 285;
    freq += fixed_from_int(15);
    freq /= 60;
    set_frequency(wave, freq);
}

void set_waveform(struct wave * const wave, enum waveform waveform)
{
    wave->settings.waveform = waveform;

    // Reset state
    reset_wave(wave);
}

void tap_tempo_task(void)
{
    static uint8_t taps = 0;
    static uint8_t buffer_index = 0;

    // Increment counter
    static uint16_t counter = 0;
    ++counter;

    if (!tap_arrived) {
        if (counter < 400) {
            return;
        }

        // Reset after timeout
        set_led(LED_RED, false);
        counter = 0;
        taps = 0;
        buffer_index = 0;
        return;
    }
    tap_arrived = false;

    // Increment tap counter to buffer size
    if (taps < TAP_TEMPO_BUFFER_SIZE) {
        ++taps;
    }

    if (taps == 1) {
        set_led(LED_RED, true);
    }
    else {
        // Register tap interval with cyclic buffer
        static fixed_t tap_tempo_buffer[TAP_TEMPO_BUFFER_SIZE] = {0, };
        fixed_t tap_frequency = fixed_from_int(TAP_TEMPO_TASK_FREQUENCY) / counter;
        tap_tempo_buffer[buffer_index] = tap_frequency;
        ++buffer_index;
        buffer_index %= TAP_TEMPO_BUFFER_SIZE;

        // Compute average
        fixed_t average = 0;
        for (int i=0; i<taps; i++) {
            average += tap_tempo_buffer[i];
        }
        average /= taps;

        // Set wave frequency
        set_frequency(tap_tempo_wave, average);
    }

    // Reset counter
    counter = 0;
}

midi_value_t update_wave(struct wave* const wave)
{
    // Increment speed counter
    ++wave->state.speed_counter;

    // Change the wave output value only if the speed prescaler was reached
    if (wave->state.speed_counter < wave->state.speed_prescaler) {
        goto return_output;
    }

    // Reset speed counter
    wave->state.speed_counter = 0;

    // Advance step counter
    advance_step_counter(wave);

    // Compute new wave output value
    static uint16_t output = 0;
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

return_output:
    return output;
}
