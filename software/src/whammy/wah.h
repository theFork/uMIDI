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
/// \brief      The PWM output used to enable the device
#define WAH_ENABLE_PIN                  gpio.header3.pin2

/// \brief      The PWM output used to disable the device
#define WAH_DISABLE_PIN                 gpio.header3.pin3

/// \brief      PWM output pin for Wah LED
#define WAH_PWM_PIN                     gpio.header3.pin8

/// \brief      PWM for Wah LED
#define WAH_PWM                         PWM_PIN8

/// \brief      The MIDI controller number used for Wah frequency
#define WAH_MIDI_CONTROLLER             8

/// \brief      The MIDI note number used to en-/disable the Wah
#define WAH_MIDI_NOTE                   42



//---------------- data types ----------------//


//---------------- functions and procedures ----------------//
void enable_wah(bool enable);
void set_wah_frequency(midi_value_t value);
void init_wah_module(void);


//---------------- EOF ----------------//
#endif // _WAH_H

