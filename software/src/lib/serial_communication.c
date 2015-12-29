/// \file
/// \brief      Implementation of the serial communication module
/// \see        module header

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
#include <avr/wdt.h>

#include "xboot/xbootapi.h"

#include "background_tasks.h"
#include "leds.h"
#include "usb.h"
#include "serial_communication.h"
#include "system.h"



////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

/// \brief      Buffer for incoming commands
static char cmd_buffer[CMD_BUFFER_SIZE] = "";

/// \brief      Command buffer write index
static uint8_t cmd_buffer_index = 0;

/// \brief      Array of user-defined commands
/// \see        init_serial_communication
static struct serial_command* user_commands;

/// \brief      Number of registered user commands
static uint8_t user_commands_size;

/// \brief      Device reset flag
/// \details    This flag is set when the device receives the `reset` command. The device will then
///             stop receiving data, send a shutdown message, count down a timeout and reset itself.
static bool reset = false;

/// \brief      This flag indicates if an update is in progress
static bool update_in_progress = false;

/// \brief      Number of pending update bytes
static uint16_t update_bytes_pending = 0;

/// \brief      Number of pages required for update
static uint16_t num_pages = 0;

/// \brief      CRC of the firmware
static uint16_t expected_crc = 0;

/// \brief      Buffer for application pages
static uint8_t page_buffer[SPM_PAGESIZE] = "";

/// \brief      Page buffer write index
static uint16_t page_buffer_index = 0;

/// \brief      Temporary application write address
static uint32_t temp_app_addr = 0;



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

