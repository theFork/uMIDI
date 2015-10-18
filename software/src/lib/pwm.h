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


//---------------- data types ----------------//


//---------------- functions and procedures ----------------//

/// \brief      Updates the PWM duty cycle
/// \details    Looks up the exponential value corresponding to the provided duty cycle and writes
///             the PWM counter compare register. This procedure can be used to manually control
///             the modulation.
/// \param      duty
///                 the desired duty cycle
/// \see        exp_table
void apply_duty_cycle(uint8_t duty);

/// \brief      Initializes the PWM
/// \details    CPU timer 1 is clocked with the system clock (32 MHz) and the compare output A
///             is set up to output a dual slope PWM. Additionally, a sine wave is set up, so that
///             update_pwm can be used to generate a periodic output signal.
/// \see        update_pwm
/// \see        wave
void initialize_pwm_module(void);

/// \brief      Updates the pwm duty cycle
/// \details    To enable wave output, this procedure must be called cyclically as a state machine
///             task.
/// \see        initialize_state_machine
void update_pwm(void);


//---------------- EOF ----------------//
#endif // _PWM_H
