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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>

#include "gpio.h"
#include "math.h"
#include "midi.h"
#include "sequencer.h"
#include "system.h"
#include "wave.h"


////////////////////////////////////////////////////////////////
//               P R I V A T E   D E F I N E S                //
////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

static struct sequencer_pattern patterns[SEQUENCER_PATTERNS] EEMEM;

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

void copy_pattern(const enum sequencer_pattern_number source, const enum sequencer_pattern_number destination)
{
    // Copy pattern to temporary buffer
    struct sequencer_pattern pattern = {0,};
    pattern.length = get_pattern_length(source);
    for (uint8_t step_index=0; step_index < pattern.length; ++step_index) {
        pattern.steps[step_index] = get_pattern_step(source, step_index);
    }

    overwrite_pattern(destination, &pattern);
}

char* export_pattern(const enum sequencer_pattern_number pattern_index)
{
    // Allocate static (!) string buffer:
    // 2 characters for hexadecimal encoding of each byte of the pattern plus string termination
    static char result_string[2*sizeof(struct sequencer_pattern)+1] = "";

    // Format pattern length
    char* write_pointer = result_string;
    snprintf(write_pointer, 3, "%02x", eeprom_read_byte(&patterns[pattern_index].length));
    write_pointer += 2;

    // Format pattern steps
    for (uint8_t step_index=0; step_index < SEQUENCER_STEPS_PER_PATTERN; ++step_index) {
        snprintf(write_pointer, 9, "%02x%02x%02x%02x",
                 eeprom_read_byte((uint8_t*) &patterns[pattern_index].steps[step_index].channel),
                 eeprom_read_byte((uint8_t*) &patterns[pattern_index].steps[step_index].type),
                 eeprom_read_byte(&patterns[pattern_index].steps[step_index].data0),
                 eeprom_read_byte(&patterns[pattern_index].steps[step_index].data1));
        write_pointer += 8;
    }
    return result_string;
}

uint8_t get_pattern_length(const enum sequencer_pattern_number pattern_index)
{
    return eeprom_read_byte(&patterns[pattern_index].length);
}

struct sequencer_step get_pattern_step(const enum sequencer_pattern_number pattern_index, const uint8_t step_index)
{
    struct sequencer_step result = {0,};
    result.channel = eeprom_read_byte((uint8_t*) &patterns[pattern_index].steps[step_index].channel);
    result.type    = eeprom_read_byte((uint8_t*) &patterns[pattern_index].steps[step_index].type);
    result.data0   = eeprom_read_byte(&patterns[pattern_index].steps[step_index].data0);
    result.data1   = eeprom_read_byte(&patterns[pattern_index].steps[step_index].data1);
    return result;
}

void import_pattern(const enum sequencer_pattern_number pattern_index, const char* data)
{
    struct sequencer_pattern pattern = {0,};
    char* hex_byte = "  ";

    // Extract pattern length
    strncpy(hex_byte, data, 2);
    pattern.length = strtol(hex_byte, NULL, 16);
    data += 2;

    // Extract sequencer steps
    for (uint8_t i=0; i<pattern.length; ++i) {
        strncpy(hex_byte, data, 2);
        pattern.steps[i].channel = strtol(hex_byte, NULL, 16);
        data += 2;
        pattern.steps[i].type = strtol(hex_byte, NULL, 16);
        data += 2;
        pattern.steps[i].data0 = strtol(hex_byte, NULL, 16);
        data += 2;
        pattern.steps[i].data1 = strtol(hex_byte, NULL, 16);
        data += 2;
    }

    // Write data
    overwrite_pattern(pattern_index, &pattern);
}

void init_sequencer_patterns(const struct sequencer_pattern * const factory_patterns, const uint8_t number_of_patterns)
{
    for (uint8_t pattern_index=0; pattern_index < number_of_patterns; ++pattern_index) {
        overwrite_pattern(pattern_index, &factory_patterns[pattern_index]);
    }
}

void next_sequencer_step(struct sequencer_channel* channel)
{
    // Send MIDI message with new value
    const struct sequencer_step * const step = &patterns[channel->pattern].steps[channel->step_index];
    send_midi_message(eeprom_read_byte((uint8_t*) &step->channel), eeprom_read_byte((uint8_t*) &step->type),
                      eeprom_read_byte(&step->data0), eeprom_read_byte(&step->data1));

    // Call the clock tick handler if it is set
    call(channel->tick_callback);

    // Cyclically implement step index
    ++channel->step_index;
    channel->step_index %= eeprom_read_byte(&patterns[channel->pattern].length);

    // Disable the channel if the end of a one-shot pattern was reached
    if (channel->mode == SEQUENCER_CHANNEL_MODE_ONE_SHOT && channel->step_index == 0) {
        stop_sequencer(channel);
    }
}

void overwrite_pattern(const enum sequencer_pattern_number pattern_index, const struct sequencer_pattern * const pattern)
{
    // Store pattern length
    if (get_pattern_length(pattern_index) != pattern->length) {
        set_pattern_length(pattern_index, pattern->length);
    }

    // Store pattern steps
    for (uint8_t step_index=0; step_index < pattern->length; ++step_index) {
        const struct sequencer_step* const step_source = &pattern->steps[step_index];
        struct sequencer_step stored_step = get_pattern_step(pattern_index, step_index);
        if (stored_step.channel != step_source->channel
        ||  stored_step.type    != step_source->type
        ||  stored_step.data0   != step_source->data0
        ||  stored_step.data1   != step_source->data1) {
            set_pattern_step(pattern_index, step_index, step_source);
            wdt_reset();
        }
    }
}

void set_pattern_length(const enum sequencer_pattern_number pattern_index, const uint8_t length)
{
    eeprom_write_byte(&patterns[pattern_index].length, length);
}

void set_pattern_step(const enum sequencer_pattern_number pattern_index, const uint8_t step_index, const struct sequencer_step * const step)
{
    struct sequencer_step* const step_destination = &patterns[pattern_index].steps[step_index];
    eeprom_write_byte((uint8_t* ) &step_destination->channel, step->channel);
    eeprom_write_byte((uint8_t* ) &step_destination->type,    step->type);
    eeprom_write_byte(&step_destination->data0,               step->data0);
    eeprom_write_byte(&step_destination->data1,               step->data1);
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

        next_sequencer_step(channel);
    }
}

void wipe_pattern(const enum sequencer_pattern_number pattern_index)
{
    struct sequencer_pattern pattern = {0,};
    pattern.length = SEQUENCER_STEPS_PER_PATTERN;
    overwrite_pattern(pattern_index, &pattern);
    set_pattern_length(pattern_index, 0);
}
