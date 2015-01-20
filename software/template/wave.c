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

#include "wave.h"


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
            if (++state->step_counter >= settings->max_value) {
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
            return settings->max_value - state->step_counter / 2;

        case DIRECTION_UP:
            return state->step_counter / 2;

        default:
            return 0;
    }
}

static uint8_t compute_saw_down_wave(void)
{
    return settings->max_value - compute_ramp();
}

static uint8_t compute_saw_up_wave(void)
{
    return compute_ramp();
}

static uint8_t compute_square_wave(void)
{
    // Return quantized counter
    if (state->step_counter >= settings->max_value/2) {
        return settings->max_value;
    }
    else {
        return 0;
    }
}

static uint8_t compute_stairs_wave(void)
{
    uint8_t step_size = settings->max_value / STAIR_WAVE_STEPS;

    // Reinitialize step counter at edge values
    uint8_t *counter = &(state->step_counter);
    if (*counter == 0 && state->step_direction == DIRECTION_DOWN) {
        *counter = step_size;
    }
    if (*counter == settings->max_value && state->step_direction == DIRECTION_UP) {
        *counter = settings->max_value - step_size;
    }

    // Return quantized counter
    return (*counter / step_size) * step_size;
}

static uint8_t compute_triangle_wave(void)
{
    return state->step_counter;
}

void initialize_wave(struct wave *wave, uint8_t max_value, enum waveform waveform, uint8_t speed)
{
    wave->settings.max_value = max_value;
    set_speed(wave, speed);
    set_waveform(wave, waveform);
}

void set_speed(struct wave *wave, uint8_t speed)
{
    wave->settings.speed = speed;
    wave->state.speed_prescaler = (wave->settings.max_value - speed) / 4;
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
    switch (settings->waveform) {
        case WAVE_SAW_DOWN:
            return compute_saw_down_wave();

        case WAVE_SAW_UP:
            return compute_saw_up_wave();

        case WAVE_SQUARE:
            return compute_square_wave();

        case WAVE_STAIRS:
            return compute_stairs_wave();

        case WAVE_TRIANGLE:
            return compute_triangle_wave();

        default:
            break;
    }
    return 0;
}
