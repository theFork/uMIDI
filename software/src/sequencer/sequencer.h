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
#include "lib/gpio.h"
#include "lib/wave.h"


//---------------- constants ----------------//


//---------------- data types ----------------//
struct sequencer_config {
    uint8_t         controller_number;
    enum waveform   waveform;
    uint8_t         speed;
};


//---------------- functions and procedures ----------------//
bool exec_speed(const char* command);
bool exec_tap(const char* command);
bool exec_pattern(const char* command);
void handle_control_change(uint8_t current_controller, uint8_t value);
void init_sequencer_module(struct sequencer_config* config, struct gpio_pin* leds[], uint8_t leds_size);
void update_sequencer(void);


//---------------- EOF ----------------//
#endif // _SEQUENCER_H

