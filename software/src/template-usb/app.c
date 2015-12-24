/// \file
/// \brief      Implementation of the application specific module

/*
 * Copyright 2015 Sebastian Neuser
 *
 * This file is part of the uMIDI firmware.
 *
 * The uMIDI firmware is free software: you can redistribute it and/or modify
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

#include "lib/background_tasks.h"
#include "lib/gpio.h"
#include "lib/leds.h"
#include "lib/midi.h"
#include "lib/usb.h"
#include "xboot/xbootapi.h"

#include "config.h"
#include "app.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

/// \brief      Device reset flag
/// \details    This flag is set when the device receives the `reset` command. The device will then
///             stop receiving data, send a shutdown message, count down a timeout and reset itself.
static bool reset = false;


////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

static void print_help(void)
{
    usb_send_string("\n\r");
    usb_send_string("Welcome to the uMIDI serial interface!\n\r");
    usb_send_string("These are your options:\n\r");
    usb_send_string("    c  :  Clears the console by printing CR/LFs\n\r");
    usb_send_string("    h  :  Prints this help message\n\r");
    usb_send_string("    r  :  Toggles the red led\n\r");
    usb_send_string("    t  :  Tests the usb_printf() function\n\r");
    usb_send_string("Please enter a command:\n\r");
}

void serial_communication_task(void)
{
    // Announce readiness
    // TODO: Find out, when it is REALLY ok to send.
    static uint8_t counter = 80;
    if (counter > 0) {
        usb_send_string("Hello, friend!\n\r");
        --counter;
    }

    // Wait for timeout and reset device if the command was issued
    if (reset) {
        static uint8_t reset_timeout = RESET_TIMEOUT * F_TASK_SLOW;
        --reset_timeout;
        if (reset_timeout == 0) {
            xboot_reset();
        }
        return;
    }

    // Receive character with echo enabled
    char data = usb_receive_char(true);

    // Execute command
    static uint16_t test_counter = 0;
    switch (data) {
    case USB_EMPTY_CHAR:
        break;

    case 'b':
        usb_send_string("\n\rResetting device...\n\r");
        reset = true;
        break;

    case 'c':
        for (int i=0; i<128; ++i) {
            usb_send_string("\n\r");
        }
        break;

    case 'h':
        print_help();
        break;

    case 't':
        usb_printf("\n\rTest call %u, this is %d%d%d%d!\n\r", ++test_counter, 1, 3, 3, 7);
        break;

    case 'r':
        toggle_led(LED_RED);
        usb_send_string("\n\rToggling red LED\n\r");
        break;

    default:
        usb_send_string("\n\rUnknown command\n\r");
        print_help();
        break;
    }
}
