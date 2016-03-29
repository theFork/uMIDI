/// \file
/// \brief      Sequencer module API

/*
 * Copyright 2015 Sebastian Neuser
 *
 * This file is part of the uMIDI firmware.
 *
 * The MIDI volume controller firmware is free software: you can redistribute it and/or modify
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

#ifndef _SEQUENCER_H
#define _SEQUENCER_H


//---------------- includes ----------------//
#include <stdbool.h>
#include <stdint.h>

#include "gpio.h"
#include "math.h"
#include "midi.h"
#include "wave.h"


//---------------- constants ----------------//

/// \brief      Number of steps in a sequencer pattern
#define SEQUENCER_STEPS_PER_PATTERN     16


//---------------- data types ----------------//

/// \brief      Enumeration of available sequencer patterns
enum sequencer_pattern_number
{
    SEQUENCER_PATTERN_01,   ///< Sequencer pattern 01
    SEQUENCER_PATTERN_02,   ///< Sequencer pattern 02
    SEQUENCER_PATTERN_03,   ///< Sequencer pattern 03
    SEQUENCER_PATTERN_04,   ///< Sequencer pattern 04
    SEQUENCER_PATTERN_05,   ///< Sequencer pattern 05
    SEQUENCER_PATTERN_06,   ///< Sequencer pattern 06
    SEQUENCER_PATTERN_07,   ///< Sequencer pattern 07
    SEQUENCER_PATTERN_08,   ///< Sequencer pattern 08
    SEQUENCER_PATTERN_09,   ///< Sequencer pattern 09
    SEQUENCER_PATTERN_10,   ///< Sequencer pattern 10
    SEQUENCER_PATTERN_11,   ///< Sequencer pattern 11
    SEQUENCER_PATTERN_12,   ///< Sequencer pattern 12
    SEQUENCER_PATTERN_13,   ///< Sequencer pattern 13
    SEQUENCER_PATTERN_14,   ///< Sequencer pattern 14
    SEQUENCER_PATTERN_15,   ///< Sequencer pattern 15
    SEQUENCER_PATTERN_16,   ///< Sequencer pattern 16
    SEQUENCER_PATTERN_17,   ///< Sequencer pattern 17
    SEQUENCER_PATTERN_18,   ///< Sequencer pattern 18
    SEQUENCER_PATTERN_19,   ///< Sequencer pattern 19
    SEQUENCER_PATTERN_20,   ///< Sequencer pattern 20
    SEQUENCER_PATTERNS      ///< Number of available sequencer patterns
};

/// \brief      Enumeration of available sequencer channels
enum sequencer_channel_number
{
    SEQUENCER_CHANNEL_1,    ///< Sequencer channel 1
    SEQUENCER_CHANNEL_2,    ///< Sequencer channel 2
    SEQUENCER_CHANNEL_3,    ///< Sequencer channel 3
    SEQUENCER_CHANNEL_4,    ///< Sequencer channel 4
    SEQUENCER_CHANNELS      ///< Number of available sequencer channels
};

/// \brief      Type
enum sequencer_channel_mode
{
    SEQUENCER_CHANNEL_MODE_ONE_SHOT,    ///< The pattern is played only once on start of the sequencer channel
    SEQUENCER_CHANNEL_MODE_CONTINUOUS   ///< The pattern is looped as long as the sequencer channel is active
};

/// \brief      A sequencer step
/// \details    Exactly specifies the MIDI message that is sent when the step in the pattern is
///             reached.
struct sequencer_step
{
    enum midi_channel       channel;    ///< MIDI channel
    enum midi_message_type  type;       ///< MIDI message type
    midi_value_t            data0;      ///< Data byte 0
    midi_value_t            data1;      ///< Data byte 1 (optional, depending on the message type)
};

/// \brief      A sequencer pattern
/// \details    Contains a list of sequencer steps and the length of that list.
struct sequencer_pattern
{
    uint8_t                 length;                             ///< Number of sequencer steps
    struct sequencer_step   steps[SEQUENCER_STEPS_PER_PATTERN]; ///< Sequencer steps
};

/// \brief      State and configuration of a sequencer channel
/// \details    A channel has three configuration parameters that have to be specified:
///             * pattern
///             * speed
///             * channel mode
///             Other members of this struct are internal state and should not be manipulated.
struct sequencer_channel
{
    enum sequencer_pattern_number   pattern;        ///< Stores the sequencer channel's pattern index
    uint8_t                         speed;          ///< Stores the sequencer channel's speed
    enum sequencer_channel_mode     mode;           ///< Channel mode (one-shot or continuous)
    bool                            running;        ///< This flag indicates if the sequencer channel is currently running
    uint8_t                         step_index;     ///< This counter points to the next sequencer step in the channel's pattern
    struct wave                     wave;           ///< An internal waveform used as a clock source for the sequencer channel
    bool                            clock_state;    ///< Last known state of the channel's clock
    void (*tick_callback)(void);                   ///< An event handler for sequencer clock ticks or `NULL`
};


//---------------- functions and procedures ----------------//
/// \brief      Increments or decrements a sequencer channel's pattern index
/// \details    Rotates the pattern index, for example: If the resulting pattern index is greater
///             than the number of patterns, counting restarts at the first pattern.
/// \param      channel
///                 the sequencer channel to adjust
/// \param      difference
///                 an offset from the currently stored pattern
/// \returns    the set pattern
enum sequencer_pattern_number adjust_sequencer_pattern(struct sequencer_channel* channel, int8_t difference);

/// \brief      Increments or decrements a sequencer channel's speed
/// \details    Cyclically rotates the speed in the range of a MIDI value [0 .. 127].
/// \param      channel
///                 the sequencer channel to adjust
/// \param      difference
///                 an offset from the current speed
/// \returns    the newly set speed
midi_value_t adjust_sequencer_speed(struct sequencer_channel* channel, int8_t difference);

/// \brief      Configures a sequencer channel
/// \details    This function also registers the channel's wave with the tap tempo function.
/// \param      number
///                 the sequencer channel slot to use
/// \param      channel
///                 the sequencer channel
/// \see        sequencer_channel
/// \see        register_tap
void configure_sequencer_channel(enum sequencer_channel_number number, struct sequencer_channel* channel);

/// \brief      Copies a sequencer pattern
/// \details    Duplicates the pattern at the source index and overwrites the pattern at the
///             destination index with it.
/// \param      source
///                 number of the pattern to copy
/// \param      destination
///                 number of the pattern to overwrite
void copy_pattern(enum sequencer_pattern_number source, enum sequencer_pattern_number destination);

/// \brief      Dumps a sequencer pattern
/// \details    Reads the data from EEPROM and converts it to a string of hexadecimal digits.
/// \param      number
///                 number of the pattern to export
/// \returns    the pattern stored in EEPROM as a hex-string
char* export_pattern(enum sequencer_pattern_number number);

/// \brief      Initializes / Restores "factory-default" patterns
/// \details    Copies a list of patterns to the start of the EEPROM pattern storage.
/// \param      factory_patterns
///                 a pointer to a list of "factory"-default patterns
/// \param      number_of_patterns
///                 number of "factory" patterns
void init_sequencer_patterns(const struct sequencer_pattern* factory_patterns, uint8_t number_of_patterns);

/// \brief      Stores a given pattern at the specified location
/// \details    Overwrites stored data!
/// \param      number
///                 number of the pattern to overwrite
/// \param      pattern
///                 the new pattern to store
void overwrite_pattern(enum sequencer_pattern_number number, const struct sequencer_pattern* pattern);

/// \brief      Stores a pattern given as a hex-string at the specified location
/// \details    Overwrites stored data!
/// \param      number
///                 number of the pattern to overwrite
/// \param      data
///                 the new pattern to store as a string of hexadecimal digits
void import_pattern(enum sequencer_pattern_number number, const char* data);

/// \brief      Selects a sequencer channel's pattern
/// \param      channel
///                 the sequencer channel to adjust
/// \param      pattern
///                 the index of the new sequencer pattern
void set_sequencer_pattern(struct sequencer_channel* channel, enum sequencer_pattern_number pattern);

/// \brief      Sets a sequencer channel's speed
/// \param      channel
///                 the sequencer channel to adjust
/// \param      speed
///                 the new speed
void set_sequencer_speed(struct sequencer_channel* channel, midi_value_t speed);

/// \brief      Starts a sequencer channel
/// \details    If the channel is already running, it is restarted.
/// \param      channel
///                 the sequencer channel to (re-)start
void start_sequencer(struct sequencer_channel* channel);

/// \brief      Stops a sequencer channel
/// \param      channel
///                 the sequencer channel to stop
void stop_sequencer(struct sequencer_channel* channel);

/// \brief      Starts or stops a sequencer channel
/// \param      channel
///                 the sequencer channel to toggle
/// \returns    `true` if the channel was activated; `false` if it was deactivated
bool toggle_sequencer(struct sequencer_channel* channel);

/// \brief      Main background task for the sequencer module
/// \details    This task plays active (-> started) sequencer channels. It uses the wave module as
///             an internal clock source, so it must be registered as a fast background task.
/// \see        update_wave
void update_sequencer(void);

/// \brief      Wipes a sequencer pattern
/// \details    Zeroes out memory at the given sequencer pattern index.
/// \param      number
///                 number of the pattern to wipe
void wipe_pattern(enum sequencer_pattern_number number);

//---------------- EOF ----------------//
#endif // _SEQUENCER_H

