/// \file
/// \brief      MIDI switcher module API

/*
 * Copyright 2016 Simon Gansen
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


//---------------- data types ----------------//


//---------------- functions and procedures ----------------//
/// \brief      Handler for the `load` command
bool exec_load(const char* command);

/// \brief      Handler for the `relays` command
bool exec_relay(const char* command);

/// \brief      Handler for the `save` command
bool exec_save(const char* command);

/// \brief      Replace current program by given program_data and update relays accordingly
void execute_program(uint16_t program_data);

///< \brief     Callback for program changes
void handle_program_change(uint8_t program);


//---------------- EOF ----------------//
#endif // _APP_H

