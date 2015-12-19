/// \file
/// \brief  PWM configuration and service functions

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

#ifndef _PWM_H
#define _PWM_H


//---------------- includes ----------------//
#include <stdint.h>

#include "lookup_tables.h"


//---------------- data types ----------------//


//---------------- constants ----------------//
/// \brief      Maximum duty cycle of the PWM.
#define PWM_MAX_DUTY    ((1L << lookup_table_resolution) - 1)


//---------------- functions and procedures ----------------//

/// \brief      Initializes the PWM
/// \details    CPU timer 1 is clocked with the system clock (32 MHz) and the compare output A
///             is set up to output a dual slope PWM. Additionally, a linear function is set up,
///             which is used in set_pwm_duty_cycle() to compute the duty cycle compare values.
/// \param      conversion_function
///                 Points to a function converting midi values to pwm compare register values
/// \see        update_pwm
/// \see        wave
void initialize_pwm_module(uint16_t (*conversion_function)(uint8_t));


/// \brief      Updates the PWM duty cycle
/// \details    Computes a suitable duty cycle compare value and writes the PWM counter compare
///             register. This procedure can be used to manually control the modulation.
/// \param      duty
///                 the desired duty cycle [0..MIDI_MAX_VALUE]
void set_pwm_duty_cycle(uint8_t duty);


//---------------- EOF ----------------//
#endif // _PWM_H
