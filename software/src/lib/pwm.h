/// \file
/// \brief  PWM configuration and service functions

/*
 * Copyright 2012-2018 Sebastian Neuser
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
///\brief   Available PWM outputs
enum pwm {
    PWM_PIN2,   ///\brief PWM at GPIO header 3 pin 2
    PWM_PIN3,   ///\brief PWM at GPIO header 3 pin 3
    PWM_PIN6,   ///\brief PWM at GPIO header 3 pin 6
    PWM_PIN7,   ///\brief PWM at GPIO header 3 pin 7
    PWM_PIN8,   ///\brief PWM at GPIO header 3 pin 8
    PWM_PIN9,   ///\brief PWM at GPIO header 3 pin 9
};


//---------------- constants ----------------//
/// \brief      Maximum duty cycle of the PWM.
#define PWM_MAX_DUTY    ((1L << lookup_table_resolution) - 1)


//---------------- functions and procedures ----------------//

/// \brief      Initializes the PWM
/// \details    The CPU timer is clocked with the system clock (32 MHz) and the compare output
///             is set up to output a dual slope PWM. Additionally, a conversion function which is
///             used in set_pwm_duty_cycle() to compute the duty cycle compare values is registered.
/// \param      pwm
///                 the PWM to initialize
/// \param      conversion_function
///                 pointer to a function converting midi values to pwm compare register values
/// \see        wave
void init_pwm(enum pwm pwm, uint16_t (*conversion_function)(midi_value_t value));


/// \brief      Updates the PWM duty cycle
/// \details    Computes a suitable duty cycle compare value and writes the PWM counter compare
///             register. This procedure can be used to manually control the modulation.
/// \param      pwm
///                 the PWM to modify
/// \param      duty
///                 the desired duty cycle [0..MIDI_MAX_VALUE]
void set_pwm_duty_cycle(enum pwm pwm, midi_value_t duty);


//---------------- EOF ----------------//
#endif // _PWM_H
