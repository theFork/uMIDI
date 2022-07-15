/// \file
/// \brief      API for the main user interface of the whammy controller.

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

#ifndef _USER_INTERFACE_H
#define _USER_INTERFACE_H


//---------------- includes ----------------//
#include <stdint.h>
#include "lib/gpio.h"
#include "lib/wave.h"


//---------------- constants ----------------//

/// \brief  The GPIO pin connected to the store led
#define STORE_LED_PIN       gpio.header3.pin6

/// \brief  The GPIO pin connected to the tempo led
#define TEMPO_LED_PIN       gpio.header3.pin4


//---------------- data types ----------------//

enum ui_ctrl_mode
{
    UI_CTRL_MODE_OFF,
    UI_CTRL_MODE_DETUNE,
    UI_CTRL_MODE_NORMAL,
    UI_CTRL_MODE_LIMIT,
    UI_CTRL_MODE_MOMENTARY,
    UI_CTRL_MODE_RANDOM,
    UI_CTRL_MODE_PATTERN_01,
    UI_CTRL_MODE_PATTERN_02,
    UI_CTRL_MODE_PATTERN_03,
    UI_CTRL_MODE_PATTERN_04,
    UI_CTRL_MODE_PATTERN_05,
    UI_CTRL_MODE_PATTERN_06,
    UI_CTRL_MODE_PATTERN_07,
    UI_CTRL_MODE_PATTERN_08,
    UI_CTRL_MODE_PATTERN_09,
    UI_CTRL_MODE_PATTERN_10,
    UI_CTRL_MODE_PATTERN_11,
    UI_CTRL_MODE_PATTERN_12,
    UI_CTRL_MODE_PATTERN_13,
    UI_CTRL_MODE_PATTERN_14,
    UI_CTRL_MODE_PATTERN_15,
    UI_CTRL_MODE_PATTERN_16,
    UI_CTRL_MODE_PATTERN_17,
    UI_CTRL_MODE_PATTERN_18,
    UI_CTRL_MODE_PATTERN_19,
    UI_CTRL_MODE_PATTERN_20,
};


//---------------- functions and procedures ----------------//
void clear_value_display(void);
void signal_midi_rx(void);
void signal_usb_rx(void);
void signal_store(void);
void store_setup(void);
void toggle_hmi_layer(void);
void update_displays(void);
void value1_decrement(void);
void value1_increment(void);
void value2_decrement(void);
void value2_increment(void);
void visualize_sequencer(uint8_t value);
void visualize_wave(uint8_t value);

//---------------- EOF ----------------//
#endif // _USER_INTERFACE_H