/// \brief      Handler for the `help` command
static inline bool exec_help(void)
{
    usb_puts("");
    usb_puts("Welcome to the uMIDI serial interface!");
    usb_puts("Here is a list of available commands:");
    usb_puts("    clear        :  Clears the console by printing CR/LFs.");
    usb_puts("    fwupdate <s> :  Initiates a firmware update:");
    usb_puts("                    <s> : firmware update packet size");
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

/// \brief      Handler for the `led` command
static inline bool exec_led(const char* command)
{
    // Abort if the command is malformed
    if (strlen(command) != 7 || command[4] != ' ' || command[6] != ' ') {
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

/// \brief      Handler for the `update` command
static inline bool exec_update(const char* command)
{
    // Make sure the command is well-formed
    if (command[8] != ' ') {
    }

    // Parse size of incoming update packet
    update_bytes_pending = atoi(command + 9);
    if (update_bytes_pending == 0) {
        return false;
    }

    // Calculate number of pages
    num_pages = update_bytes_pending / SPM_PAGESIZE;
    if (update_bytes_pending % SPM_PAGESIZE) {
        ++num_pages;
    }

    // Erase temporary application memory
    usb_puts("Erasing temporary application flash section...");
    wdt_disable();
    uint8_t error_code = xboot_app_temp_erase();
    if (error_code != XB_SUCCESS) {
        usb_printf("Error erasing temprary application section: %d\n\r", error_code);
        return false;
    }
    wdt_reenable();

    // Switch to update mode
    usb_printf("Ready to receive %u bytes (%u pages)...\n\r", update_bytes_pending, num_pages);
    usb_set_echo(false);
    update_in_progress = true;
    page_buffer_index = 0;
    temp_app_addr = 0;
    return true;
}

/// \brief      Executes an interactive command
/// \details    Tries to match the beginning of the supplied string to the registered commands.
///             If a matching command string is found, its handler is invoked.
///             In case no suitable command could be matched, or the executed handler returns a
///             non-zero value, an error message is sent.
/// \param      command
///                 the full command line as a C-string
static inline void execute_command(const char* command)
{
    bool success = true;

    // Ignore empty command
    if (strcmp(command, "") == 0) {
        return;
    }

    else if (strcmp(command, "clear") == 0) {
        for (int i=0; i<80; ++i) {
            usb_puts("");
        }
    }

    else if (strncmp(command, "fwupdate", 8) == 0) {
        success = exec_update(command);
    }

    else if (strcmp(command, "help") == 0) {
        success = exec_help();
    }

    else if (strncmp(command, "led", 3) == 0) {
        success = exec_led(command);
    }

    else if (strcmp(command, "reset") == 0) {
        usb_puts("Resetting device...");
        reset = true;
    }

    else {
        // Iterate all user-defined commands and try to find a matching one
        for (uint8_t i=0; i<user_commands_size; ++i) {
            struct serial_command* user_command = user_commands + i;
            if (strncmp(command, user_command->cmd_string, strlen(user_command->cmd_string)) == 0) {
                success = user_command->handler(command);
                goto cleanup;
            }
        }

        // No known command matches :-(
        usb_printf("Unknown command: [%s]\n\r", command);
        usb_puts("Type `help` for help.\n\r");
    }

cleanup:
    if (!success) {
        usb_printf("Error executing command: [%s]\n\r", command);
    }

    // Clear command buffer and reset write pointer
    memset(cmd_buffer, '\0', sizeof(cmd_buffer));
    cmd_buffer_index = 0;
}

/// \brief      Processes a command character
/// \details    If the supplied character is a carriage return, the command line read so far is
///             executed, otherwise the character is simply appended to a (circular!) buffer.
static inline void process_command_char(void)
{
    // Fetch a character from the USB data buffer
    char data = usb_getc();

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

/// \brief      Processes binary data arriving over USB during the update process
/// \details    If the supplied character is a carriage return, the command line read so far is
///             executed, otherwise the character is simply appended to a (circular!) buffer.
static inline void process_update_data(void)
{
    // Receive and copy bytes to page buffer
    while (usb_bytes_received()) {
        page_buffer[page_buffer_index] = usb_getc();
        ++page_buffer_index;
        --update_bytes_pending;
    }

    // When the last byte has arrived
    if (update_bytes_pending == 0) {
        // Extract expected CRC checksum
        expected_crc = page_buffer[page_buffer_index-2] << 8;
        expected_crc |= page_buffer[page_buffer_index-1];
        if (expected_crc == 0) {
            usb_puts("Invalid empty CRC found!");
            return;
        }
        usb_printf("Expected CRC checksum: %x\n\r", expected_crc);

        // Rewind page_buffer_index
        page_buffer_index -= 2;

        // Pad last page with 0xFF for CRC calculation
        usb_puts("Padding last application page...");
        while (page_buffer_index < SPM_PAGESIZE)
        {
            page_buffer[page_buffer_index] = 0xff;
            ++page_buffer_index;
        }
    }

    // When a page is full, write it to the temporary application flash
    if (page_buffer_index == SPM_PAGESIZE) {
        // TODO: For unknown reasons, the following call prints 0 for the total number of pages:
        /*
        usb_printf("Writing temporary application page [%3u/%3u]...\n\r",
                   temp_app_addr / SPM_PAGESIZE + 1,
                   num_pages);
        */
        usb_printf("Writing temporary application page [%3u", temp_app_addr / SPM_PAGESIZE + 1);
        usb_printf("/%3u]...\n\r", num_pages);
        if (xboot_app_temp_write_page(temp_app_addr, page_buffer, 0) != XB_SUCCESS) {
            goto fail;
        }
        temp_app_addr += SPM_PAGESIZE;
        page_buffer_index = 0;
    }

    if (update_bytes_pending == 0) {
        // Calculate CRC of the written application and compare with expected CRC
        usb_puts("Performing CRC...");
        uint16_t actual_crc = 0;
        xboot_app_temp_crc16(&actual_crc);
        if (expected_crc != actual_crc) {
            usb_printf("CRC checksum mismatch: %x != %x\n\r", expected_crc, actual_crc);
            goto fail;
        }
        usb_puts("Success!");

        // Tell xboot to install the firmware on next reset
        if (xboot_install_firmware(expected_crc) != XB_SUCCESS) {
            goto fail;
        }

        // Reset device
        usb_puts("Resetting device and installing new firmware...");
        reset = true;
    }

    return;
fail:
    // Return to normal operation if the update failed
    usb_puts("Update failed!");
    update_in_progress = false;
    usb_set_echo(true);
}

void init_serial_communication(struct serial_command commands[], uint8_t commands_size)
{
    // Save pointer to array of user commands and its size
    user_commands = commands;
    user_commands_size = commands_size;
}

void serial_communication_task(void)
{
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

    // Process update data / command characters
    if (update_in_progress) {
        process_update_data();
    }
    else {
        process_command_char();
    }
}
