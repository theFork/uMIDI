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

#ifndef _WAVE_H
#define _WAVE_H


//---------------- includes ----------------//
#include <stdint.h>

#include "math.h"
#include "midi.h"


//---------------- code macros ----------------//

/// \brief      Step size of the random wave
/// \details    The output will change in a random direction by this amount
#define     RANDOM_WAVE_STEP_SIZE           4

/// \brief      Number of levels in the stair wave
#define     STAIR_WAVE_STEPS                5

/// \brief      Number of different output levels of waves
#define     WAVE_STEPS                      100

/// \brief      Tap tempo buffer size
/// \details    The tempo is computed as an average over this many tapped tempo values.
#define     TAP_TEMPO_BUFFER_SIZE           3

/// \brief      Tap tempo background task frequency
/// \see        F_TASK_SLOW
#define     TAP_TEMPO_TASK_FREQUENCY        F_TASK_SLOW


//---------------- data types ----------------//

/// \brief      Step counter direction
enum direction
{
    DIRECTION_DOWN,                             ///< The counter is counting down
    DIRECTION_UP,                               ///< The counter is counting up
};

/// \brief      Enumeration of available waveforms
enum waveform
{
    WAVE_OFF,                                   ///< No waveform
    WAVE_SINE,                                  ///< Sine wave
    WAVE_TRIANGLE,                              ///< Triangle wave
    WAVE_SAW_UP,                                ///< Saw up wave
    WAVE_SAW_DOWN,                              ///< Saw down wave
    WAVE_SQUARE,                                ///< Square wave
    WAVE_STAIRS,                                ///< Stair wave
    WAVE_RANDOM,                                ///< Random wave
};

/// \brief      Wave configuration
/// \see        waveform
/// \see        fixed_t
struct wave_settings
{
    enum waveform   waveform;                   ///< Waveform
    fixed_t         frequency;                  ///< Speed of the wave in [Hz]
    midi_value_t    amplitude;                  ///< Amplitude [0, 127]
    midi_value_t    offset;                     ///< Static offset [0, 127]
};

/// \brief      Internal state of a wave
/// \see        direction
struct wave_state
{
    uint16_t        speed_prescaler;            ///< Prescaler value for the speed counter
    uint16_t        speed_counter;              ///< Current speed counter value
    uint8_t         step_counter;               ///< Current step counter value
    enum direction  step_direction;             ///< Current step counter direction
};

/// \brief      The settings and internal state of a wave
/// \see        wave_settings
/// \see        wave_state
struct wave
{
    struct wave_settings    settings;           ///< Settings
    struct wave_state       state;              ///< Internal state
};


//---------------- functions and procedures ----------------//

/// \brief      Configures a wave for tap tempo functionality
/// \param      wave
///                 the wave to configure
/// \see        tap_tempo_task
void configure_tap_tempo_wave(struct wave * const wave);

/// \brief      Initializes a wave
/// \param      wave
///                 the wave to initialize
/// \param      waveform
///                 the initial waveform
/// \param      speed
///                 the initial speed
/// \param      amplitude
///                 the amplitude
/// \param      offset
///                 the static offset
/// \see        wave
/// \see        waveform
void init_wave(struct wave* wave, enum waveform waveform, midi_value_t speed,
               midi_value_t amplitude, midi_value_t offset);

/// \brief      Registers a tempo tap event
/// \details    This function must be called whenever a tempo tap event occurrs. It sets an
///             internal flag to inform the background task about the event.
/// \see        tap_tempo_task
void register_tap(void);

/// \brief      Resets a wave
/// \details    Re-initializes all internal counters and states of the supplied wave
/// \param      wave
///                 the wave to reset
void reset_wave(struct wave * const wave);

/// \brief      Updates the frequency of a wave
/// \param      wave
///                 the wave to update
/// \param      frequency
///                 the new frequency
/// \see        wave
void set_frequency(struct wave* wave, fixed_t frequency);

/// \brief      Updates the speed of a wave
/// \details    Sets the wave frequency to the given speed in [BPM], adding an offset of 15 BPM.
/// \param      wave
///                 the wave to update
/// \param      speed
///                 the new speed in [BPM]
/// \see        wave
void set_speed(struct wave* wave, midi_value_t speed);

/// \brief      Updates the waveform of a wave
/// \param      wave
///                 the wave to update
/// \param      waveform
///                 the new waveform
void set_waveform(struct wave* wave, enum waveform waveform);

/// \brief      Background task for the tap tempo function
/// \details    This function must be registered as a slow background task. It computes a frequency
///             from an incrementing internal counter and the task frequency and updates the speed
///             of the wave configured for tap tempo operation.
/// \see        configure_tap_tempo_wave
/// \see        register_tap
void tap_tempo_task(void);

/// \brief      Computes the current wave output value
/// \details    This function must be registered as a fast background task. Over time the output
///             value follows the configured waveform.
/// \param      wave
///                 the wave whose output should be computed
/// \return     the current output value
midi_value_t update_wave(struct wave* wave);


//---------------- EOF ----------------//
#endif // _WAVE_H
