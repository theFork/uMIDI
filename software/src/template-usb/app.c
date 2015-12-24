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

#include <stdlib.h>
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

/// \brief      This flag indicates if an update is in progress
static bool update_in_progress = false;

/// \brief      This number stores the size of an incoming update packet
static uint16_t update_bytes_pending = 0;

/// \brief      Buffer for application pages
static uint8_t page_buffer[SPM_PAGESIZE] = "";

/// \brief      Page buffer write index
static uint16_t page_buffer_index = 0;

/// \brief      Temporary application write address
static uint32_t temp_app_addr = 0;



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

static inline bool exec_help(void)
{
    usb_puts("");
    usb_puts("Welcome to the uMIDI serial interface!");
    usb_puts("Here is a list of available commands:");
    usb_puts("    clear        :  Clears the console by printing CR/LFs.");
    usb_puts("    fwupdate <s> :  Initiates a firmware update:");
    usb_puts("                    <s>: firmware update packet size");
    usb_puts("    help         :  Prints this help message.");
    usb_puts("    led <l> <a>  :  Manipulates the two on-board LEDs:");
    usb_puts("                    <l> : LED to manipulate");
    usb_puts("                          'g' = green LED");
    usb_puts("                          'r' = red LED");
    usb_puts("                          '!' = both LEDs");
    usb_puts("                    <a> : LED mode / action");
    usb_puts("                          'b' = blink");
    usb_puts("                          'f' = flash");
    usb_puts("                          't' = toggle");
    usb_puts("    reset        :  Resets the device.");
    usb_puts("Please enter a command:");

    // Success
    return true;
}

static inline bool exec_led(const char* command)
{
    // Abort if the command is malformed
    if (strlen(command) != 7) {
        usb_printf("Length: %d\n\r", strlen(command));
        return false;
    }

    // Parse LED(s) to manipulate
    enum led led;
    switch (command[4]) {
    case 'g':
        led = LED_GREEN;
        break;

    case 'r':
        led = LED_RED;
        break;

    case '!':
        led = LED_ALL;
        break;

    default:
        usb_puts("LED\n\r");
        return false;
    }

    // Parse and execute action
    switch (command[6]) {
    case 'b':
        blink_led(led, F_TASK_SLOW);
        break;

    case 'f':
        flash_led(led);
        break;

    case 't':
        toggle_led(led);
        break;

    default:
        usb_puts("Action\n\r");
        return false;
    }

    return true;
}

static inline bool exec_update(const char* command)
{
    // Parse size of incoming update packet
    update_bytes_pending = atoi(command + 9);
    if (update_bytes_pending == 0) {
        return false;
    }
    if (update_bytes_pending > SPM_PAGESIZE) {
        usb_puts("Buffer is too small!\n\r");
        return false;
    }

    usb_printf("Ready to receive %u bytes of raw data...\n\r", update_bytes_pending);
    usb_set_echo(false);
    update_in_progress = true;
    return true;
}

static void execute_command(const char* command)
{
    bool success = true;

    // Ignore empty command
    if (strcmp(command, "") == 0) {
        return;
    }

    else if (strcmp(command, "clear") == 0) {
        for (int i=0; i<128; ++i) {
            usb_puts("");
        }
    }

    else if (strcmp(command, "help") == 0) {
        success = exec_help();
    }

    else if (strncmp(command, "led ", 4) == 0) {
        success = exec_led(command);
    }

    else if (strncmp(command, "fwupdate ", 9) == 0) {
        success = exec_update(command);
    }

    else if (strcmp(command, "reset") == 0) {
        usb_puts("Resetting device...");
        reset = true;
    }

    else {
        usb_printf("Unknown command: [%s]\n\r", command);
        usb_puts("Type `help` for help.");
    }

    if (!success) {
        usb_printf("Error executing command: [%s]\n\r", command);
    }

    // Clear command buffer and reset write pointer
    memset(cmd_buffer, '\0', sizeof(cmd_buffer));
    cmd_buffer_index = 0;
}

static inline void process_command_char(char data)
{
    // Parse and execute commands if the enter key was hit
    if (data == '\r') {
        execute_command(cmd_buffer);
        return;
    }

    // Add char to command buffer; cycle on overflow
    cmd_buffer[cmd_buffer_index] = data;
    ++cmd_buffer_index;
    cmd_buffer_index %= CMD_BUFFER_SIZE;
}

static inline void process_update_data(void)
{
    page_buffer[page_buffer_index] = usb_getc();
    ++page_buffer_index;
    --update_bytes_pending;

    // TODO: Extract CRC from last page and rewind page_buffer_index
    // TODO: Pad last page
    // TODO: When a page is full, write it to the temporary application flash
    // TODO: Calculate CRC of the written application and compare with expected CRC
    // TODO: Tell xboot to install the firmware on next reset
    // TODO: Reset device

    // Return to normal operation if the update failed
    if (update_bytes_pending == 0) {
        usb_puts("Update failed!");
        update_in_progress = false;
        usb_set_echo(true);
    }
}

void serial_communication_task(void)
{
    // Announce readiness
    // TODO: Find out, when it is REALLY ok to send.
    static uint8_t counter = 80;
    if (counter > 0) {
        usb_puts("Hello, friend!");
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

    // If nothing was received, there is nothing to do
    if (!usb_bytes_received()) {
        return;
    }

    // Just append data to buffer if raw data mode is active
    if (update_in_progress) {
        process_update_data();
    }
    else {
        process_command_char(usb_getc());
    }
}
