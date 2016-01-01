/// \file
/// \brief      Expression pedal API

/*
 * Copyright 2015 Sebastian Neuser
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

#ifndef _EXPRESSION_H
#define _EXPRESSION_H


//---------------- includes ----------------//
#include <stdint.h>


//---------------- constants ----------------//


//---------------- data types ----------------//


//---------------- functions and procedures ----------------//

/// \brief      Updates the stored expression value
/// \details    If the value changed, the new value is sent as a MIDI control change message for
///             controller #69 (Waldorf Blofeld's F1 Cutoff).
void update_expression_value(uint16_t new_value);

/// \brief      Triggers an ADC conversion of the expression pedal input
void trigger_expression_conversion(void);


//---------------- EOF ----------------//
#endif // _EXPRESSION_H

