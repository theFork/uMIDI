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

/*
 * Wave computations module.
*/

#include <stdlib.h>

#include "midi.h"
#include "wave.h"
#include "lookup_tables.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

static struct wave_settings *settings;
static struct wave_state    *state;



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

static void advance_step_counter(void)
{
    switch (state->step_direction) {
        case DIRECTION_DOWN:
            // Decrement counter; switch direction at min value
            if (--state->step_counter <= 0) {
                state->step_direction = DIRECTION_UP;
            }
            break;

        case DIRECTION_UP:
            // Increment counter; switch direction at max value
            if (++state->step_counter >= MIDI_MAX_VALUE) {
                state->step_direction = DIRECTION_DOWN;
            }
            break;

        default:
            break;
    }
}

static uint8_t compute_ramp(void)
{
    switch (state->step_direction) {
        case DIRECTION_DOWN:
            return MIDI_MAX_VALUE - state->step_counter / 2;

        case DIRECTION_UP:
            return state->step_counter / 2;

        default:
            return 0;
    }
}

static uint8_t compute_random_wave(void)
{
    static uint8_t current_value = 0;

    // Check if step counter has changed
    static uint8_t old_step_counter = 0;
    if (state->step_counter == old_step_counter) {
        return current_value;
    }
    old_step_counter = state->step_counter;

    // Adjust output randomly
    if (rand() % 2 && current_value <= MIDI_MAX_VALUE - RANDOM_WAVE_STEP_SIZE) {
        current_value += RANDOM_WAVE_STEP_SIZE;
    }
    else if (current_value >= RANDOM_WAVE_STEP_SIZE) {
        current_value -= RANDOM_WAVE_STEP_SIZE;
    }
    return current_value;
}

static uint8_t compute_saw_down_wave(void)
{
    return MIDI_MAX_VALUE - compute_ramp();
}

static uint8_t compute_saw_up_wave(void)
{
    return compute_ramp();
}

static uint8_t compute_sine_wave(void)
{
    switch (state->step_direction) {
        case DIRECTION_DOWN:
            return MIDI_MAX_VALUE - lookup_sine(state->step_counter);

        case DIRECTION_UP:
            return lookup_sine(state->step_counter);

        default:
            return 0;
    }
}

static uint8_t compute_square_wave(void)
{
    return (state->step_direction == DIRECTION_UP) ? MIDI_MAX_VALUE : 0;
}

static uint8_t compute_stairs_wave(void)
{
    uint8_t step_size = MIDI_MAX_VALUE / STAIR_WAVE_STEPS;

    // Reinitialize step counter at edge values
    uint8_t *counter = &(state->step_counter);
    if (*counter == 0) {
        *counter = step_size;
    }
    else if (*counter > step_size * STAIR_WAVE_STEPS) {
        *counter = step_size * STAIR_WAVE_STEPS - step_size;
        state->step_direction = DIRECTION_DOWN;
    }

    // Return quantized counter
    return (*counter / step_size) * step_size;
}

static uint8_t compute_triangle_wave(void)
{
    return state->step_counter;
}

void initialize_wave(struct wave* wave, enum waveform waveform, uint8_t speed, uint8_t amplitude, uint8_t offset)
{
    wave->settings.amplitude = amplitude;
    wave->settings.offset = offset;
    set_speed(wave, speed);
    set_waveform(wave, waveform);
}

void set_speed(struct wave *wave, uint8_t speed)
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

uint8_t update_wave(struct wave *wave)
{
    // Define shortcuts
    settings = &(wave->settings);
    state = &(wave->state);

    // Increment speed counter and reset it if the prescaler was reached
    ++state->speed_counter;
    if (state->speed_counter >= state->speed_prescaler) {
        state->speed_counter = 0;

        // Advance step counter
        advance_step_counter();
    }

    // Compute and return wave value
    uint16_t output;
    switch (settings->waveform) {
        case WAVE_RANDOM:
            output = compute_random_wave();
            break;

        case WAVE_SAW_DOWN:
            output = compute_saw_down_wave();
            break;

        case WAVE_SAW_UP:
            output = compute_saw_up_wave();
            break;

        case WAVE_SINE:
            output = compute_sine_wave();
            break;

        case WAVE_SQUARE:
            output = compute_square_wave();
            break;

        case WAVE_STAIRS:
            output = compute_stairs_wave();
            break;

        case WAVE_TRIANGLE:
            output = compute_triangle_wave();
            break;

        default:
            break;
    }

    // Amplify and add offset
    output *= settings->amplitude;
    output /= MIDI_MAX_VALUE;
    output += settings->offset;
    return output;
}
