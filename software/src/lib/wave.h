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

/// \brief      Amplitudes that represent certain intervals on a DigiTech Whammy pedal
enum whammy_amplitude
{
    WHAMMY_AMPLITUDE_UNISON             = 0,    ///< Unison, valid in every mode
    WHAMMY_AMPLITUDE_MINOR_SECOND       = 13,   ///< Minor second in 1oct mode
    WHAMMY_AMPLITUDE_MAJOR_SECOND       = 24,   ///< Major second in 1oct mode
    WHAMMY_AMPLITUDE_MINOR_THIRD        = 32,   ///< Minor third in 1oct mode
    WHAMMY_AMPLITUDE_MAJOR_THIRD        = 42,   ///< Major third in 1oct mode
    WHAMMY_AMPLITUDE_PERFECT_FOURTH     = 52,   ///< Perfect fourth in 1oct mode
    WHAMMY_AMPLITUDE_TRITONE            = 63,   ///< Tritone in 1oct mode
    WHAMMY_AMPLITUDE_PERFECT_FIFTH      = 74,   ///< Perfect fifth in 1oct mode
    WHAMMY_AMPLITUDE_MINOR_SIXTH        = 84,   ///< Minor sixth in 1oct mode
    WHAMMY_AMPLITUDE_MAJOR_SIXTH        = 95,   ///< Major sixth in 1oct mode
    WHAMMY_AMPLITUDE_MINOR_SEVENTH      = 106,  ///< Minor seventh in 1oct mode
    WHAMMY_AMPLITUDE_MAJOR_SEVENTH      = 116,  ///< Major seventh in 1oct mode
    WHAMMY_AMPLITUDE_OCTAVE             = 127,  ///< Octave in 1oct mode
    WHAMMY_AMPLITUDE_1ST_MINOR_SECOND   = 5,    ///< First minor second in 2oct mode
    WHAMMY_AMPLITUDE_1ST_MAJOR_SECOND   = 10,   ///< First major second in 2oct mode
    WHAMMY_AMPLITUDE_1ST_MINOR_THIRD    = 16,   ///< First minor third in 2oct mode
    WHAMMY_AMPLITUDE_1ST_MAJOR_THIRD    = 21,   ///< First major third in 2oct mode
    WHAMMY_AMPLITUDE_1ST_PERFECT_FOURTH = 26,   ///< First perfect fourth in 2oct mode
    WHAMMY_AMPLITUDE_1ST_TRITONE        = 32,   ///< First tritone in 2oct mode
    WHAMMY_AMPLITUDE_1ST_PERFECT_FIFTH  = 37,   ///< First perfect fifth in 2oct mode
    WHAMMY_AMPLITUDE_1ST_MINOR_SIXTH    = 42,   ///< First minor sixth in 2oct mode
    WHAMMY_AMPLITUDE_1ST_MAJOR_SIXTH    = 48,   ///< First major sixth in 2oct mode
    WHAMMY_AMPLITUDE_1ST_MINOR_SEVENTH  = 53,   ///< First minor seventh in 2oct mode
    WHAMMY_AMPLITUDE_1ST_MAJOR_SEVENTH  = 58,   ///< First major seventh in 2oct mode
    WHAMMY_AMPLITUDE_1ST_OCTAVE         = 63,   ///< First octave in 2oct mode
    WHAMMY_AMPLITUDE_2ND_MINOR_SECOND   = 69,   ///< Second minor second in 2oct mode
    WHAMMY_AMPLITUDE_2ND_MAJOR_SECOND   = 74,   ///< Second major second in 2oct mode
    WHAMMY_AMPLITUDE_2ND_MINOR_THIRD    = 79,   ///< Second minor third in 2oct mode
    WHAMMY_AMPLITUDE_2ND_MAJOR_THIRD    = 85,   ///< Second major third in 2oct mode
    WHAMMY_AMPLITUDE_2ND_PERFECT_FOURTH = 90,   ///< Second perfect fourth in 2oct mode
    WHAMMY_AMPLITUDE_2ND_TRITONE        = 95,   ///< Second tritone in 2oct mode
    WHAMMY_AMPLITUDE_2ND_PERFECT_FIFTH  = 100,  ///< Second perfect fifth in 2oct mode
    WHAMMY_AMPLITUDE_2ND_MINOR_SIXTH    = 106,  ///< Second minor sixth in 2oct mode
    WHAMMY_AMPLITUDE_2ND_MAJOR_SIXTH    = 111,  ///< Second major sixth in 2oct mode
    WHAMMY_AMPLITUDE_2ND_MINOR_SEVENTH  = 117,  ///< Second minor seventh in 2oct mode
    WHAMMY_AMPLITUDE_2ND_MAJOR_SEVENTH  = 122,  ///< Second major seventh in 2oct mode
    WHAMMY_AMPLITUDE_2ND_OCTAVE         = 127,  ///< Second octave in 2oct mode
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
    WAVE_PATTERN_01,                            ///< Wave pattern 01
    WAVE_PATTERN_02,                            ///< Wave pattern 02
    WAVE_PATTERN_03,                            ///< Wave pattern 03
    WAVE_PATTERN_04,                            ///< Wave pattern 04
    WAVE_PATTERN_05,                            ///< Wave pattern 05
    WAVE_PATTERN_06,                            ///< Wave pattern 06
    WAVE_PATTERN_07,                            ///< Wave pattern 07
    WAVE_PATTERN_08,                            ///< Wave pattern 08
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
