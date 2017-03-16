/// \file
/// \brief      API for the command line interface of the whammy controller.

/*
 * Copyright 2015, 2016 Sebastian Neuser
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

#ifndef _CMDLINE_INTERFACE_H
#define _CMDLINE_INTERFACE_H


//---------------- includes ----------------//


//---------------- constants ----------------//


//---------------- data types ----------------//


//---------------- functions and procedures ----------------//
bool exec_ampl(const char* command);
bool exec_backup(const char* command);
bool exec_dump(const char* command);
bool exec_factory_reset(const char* command);
bool exec_mode(const char* command);
bool exec_patcpy(const char* command);
bool exec_patlen(const char* command);
bool exec_patmod(const char* command);
bool exec_patwipe(const char* command);
bool exec_pgm(const char* command);
bool exec_speed(const char* command);
bool exec_store(const char* command);
bool exec_tap(const char* command);
bool exec_wham(const char* command);

//---------------- EOF ----------------//
#endif // _CMDLINE_INTERFACE_H

