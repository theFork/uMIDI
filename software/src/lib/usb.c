/// \file
/// \brief      USB CDC module implementation

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

//---------------- includes ----------------//

#include <limits.h>
#include <stdarg.h>
#include <stdio.h>

#include "lufa/LUFA/Drivers/USB/USB.h"

#include "leds.h"
#include "usb.h"
#include "usb_descriptors.h"
#include "system.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

/// \brief      LUFA CDC Class driver interface configuration and state information.
/// \details    This structure is passed to all CDC Class driver functions, so that multiple
///             instances of the same class within a device can be differentiated from one another.
static USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface = {
    .Config = {
        .ControlInterfaceNumber = 0,

        .DataINEndpoint = {
            .Address    = CDC_TX_EPADDR,
            .Size       = CDC_TXRX_EPSIZE,
            .Banks      = 1,
        },
        .DataOUTEndpoint = {
            .Address    = CDC_RX_EPADDR,
            .Size       = CDC_TXRX_EPSIZE,
            .Banks      = 1,
        },
        .NotificationEndpoint = {
            .Address    = CDC_NOTIFICATION_EPADDR,
            .Size       = CDC_NOTIFICATION_EPSIZE,
            .Banks      = 1,
        },
    },
};

/// \brief      TODO
static volatile uint8_t ok_to_send = 0;



////////////////////////////////////////////////////////////////
//                     C A L L B A C K S                      //
////////////////////////////////////////////////////////////////

/// \brief      Event handler for the library USB Connection event.
void EVENT_USB_Device_Connect(void)
{
    flash_led(LED_RED);
}

/// \brief      Event handler for the library USB Disconnection event.
void EVENT_USB_Device_Disconnect(void)
{
    flash_led(LED_RED);
}

/// \brief      Event handler for the library USB Configuration Changed event.
void EVENT_USB_Device_ConfigurationChanged(void)
{
    bool ok = CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface);
    if (!ok) {
        for(;;) {
            flash_led(LED_RED);
            flash_led(LED_GREEN);
        }
    }
}

/// \brief      Event handler for the library USB Control Request reception event.
void EVENT_USB_Device_ControlRequest(void)
{
    CDC_Device_ProcessControlRequest(&VirtualSerial_CDC_Interface);
}

/// \brief      Callback for control line state changes.
void EVENT_CDC_Device_ControLineStateChanged(USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo)
{
    ok_to_send = (CDCInterfaceInfo->State.ControlLineStates.HostToDevice
               &  CDC_CONTROL_LINE_OUT_DTR);
}



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void init_usb_module(void)
{
    // Init procedure from LUFA
    USB_Init();

    // Turn on all low level interrupts
    PMIC.CTRL |= PMIC_LOLVLEN_bm;
}

int usb_printf(const char* format, ...)
{
    va_list ap;
    int i;
    char buffer[80] = "";

    va_start(ap, format);
    i = vsprintf(buffer, format, ap);
    va_end(ap);

    usb_send_string(buffer);
    return i;
}

char usb_receive_char(bool echo)
{
    unsigned char data = CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);

    // Echo back the received character if desired
    if (echo) {
        switch (data) {
        case USB_EMPTY_CHAR:
            // Do not echo dummy characters
            break;

        case '\r':
            usb_send_string("\n\r");
            break;

        default:
            usb_send_char(data);
            break;
        }
    }

    return data;
}

void usb_send_char(char c)
{
    CDC_Device_SendByte(&VirtualSerial_CDC_Interface, c);
    CDC_Device_Flush(&VirtualSerial_CDC_Interface);
}

void usb_send_string(char* string)
{
    if (USB_DeviceState == DEVICE_STATE_Configured && ok_to_send) {
        int error;
        error = CDC_Device_SendString(&VirtualSerial_CDC_Interface, string);
        if (error) {
            panic(400, 100);
        }
        error = CDC_Device_Flush(&VirtualSerial_CDC_Interface);
        if (error) {
            // enum Endpoint_WaitUntilReady_ErrorCodes_t
            panic(100, 400);
        }
    }
}

void usb_main_task(void)
{
    CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
    USB_USBTask();
}
