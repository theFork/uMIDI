/// \file
/// \brief      Header for the application specific module
/// \details    Place your application specific code here.

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

#ifndef _APP_H
#define _APP_H


//---------------- includes ----------------//
#include <stdint.h>


//---------------- constants ----------------//

/// \brief      Device reset timout in [s]
#define RESET_TIMEOUT           2

/// \brief      Size of the command buffer of the shell
/// \details    Must be int the range of `uint8_t`.
#define CMD_BUFFER_SIZE         80


//---------------- data types ----------------//


//---------------- functions and procedures ----------------//
/// \brief      TODO
/// \details    TODO
void serial_communication_task(void);


//---------------- EOF ----------------//
#endif // _APP_H

