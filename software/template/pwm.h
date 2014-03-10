/*
 * Copyright 2012-2014 Sebastian Neuser
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

/*
 * Header of the PWM module.
*/

#ifndef _PWM_H
#define _PWM_H


//---------------- includes ----------------//
#include <stdint.h>


//---------------- AVR PORT mapping ----------------//
#define     PWM_LED         gpio.portC.pin4


//---------------- code macros ----------------//
#define     PWM_LED_SCALER  20


//---------------- data types ----------------//


//---------------- functions and procedures ----------------//
void applyDutyCycle(uint8_t duty);
void configurePWM(void);


//---------------- EOF ----------------//
#endif // _PWM_H
