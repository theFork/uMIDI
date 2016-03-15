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
#include <stdint.h>

#include "gpio.h"
#include "wave.h"


//---------------- constants ----------------//


//---------------- data types ----------------//

/// \brief      Enumeration of available sequencer patterns
enum sequencer_pattern
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
};


//---------------- functions and procedures ----------------//
void init_sequencer_module(void);

void toggle_sequencer(void);

void update_sequencer(void);

//---------------- EOF ----------------//
#endif // _SEQUENCER_H

