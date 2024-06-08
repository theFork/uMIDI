/// \file
/// \brief      Wah-wah module API

/*
 * Copyright 2022 Sebastian Neuser
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

#ifndef _WAH_H
#define _WAH_H


//---------------- includes ----------------//
#include <stdbool.h>

#include "lib/gpio.h"
#include "lib/pwm.h"


//---------------- constants ----------------//
/// \brief      The GPIO input used for the toggle switch
#define TOGGLE_PIN                      gpio.header3.pin6

/// \brief      The GPIO output used to enable the device
#define ENABLE_PIN                      gpio.header3.pin4

/// \brief      The GPIO output used for the status LED
#define STATUS_PIN                      gpio.header3.pin8

/// \brief      The PWM output used for the Wah LED
#define WAH_PWM                         PWM_PIN2



//---------------- data types ----------------//


//---------------- functions and procedures ----------------//
void enable_wah(bool enable);
void handle_midi_cc(midi_value_t controller, midi_value_t value);
void set_wah_frequency(midi_value_t value);
void init_wah_module(void);


//---------------- EOF ----------------//
#endif // _WAH_H

