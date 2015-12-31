/// \file
/// \brief      USB CDC device API
/// \details    This module uses the LUFA library to provide serial communication over USB.
///             TODO: More details...

/*
 * Copyright 2015 Simon Gansen
 *
 * This file is part of the uMIDI firmware.
 *
 * the uMIDI firmware is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * the uMIDI firmware is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the uMIDI firmware.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _USB_H
#define _USB_H


//---------------- includes -----------------//
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdarg.h>

#include "lufa/LUFA/Drivers/USB/USB.h"



//---------------- constants ----------------//

/// \brief      Endpoint address of the CDC device-to-host notification IN endpoint.
#define CDC_NOTIFICATION_EPADDR     (ENDPOINT_DIR_IN  | 2)

/// \brief      Endpoint address of the CDC device-to-host data IN endpoint.
#define CDC_TX_EPADDR               (ENDPOINT_DIR_IN  | 3)

/// \brief      Endpoint address of the CDC host-to-device data OUT endpoint.
#define CDC_RX_EPADDR               (ENDPOINT_DIR_OUT | 4)

/// \brief      Newline character sequence
#define USB_NEWLINE                 "\r\n"


//---------------- data types ---------------//


//---------------- functions and procedures ----------------//

/// \brief      Initializes the USB CDC module
/// \details    Calls LUFA's USB_Init() function and enables interrupts.
void init_usb_module(void);

/// \brief      Returns the number of unread bytes currently residing in the USB receive buffer
/// \return     the number of pending bytes
uint16_t usb_bytes_received(void);

/// \brief      Reads one byte or character from the USB receive buffer
/// \details    If #send_echo is set, the character is also immediately sent back to the sender,
///             whereby carriage return characters are replaced by #USB_NEWLINE.
/// \return     the character read cast to an `int16_t` or `EOF`
int16_t usb_getc(void);

/// \brief      Main background task for USB CDC operation
/// \details    Executes relevant LUFA tasks. This task should be included in the mid frequency
///             background task queue. Depending on your application and requirements, it might
///             also be reasonable to move this to either the slow or the fast queue - yielding
///             lower resp. higher data transfer rates.
void usb_main_task(void);

/// \brief      Does exactly what you would expect. ;-)
/// \details    Unlike the "real" `printf`, this function does not return the number of written
///             bytes, because it can not be guaranteed that the bytes really got sent over the
///             bus. After writing the string to the send buffer, it is flushed.
/// \see        man 3 printf
void usb_printf(const char* format, ...);

/// \brief      Sends the given character over USB
/// \details    Flushes the USB send buffer.
void usb_putc(char c);

/// \brief      Sends the given string and a newline sequence over USB
/// \details    Flushes the USB send buffer.
/// \see        USB_NEWLINE
void usb_puts(char* string);

/// \brief      Enables / disables terminal echo
/// \details    When the echo is enabled, every character received over USB is immediately sent
///             back to the source to provide a shell / terminal-like interactive experience.
///             While this is cool and desirable for user interaction, it most certainly is not a
///             good idea, when you want to transfer larger character streams or even binary data.
/// \param      echo
///                 new value for the flag
void usb_set_echo(bool echo);


//---------------- EOF ----------------//
#endif // _USB_H
