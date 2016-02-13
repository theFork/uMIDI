/// \file
/// \brief      USB CDC device driver implementation

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

/// \brief      This flag indicates if the USB port is ready to send data
/// \details    The flag is set / cleared by a LUFA event callback.
/// \see        EVENT_CDC_Device_ControLineStateChanged
static bool ok_to_send = 0;



////////////////////////////////////////////////////////////////
//                     C A L L B A C K S                      //
////////////////////////////////////////////////////////////////

/// \brief      Event handler for the library USB Connection event.
/// \brief      Indicates device connection by flashing the red on-board LED.
void EVENT_USB_Device_Connect(void)
{
    flash_led(LED_RED);
}

/// \brief      Event handler for the library USB Disconnection event.
/// \brief      Indicates device disconnection by flashing the red on-board LED.
void EVENT_USB_Device_Disconnect(void)
{
    flash_led(LED_RED);
}

/// \brief      Event handler for the library USB Configuration Changed event.
/// \details    Configures the endpoints or enters panic mode if something goes wrong.
void EVENT_USB_Device_ConfigurationChanged(void)
{
    if (!CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface)) {
        panic(100, 800);
    }
}

/// \brief      Event handler for the library USB Control Request reception event.
void EVENT_USB_Device_ControlRequest(void)
{
    CDC_Device_ProcessControlRequest(&VirtualSerial_CDC_Interface);
}

/// \brief      Callback for control line state changes.
/// \details    Sets / clears the #ok_to_send flag.
void EVENT_CDC_Device_ControLineStateChanged(USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo)
{
    ok_to_send = (CDCInterfaceInfo->State.ControlLineStates.HostToDevice
               &  CDC_CONTROL_LINE_OUT_DTR);
}



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

/// \brief      Flushes the USB send buffer
/// \details    Panics if something goes wrong!
static void flush(void)
{
    // The following call returns an enum Endpoint_WaitUntilReady_ErrorCodes_t value
    if (USB_DeviceState == DEVICE_STATE_Configured && ok_to_send) {
        enum Endpoint_WaitUntilReady_ErrorCodes_t error;
        error = CDC_Device_Flush(&VirtualSerial_CDC_Interface);
        if (error != ENDPOINT_READYWAIT_NoError) {
            panic(100, 400);
        }
    }
}

/// \brief      Writes the given string to the USB send buffer and flushes it
/// \details    Panics if something goes wrong!
static void send_string(const char * const string)
{
    if (USB_DeviceState == DEVICE_STATE_Configured && ok_to_send) {
        enum Endpoint_Stream_RW_ErrorCodes_t error;
        error = CDC_Device_SendString(&VirtualSerial_CDC_Interface, string);
        if (error != ENDPOINT_RWSTREAM_NoError) {
            panic(400, 100);
        }
        flush();
    }
}

void init_usb_module(void)
{
    // Init procedure from LUFA
    USB_Init();

    // Turn on all low level interrupts
    PMIC.CTRL |= PMIC_LOLVLEN_bm;
}

uint16_t usb_bytes_received(void)
{
    if (USB_DeviceState == DEVICE_STATE_Configured) {
        return CDC_Device_BytesReceived(&VirtualSerial_CDC_Interface);
    }
    return 0;
}

int16_t usb_getc(void)
{
    int16_t data = EOF;
    if (USB_DeviceState == DEVICE_STATE_Configured) {
        data = CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);
    }

    // The LUFA documentation states that "a negative value" is returned if no data was received.
    // We'll map all of these to EOF to make things a little more deterministic.
    if (data < 0) {
        data = EOF;
    }

    return data;
}

void usb_main_task(void)
{
    CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
    USB_USBTask();
}

void usb_printf(const char * const format, ...)
{
    va_list ap;
    char buffer[80] = "";

    va_start(ap, format);
    vsprintf(buffer, format, ap);
    va_end(ap);

    send_string(buffer);
}

void usb_putc(char c)
{
    if (USB_DeviceState == DEVICE_STATE_Configured && ok_to_send) {
        enum Endpoint_WaitUntilReady_ErrorCodes_t error;
        error = CDC_Device_SendByte(&VirtualSerial_CDC_Interface, c);
        if (error != ENDPOINT_READYWAIT_NoError) {
            panic(800, 100);
        }
        flush();
    }
    else {
        flash_led(LED_RED);
    }
}

void usb_puts(const char * const string)
{
    send_string(string);
    send_string(USB_NEWLINE);
}
