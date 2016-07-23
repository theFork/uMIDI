/// \file
/// \brief      Serial communication module
/// \details    This module contains a firmware update procedure and a command interpreter that
///             can be extended by custom commands.

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

#ifndef _SERIAL_COM_H
#define _SERIAL_COM_H


//---------------- includes ----------------//
#include <stdbool.h>
#include <stdint.h>
#include <avr/pgmspace.h>


//---------------- constants ----------------//

/// \brief      Size of the command buffer of the shell
/// \details    Must be int the range of `uint8_t`.
#define CMD_BUFFER_SIZE         100

/// \brief      Number of recent commands in the history
/// \details    Must be int the range of `uint8_t`.
#define CMD_HISTORY_SIZE        4

/// \brief      ASCII escape character
#define ESCAPE_CHAR_CODE        27

/// \brief      Maximum program size in number of pages
/// \details    - usable program space 136 KiB - 8 KiB boot section
///             - one self-programming page is 256 B
///             - the last page is in use by the bootloader and can therefore not be used
#define MAX_PAGE_NUM            ((PROGMEM_SIZE - BOOT_SECTION_SIZE) / SPM_PAGESIZE / 2 - 1)

/// \brief      Device reset timout in [s]
#define RESET_TIMEOUT           2


//---------------- data types ----------------//

/// \brief      Type for command handlers
/// \see        serial_command
typedef bool (*cmd_handler_t)(const char* cmd);

/// \brief      Command specification
/// \details    *Note:* The strings must be placed in program space. Unfortunately this is a
///             two-step process: First you have to define the string in program space:
///             \code
///             static const char cmd_string[] PROGMEM = "command";
///             \endcode
///             You can then reference this string in the definition of a command struct. Please
///             look at the USB-ready application template for an examplary command definition.
struct serial_command
{
    PGM_P cmd_string;           ///< The command as typed in the console

    PGM_P help_string;          ///< Description of the command (used by the `help` command)
                                ///<
                                ///< The description may contain newline characters, which will
                                ///< be automatically followed by white space in the help message
                                ///< output to ensure nice formatting. The first line specifies
                                ///< command parameters or options, which are printed directly after
                                ///< the #cmd_string, so if your command does not take any, but
                                ///< the description does not fit in one line, you should specify an
                                ///< empty first line.

    cmd_handler_t handler;      ///< Handler to call when #cmd_string is recognized
                                ///< \param cmd
                                ///<            the full command line
                                ///< \returns `true` if command execution succeeded,
                                ///<          `false` otherwise
};


//---------------- functions and procedures ----------------//

/// \brief      Initializes the USB communication module
/// \details    Registers the specified commands.
/// \param      commands
///                 an array of command specifications
/// \param      commands_size
///                 the number of command specifications in \p commands
/// \see        serial_command
/// \see        serial_communication_task
void init_serial_communication(const struct serial_command commands[], uint8_t commands_size);

/// \brief      Main task for USB communication
/// \details    This task must be run as a slow or medium speed task in order to enable USB
///             communication and firmware updates. It processes commands arriving on the bus
///             and takes required actions by calling registered handler functions.
/// \see        init_serial_communication
void serial_communication_task(void);


//---------------- EOF ----------------//
#endif // _SERIAL_COM_H

