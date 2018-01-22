/// \file
/// \brief      Wah-wah module API

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

#ifndef _WAH_H
#define _WAH_H


//---------------- includes ----------------//
#include <stdint.h>
#include "lib/pwm.h"


//---------------- constants ----------------//
/// \brief      The MIDI note that represents uMIDI wah's enable / bypass state
#define MIDI_NOTE_ENABLE_WAH            42

/// \brief      PWM output used for the Wah LED
#define WAH_PWM                         PWM_PIN8

/// \brief      Maximum PWM duty cycle for the Wah LED
#define WAH_PWM_MAX_DUTY                (PWM_MAX_DUTY-250)

/// \brief      Minimum PWM duty cycle for the Wah LED
#define WAH_PWM_MIN_DUTY                600

/// \brief      Minimum PWM duty cycle for the Wah LED
#define WAH_PWM_RANGE                   (WAH_PWM_MAX_DUTY - WAH_PWM_MIN_DUTY)


//---------------- data types ----------------//


//---------------- functions and procedures ----------------//
void enable_wah(bool enable);
void init_wah_module(void);
bool is_wah_enabled(void);
void toggle_wah(void);


//---------------- EOF ----------------//
#endif // _WAH_H

