/// \file
/// \brief      Whammy controller API

/*
 * Copyright 2015, 2016 Sebastian Neuser
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

#ifndef _WHAMMY_H
#define _WHAMMY_H


//---------------- includes ----------------//
#include <stdbool.h>
#include <stdint.h>
#include "lib/gpio.h"
#include "lib/wave.h"
#include "lib/sequencer.h"
#include "user_interface.h"


//---------------- constants ----------------//

/// \brief      The MIDI controller number used by the Whammy pedal to modify the pitch
#define WHAMMY_MIDI_CC_NUMBER   11


//---------------- data types ----------------//

/// \brief      Available modes of the Whammy controller
enum whammy_ctrl_mode
{
    WHAMMY_CTRL_MODE_OFF,       ///< The controller does not send CC messages
    WHAMMY_CTRL_MODE_DETUNE,    ///< The controller selects a program and a static pitch
    WHAMMY_CTRL_MODE_NORMAL,    ///< The controller passes on CC messages it receives
    WHAMMY_CTRL_MODE_LIMIT,     ///< The controller scales and passes on CC messages it receives
    WHAMMY_CTRL_MODE_MOMENTARY, ///< The controller sends CC messages that achieve a momentary pitch
                                ///< bend on reception of note on / off messages.
    WHAMMY_CTRL_MODE_PATTERN,   ///< The controller sends CC messages following a sequencer pattern
    WHAMMY_CTRL_MODE_RANDOM,    ///< The controller sends CC messages following a waveform
};

/// \brief      Available modes of the Whammy pedal
/// \details    These values are sent to the pedal in MIDI PC messages.
enum whammy_mode
{
    WHAMMY_MODE_OFF                     =  32,  ///< The Whammy pedal is inactive
    WHAMMY_MODE_CLASSIC_2OCT_UP         =   0,  ///< Classic mode, 2 octaves up
    WHAMMY_MODE_CLASSIC_1OCT_UP         =   1,  ///< Classic mode, 1 octave up
    WHAMMY_MODE_CLASSIC_5TH_UP          =   2,  ///< Classic mode, 5th up
    WHAMMY_MODE_CLASSIC_4TH_UP          =   3,  ///< Classic mode, 4th up
    WHAMMY_MODE_CLASSIC_2ND_UP          =   4,  ///< Classic mode, 2nd up
    WHAMMY_MODE_CLASSIC_2ND_DOWN        =   5,  ///< Classic mode, 2nd down
    WHAMMY_MODE_CLASSIC_4TH_DOWN        =   6,  ///< Classic mode, 4th down
    WHAMMY_MODE_CLASSIC_5TH_DOWN        =   7,  ///< Classic mode, 5th down
    WHAMMY_MODE_CLASSIC_1OCT_DOWN       =   8,  ///< Classic mode, 1 octave down
    WHAMMY_MODE_CLASSIC_DIVE_BOMB       =   9,  ///< Classic mode, dive bomb
    WHAMMY_MODE_CLASSIC_CHORUS_DEEP     =  10,  ///< Classic mode, deep chorus
    WHAMMY_MODE_CLASSIC_CHORUS_SHALLOW  =  11,  ///< Classic mode, shallow chorus
    WHAMMY_MODE_CLASSIC_4TH_3RD         =  12,  ///< Classic mode, 4th down / 3rd up
    WHAMMY_MODE_CLASSIC_4TH_5TH         =  13,  ///< Classic mode, 4th down / 5th up
    WHAMMY_MODE_CLASSIC_5TH_5TH         =  14,  ///< Classic mode, 5th down / 5th up
    WHAMMY_MODE_CLASSIC_5TH_6TH         =  15,  ///< Classic mode, 5th up / 6th up
    WHAMMY_MODE_CLASSIC_5TH_OCT         =  16,  ///< Classic mode, 5th up / octave up
    WHAMMY_MODE_CLASSIC_OCT_4TH         =  17,  ///< Classic mode, octave down / 4th down
    WHAMMY_MODE_CLASSIC_OCT_OCT         =  18,  ///< Classic mode, octave down / octave up
    WHAMMY_MODE_CLASSIC_OCT_10TH        =  19,  ///< Classic mode, octave up / 10th up
    WHAMMY_MODE_CLASSIC_OCT_2OCT        =  20,  ///< Classic mode, 1 octave up / 2 octaves up
    WHAMMY_MODE_CHORDS_2OCT_UP          =  42,  ///< Chords mode, 2 octaves up
    WHAMMY_MODE_CHORDS_1OCT_UP          =  43,  ///< Chords mode, 1 octave up
    WHAMMY_MODE_CHORDS_5TH_UP           =  44,  ///< Chords mode, 5th up
    WHAMMY_MODE_CHORDS_4TH_UP           =  45,  ///< Chords mode, 4th up
    WHAMMY_MODE_CHORDS_2ND_UP           =  46,  ///< Chords mode, 2nd up
    WHAMMY_MODE_CHORDS_2ND_DOWN         =  47,  ///< Chords mode, 2nd down
    WHAMMY_MODE_CHORDS_4TH_DOWN         =  48,  ///< Chords mode, 4th down
    WHAMMY_MODE_CHORDS_5TH_DOWN         =  49,  ///< Chords mode, 5th down
    WHAMMY_MODE_CHORDS_1OCT_DOWN        =  50,  ///< Chords mode, 1 octave down
    WHAMMY_MODE_CHORDS_DIVE_BOMB        =  51,  ///< Chords mode, dive bomb
    WHAMMY_MODE_CHORDS_CHORUS_DEEP      =  52,  ///< Chords mode, deep chorus
    WHAMMY_MODE_CHORDS_CHORUS_SHALLOW   =  53,  ///< Chords mode, shallow chorus
    WHAMMY_MODE_CHORDS_4TH_3RD          =  54,  ///< Chords mode, 4th down / 3rd up
    WHAMMY_MODE_CHORDS_4TH_5TH          =  55,  ///< Chords mode, 4th down / 5th up
    WHAMMY_MODE_CHORDS_5TH_5TH          =  56,  ///< Chords mode, 5th down / 5th up
    WHAMMY_MODE_CHORDS_5TH_6TH          =  57,  ///< Chords mode, 5th up / 6th up
    WHAMMY_MODE_CHORDS_5TH_OCT          =  58,  ///< Chords mode, 5th up / octave up
    WHAMMY_MODE_CHORDS_OCT_4TH          =  59,  ///< Chords mode, octave down / 4th down
    WHAMMY_MODE_CHORDS_OCT_OCT          =  60,  ///< Chords mode, octave down / octave up
    WHAMMY_MODE_CHORDS_OCT_10TH         =  61,  ///< Chords mode, octave up / 10th up
    WHAMMY_MODE_CHORDS_OCT_2OCT         =  62,  ///< Chords mode, 1 octave up / 2 octaves up
};

/// \brief      Amplitudes that represent certain intervals on a DigiTech Whammy pedal
enum whammy_note
{
    WHAMMY_NOTE_UNISON              = 0,    ///< Unison, valid in every mode
    WHAMMY_NOTE_MINOR_SECOND        = 13,   ///< Minor second in 1oct mode
    WHAMMY_NOTE_MAJOR_SECOND        = 24,   ///< Major second in 1oct mode
    WHAMMY_NOTE_MINOR_THIRD         = 32,   ///< Minor third in 1oct mode
    WHAMMY_NOTE_MAJOR_THIRD         = 42,   ///< Major third in 1oct mode
    WHAMMY_NOTE_PERFECT_FOURTH      = 52,   ///< Perfect fourth in 1oct mode
    WHAMMY_NOTE_TRITONE             = 63,   ///< Tritone in 1oct mode
    WHAMMY_NOTE_PERFECT_FIFTH       = 74,   ///< Perfect fifth in 1oct mode
    WHAMMY_NOTE_MINOR_SIXTH         = 84,   ///< Minor sixth in 1oct mode
    WHAMMY_NOTE_MAJOR_SIXTH         = 95,   ///< Major sixth in 1oct mode
    WHAMMY_NOTE_MINOR_SEVENTH       = 106,  ///< Minor seventh in 1oct mode
    WHAMMY_NOTE_MAJOR_SEVENTH       = 116,  ///< Major seventh in 1oct mode
    WHAMMY_NOTE_OCTAVE              = 127,  ///< Octave in 1oct mode
    WHAMMY_NOTE_1ST_MINOR_SECOND    = 5,    ///< First minor second in 2oct mode
    WHAMMY_NOTE_1ST_MAJOR_SECOND    = 10,   ///< First major second in 2oct mode
    WHAMMY_NOTE_1ST_MINOR_THIRD     = 16,   ///< First minor third in 2oct mode
    WHAMMY_NOTE_1ST_MAJOR_THIRD     = 21,   ///< First major third in 2oct mode
    WHAMMY_NOTE_1ST_PERFECT_FOURTH  = 26,   ///< First perfect fourth in 2oct mode
    WHAMMY_NOTE_1ST_TRITONE         = 32,   ///< First tritone in 2oct mode
    WHAMMY_NOTE_1ST_PERFECT_FIFTH   = 37,   ///< First perfect fifth in 2oct mode
    WHAMMY_NOTE_1ST_MINOR_SIXTH     = 42,   ///< First minor sixth in 2oct mode
    WHAMMY_NOTE_1ST_MAJOR_SIXTH     = 48,   ///< First major sixth in 2oct mode
    WHAMMY_NOTE_1ST_MINOR_SEVENTH   = 53,   ///< First minor seventh in 2oct mode
    WHAMMY_NOTE_1ST_MAJOR_SEVENTH   = 58,   ///< First major seventh in 2oct mode
    WHAMMY_NOTE_1ST_OCTAVE          = 63,   ///< First octave in 2oct mode
    WHAMMY_NOTE_2ND_MINOR_SECOND    = 69,   ///< Second minor second in 2oct mode
    WHAMMY_NOTE_2ND_MAJOR_SECOND    = 74,   ///< Second major second in 2oct mode
    WHAMMY_NOTE_2ND_MINOR_THIRD     = 79,   ///< Second minor third in 2oct mode
    WHAMMY_NOTE_2ND_MAJOR_THIRD     = 85,   ///< Second major third in 2oct mode
    WHAMMY_NOTE_2ND_PERFECT_FOURTH  = 90,   ///< Second perfect fourth in 2oct mode
    WHAMMY_NOTE_2ND_TRITONE         = 95,   ///< Second tritone in 2oct mode
    WHAMMY_NOTE_2ND_PERFECT_FIFTH   = 100,  ///< Second perfect fifth in 2oct mode
    WHAMMY_NOTE_2ND_MINOR_SIXTH     = 106,  ///< Second minor sixth in 2oct mode
    WHAMMY_NOTE_2ND_MAJOR_SIXTH     = 111,  ///< Second major sixth in 2oct mode
    WHAMMY_NOTE_2ND_MINOR_SEVENTH   = 117,  ///< Second minor seventh in 2oct mode
    WHAMMY_NOTE_2ND_MAJOR_SEVENTH   = 122,  ///< Second major seventh in 2oct mode
    WHAMMY_NOTE_2ND_OCTAVE          = 127,  ///< Second octave in 2oct mode
};

/// \brief      Data structure overlay for program dwords
union whammy_ctrl_program {
    uint32_t word;                                  ///< The bare 32-bit dword stored in EEPROM
    struct {
        enum whammy_ctrl_mode ctrl_mode     : 3;    ///< Whammy controller mode
        enum whammy_mode      pedal_mode    : 7;    ///< Whammy pedal mode
        uint8_t               speed         : 8;    ///< Speed of the waveform / sequencer / bend
        uint8_t               range         : 7;    ///< Wave amplitude / bend interval / limit,
                                                    ///  depending on #ctrl_mode
        uint8_t               pattern       : 5;    ///< Waveform or pattern, depending on #ctrl_mode
    } field;                                        ///< A struct with data fields contained in the dword
};


//---------------- functions and procedures ----------------//
uint8_t adjust_speed(int8_t delta);
enum whammy_mode adjust_whammy_mode(int8_t delta);
void copy_whammy_ctrl_pattern(enum sequencer_pattern_number destination);
void dump_current_pattern(void);
void dump_current_program(void);
void enter_bypass_mode(void);
void enter_detune_mode(void);
void enter_limit_mode(void);
void enter_momentary_mode(void);
void enter_normal_mode(void);
void enter_pattern_mode(const enum sequencer_pattern_number pattern);
void enter_random_mode(void);
union whammy_ctrl_program get_active_program(void);
uint8_t get_current_amplitude(void);
enum ui_ctrl_mode get_current_ctrl_mode(void);
enum sequencer_pattern_number get_current_pattern_number(void);
uint8_t get_current_speed(void);
enum whammy_mode get_current_whammy_mode(void);
void init_whammy_controller(void);
void reset_whammy_patterns(void);
void save_current_program(void);
enum ui_ctrl_mode select_next_ctrl_mode(void);
enum ui_ctrl_mode select_previous_ctrl_mode(void);
void set_whammy_ctrl_pattern_length(uint8_t length);
void set_whammy_ctrl_pattern_step(uint8_t step_index, const struct sequencer_step* step);
midi_value_t set_whammy_ctrl_range(int8_t range, bool adjust);
void set_whammy_ctrl_speed(uint8_t speed);
void set_whammy_mode(enum whammy_mode mode);
void update_controller_value(void);
void wipe_whammy_ctrl_pattern(void);

//---------------- EOF ----------------//
#endif // _WHAMMY_H

