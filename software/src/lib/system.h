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

/// \brief      Configures the PLL for USB operation
/// \details    Sets the PLL output frequency to 48 MHz, enables the PLL and waits until it has
///             successfully locked.
void enable_usb_pll(void);

/// \brief      Stops program execution and blinks the on-board LEDs.
/// \details    Disables interrupts and watchdog and traps the program in an endless loop.
/// \param      delay_red_ms
///                 blink interval of the red LED in [ms]
/// \param      delay_green_ms
///                 blink interval of the red LED in [ms]
void panic(uint16_t delay_red_ms, uint16_t delay_green_ms);

/// \brief      Re-enables the watchdog
void wdt_reenable(void);


//---------------- EOF ----------------//
#endif // _SYSTEM_H
