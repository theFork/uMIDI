/// \file
/// \brief      An application specific module
/// \details    Place your application specific code here.

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

#ifndef _APP_H
#define _APP_H


//---------------- includes ----------------//


//---------------- constants ----------------//

/// \brief      The PWM output used to enable the device
#define ENABLE_PIN                      gpio.header3.pin2


//---------------- data types ----------------//


//---------------- functions and procedures ----------------//
void enable_wah(bool enable);
bool exec_enable(const char* command);
bool exec_led(const char* command);



//---------------- EOF ----------------//
#endif // _APP_H

