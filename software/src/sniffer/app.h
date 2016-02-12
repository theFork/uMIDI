/// \file
/// \brief      MIDI sniffer module API

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


//---------------- constants ----------------//


//---------------- data types ----------------//


//---------------- functions and procedures ----------------//

///< \brief     Implementation of the `chan` command
bool exec_channel(const char* command);

///< \brief     Callback for control change messges
void handle_control_change(uint8_t controller, uint8_t value);

///< \brief     Callback for note off messges
void handle_note_off(uint8_t note);

///< \brief     Callback for not on messages
void handle_note_on(uint8_t note);

///< \brief     Callback for program changes
void handle_program_change(uint8_t program);


//---------------- EOF ----------------//
#endif // _APP_H

