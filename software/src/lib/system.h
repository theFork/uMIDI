/// \file
/// \brief  System configuration procedures

/*
 * Copyright 2015 Sebastian Neuser
 *
 * This file is part of the uMIDI firmware.
 *
 * the uMIDI firmware is free software: you can redistribute it and/or modify
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

#ifndef _SYSTEM_H
#define _SYSTEM_H


//---------------- includes ----------------//


//---------------- constants ----------------//


//---------------- data types ----------------//


//---------------- functions and procedures ----------------//

/// \brief      Configures the system clock
/// \details    Activates and selects the internal 32 MHz RC oscillator as system clock source.
void configure_system_clock(void);


//---------------- EOF ----------------//
#endif // _SYSTEM_H

