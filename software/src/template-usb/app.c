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

#include <string.h>

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

/// \brief      Buffer for incoming commands
static char cmd_buffer[CMD_BUFFER_SIZE] = "";

/// \brief      Command buffer write index
static uint8_t cmd_buffer_index = 0;


////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

static inline void exec_help(void)
{
    usb_send_string("\n\r");
    usb_send_string("Welcome to the uMIDI serial interface!\n\r");
    usb_send_string("Here is a list of available commands:\n\r");
    usb_send_string("    clear      :  Clears the console by printing CR/LFs\n\r");
    usb_send_string("    help       :  Prints this help message\n\r");
    usb_send_string("    reset      :  Resets the device\n\r");
    usb_send_string("    led <c>    :  Toggles one of the two LEDs:\n\r");
    usb_send_string("                  c='g' -> green LED\n\r");
    usb_send_string("                  c='r' -> red LED\n\r");
    usb_send_string("Please enter a command:\n\r");
}

static inline void exec_led(char led)
{
    if (led == 'g') {
        usb_send_string("Toggling green LED\n\r");
        toggle_led(LED_GREEN);
    }
    else if (led == 'r') {
        usb_send_string("Toggling red LED\n\r");
        toggle_led(LED_RED);
    }
}

static void execute_command(const char* command)
{
    // Ignore empty command
    if (strcmp(command, "") == 0) {
        return;
    }

    else if (strcmp(command, "clear") == 0) {
        for (int i=0; i<128; ++i) {
            usb_send_string("\n\r");
        }
    }

    else if (strcmp(command, "help") == 0) {
        exec_help();
    }

    else if (strncmp(command, "led ", 4) == 0) {
        if (strlen(command) == 5) {
            exec_led(command[4]);
        }
    }

    else if (strcmp(command, "reset") == 0) {
        usb_send_string("Resetting device...\n\r");
        reset = true;
    }

    else {
        usb_printf("\n\rUnknown command: [%s]\n\r", command);
        usb_send_string("Type `help` for help...\n\r");
    }

    // Clear command buffer and reset write pointer
    memset(cmd_buffer, '\0', sizeof(cmd_buffer));
    cmd_buffer_index = 0;
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

    // Parse and execute commands
    switch (data) {
    case USB_EMPTY_CHAR:
        // Ignore non-printable (non-ASCII) characters
        break;

    case '\r':
        // Parse and execute command
        execute_command(cmd_buffer);
        break;

    default:
        // Add char to command buffer; cycle on overflow
        cmd_buffer[cmd_buffer_index] = data;
        ++cmd_buffer_index;
        cmd_buffer_index %= CMD_BUFFER_SIZE;
        break;
    }
}
