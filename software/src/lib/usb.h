/// \file
/// \brief      USB CDC module API
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

/// \brief      Empty character code
/// \details    This is returned by #receive_char when no character was received.
#define USB_EMPTY_CHAR              255


//---------------- data types ---------------//


//---------------- functions and procedures ----------------//

/// \brief      Initializes the USB CDC module.
/// \details    TODO
void init_usb_module(void);

/// \brief      TODO
/// \details    TODO
uint16_t usb_bytes_received(void);

/// \brief      TODO
/// \details    TODO
int16_t usb_getc(void);

/// \brief      TODO
/// \details    TODO
void usb_main_task(void);

/// \brief      TODO
/// \details    TODO
int usb_printf(const char* format, ...);

/// \brief      TODO
/// \details    TODO
void usb_putc(char c);

/// \brief      TODO
/// \details    TODO
void usb_puts(char* string);

/// \brief      TODO
/// \details    TODO
void usb_set_echo(bool echo);


//---------------- EOF ----------------//
#endif // _USB_H
