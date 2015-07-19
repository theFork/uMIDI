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

/*
 * Header for the device configuration.
 * @author Sebastian Neuser
*/

#ifndef _CONFIG_H
#define _CONFIG_H


//---------------- includes ----------------//
#include <stdint.h>


//---------------- constant declaration ----------------//
extern const struct adc_conversion_config expression_conversion;
extern const struct adc_config adc_config;


//---------------- EOF ----------------//
#endif // _CONFIG_H

