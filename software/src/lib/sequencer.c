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

#include <stdbool.h>
#include <stdlib.h>
#include <avr/pgmspace.h>

#include "gpio.h"
#include "math.h"
#include "midi.h"
#include "sequencer.h"
#include "wave.h"


////////////////////////////////////////////////////////////////
//               P R I V A T E   D E F I N E S                //
////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

const struct sequencer_pattern patterns[SEQUENCER_PATTERNS] PROGMEM;

/// \brief      Internal array of sequencer channels
static struct sequencer_channel* channels[SEQUENCER_CHANNELS] = {0,};



///////////////////////////////////////////////////////////////////
// S T A T I C   F U N C T I O N S   A N D   P R O C E D U R E S //
///////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

enum sequencer_pattern_number adjust_sequencer_pattern(struct sequencer_channel * const channel, const int8_t difference)
{
    channel->pattern += SEQUENCER_PATTERNS;
    channel->pattern += difference;
    channel->pattern %= SEQUENCER_PATTERNS;
    return channel->pattern;
}

midi_value_t adjust_sequencer_speed(struct sequencer_channel * const channel, const int8_t difference)
{
    channel->speed += MIDI_MAX_VALUE+1;
    channel->speed += difference;
    channel->speed %= MIDI_MAX_VALUE+1;
    set_speed(&channel->wave, channel->speed);
    return channel->speed;
}

void configure_sequencer_channel(const enum sequencer_channel_number number, struct sequencer_channel * const channel)
{
    // Store pointer to the channel configuration / state
    channels[number] = channel;

    // Use square wave as clock source
    init_wave(&channel->wave, WAVE_SQUARE, 0, 1, 0);
    set_speed(&channel->wave, channel->speed);
    configure_tap_tempo_wave(&channel->wave);
}

void init_sequencer_module(void)
{
    // TODO: Remove?
}

void set_sequencer_pattern(struct sequencer_channel * const channel, const enum sequencer_pattern_number pattern)
{
    channel->pattern = pattern;
}

void set_sequencer_speed(struct sequencer_channel * const channel, const midi_value_t speed)
{
    channel->speed = speed;
}

void start_sequencer(struct sequencer_channel * const channel)
{
    channel->step_index = 0;
    reset_wave(&channel->wave);
    channel->running = true;
}

void stop_sequencer(struct sequencer_channel * const channel)
{
    channel->running = false;
}

bool toggle_sequencer(struct sequencer_channel * const channel)
{
    if (channel->running) {
        stop_sequencer(channel);
        return false;
    }
    else {
        start_sequencer(channel);
        return true;
    }
}

void update_sequencer(void)
{
    for (uint8_t i=0; i<SEQUENCER_CHANNELS; ++i) {
        struct sequencer_channel* const channel = channels[i];

        // Abort if the channel is not active
        if (!channel->running) {
            return;
        }

        // See if the sequencer clock ticked and update it
        bool current_clock_state = update_wave(&channel->wave);
        if (current_clock_state == channel->clock_state) {
            return;
        }
        channel->clock_state = current_clock_state;

        // Skip low->high transition
        if (current_clock_state) {
            return;
        }

        // Read MIDI message from program space and send it
        const struct sequencer_step * const step = &patterns[channel->pattern].steps[channel->step_index];
        send_midi_message(pgm_read_byte(&step->channel), pgm_read_byte(&step->type),
                          pgm_read_byte(&step->data0), pgm_read_byte(&step->data1));

        // Call the clock tick handler if it is set
        if (channel->tick_callback != NULL) {
            channel->tick_callback();
        }

        // Cyclically implement step index
        ++channel->step_index;
        channel->step_index %= pgm_read_byte(&patterns[channel->pattern].length);

        // Disable the channel if the end of a one-shot pattern was reached
        if (channel->mode == SEQUENCER_CHANNEL_MODE_ONE_SHOT && channel->step_index == 0) {
            stop_sequencer(channel);
        }
    }
}
