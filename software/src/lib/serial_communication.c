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

/// \brief      Buffer for incoming commands
static char cmd_history[CMD_HISTORY_SIZE][CMD_BUFFER_SIZE] = {"",};

/// \brief      Command buffer write index
static uint8_t cmd_history_index = 0;

/// \brief      Array of user-defined commands
/// \see        init_serial_communication
static const struct serial_command* user_commands;

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

/// \brief      Terminal echo flag
/// \details    When this flag is set, received bytes are immediately echoed back to provide a
///             shell-like experience.
static bool echo_on = true;



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

/// \brief      Handler for the `help` command
static inline bool exec_help(void)
{
    usb_puts("");
    usb_puts("Welcome to the uMIDI serial interface!");
    usb_puts("Built-in commands:");
    usb_puts("    clear             :  Clears the console by printing CR/LFs.");
    usb_puts("    fwupdate <s>      :  Initiates a firmware update:");
    usb_puts("                         <s> : firmware update packet size");
    usb_puts("    help              :  Prints this help message.");
    usb_puts("    reset             :  Resets the device.");

    if (user_commands_size) {
        usb_puts("Special commands:");
    }

    char* tok_ptr;
    for (uint8_t i=0; i<user_commands_size; ++i) {
        // Check if the help string contains newline characters
        char* first_nl = strchr(user_commands[i].help_string, '\n');
        if (first_nl) {
            // Parse specified options / parameters to the command string
            char* params = strtok_r(strdup(user_commands[i].help_string), "\n", &tok_ptr);

            if (user_commands[i].help_string == first_nl) {
                // If the first character of the help string is a newline character, the parsed
                // params are actually the first line of the description text.
                usb_printf("    %-16s  :  %s" USB_NEWLINE, user_commands[i].cmd_string, params);
            }
            else {
                // Append options / parameters to command string and print the whole thing
                char first_column[17] = "";
                snprintf(first_column, sizeof(first_column),
                         "%s %s", user_commands[i].cmd_string, params);
                usb_printf("    %-16s  :  ", first_column);

                // Complete the first line of the help string
                usb_printf("%s" USB_NEWLINE, strtok_r(NULL, "\n", &tok_ptr));
            }

            // Split remaining command description at '\n' chars, pad with spaces and print
            char* tail = strtok_r(NULL, "\n", &tok_ptr);
            while (tail) {
                usb_printf("                         %s" USB_NEWLINE, tail);
                tail = strtok_r(NULL, "\n", &tok_ptr);
            }
        }
        else {
            // Print simple command description
            usb_printf("    %-16s  :  %s" USB_NEWLINE,
                       (user_commands+i)->cmd_string, (user_commands+i)->help_string);
        }
    }

    usb_puts("Please enter a command:");

    // Success
    return true;
}

/// \brief      Handler for the `update` command
static inline bool exec_update(const char * const command)
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

    // Abort if the program is too big to fit into memory
    if (num_pages > MAX_PAGE_NUM) {
        usb_puts("Program is too big! Aborting.");
        return true;
    }

    // Warn if the program is approaching the maximum size
    uint8_t pages_left = MAX_PAGE_NUM - num_pages;
    if (pages_left < 20) {
        usb_printf("Warning: Only %u pages of program space left!" USB_NEWLINE, pages_left);
    }

    // Erase temporary application memory
    usb_puts("Erasing temporary application flash section...");
    wdt_disable();
    uint8_t error_code = xboot_app_temp_erase();
    if (error_code != XB_SUCCESS) {
        usb_printf("Error erasing temprary application section: %d" USB_NEWLINE, error_code);
        return true;
    }
    wdt_reenable();

    // Switch to update mode
    usb_printf("Ready to receive %u bytes (%u pages)..." USB_NEWLINE,
               update_bytes_pending,
               num_pages);
    echo_on = false;
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
static inline void execute_command(const char * const command)
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

    else if (strcmp(command, "reset") == 0) {
        usb_puts("Resetting device...");
        reset = true;
    }

    else {
        // Iterate all user-defined commands and try to find a matching one
        for (uint8_t i=0; i<user_commands_size; ++i) {
            const struct serial_command* user_command = user_commands + i;
            if (strncmp(command, user_command->cmd_string, strlen(user_command->cmd_string)) == 0) {
                success = user_command->handler(command);
                goto cleanup;
            }
        }

        // No known command matches :-(
        usb_printf("Unknown command: [%s]" USB_NEWLINE, command);
        usb_puts("Type `help` for help." USB_NEWLINE);
    }

cleanup:
    if (!success) {
        usb_printf("Error executing command: [%s]" USB_NEWLINE, command);
    }

    // Clear command buffer and reset write pointer
    memset(cmd_buffer, '\0', sizeof(cmd_buffer));
    cmd_buffer_index = 0;
}

/// \brief      Prints out a command from the history
/// \details    Clears out the last printed command first.
/// \param      offset
///                 offset of the command to be printed
static inline void print_command_from_history(const int8_t offset)
{
    // Cycle command history index
    cmd_history_index += CMD_HISTORY_SIZE;
    cmd_history_index += offset;
    cmd_history_index %= CMD_HISTORY_SIZE;

    // Overwrite previously printed command
    usb_putc('\r');

    // Print out command and save command length for the next invocation
    usb_printf("%-40s", cmd_history[cmd_history_index]);
}

/// \brief      Handles ANSI escape sequences
/// \return     `true` as long as we are in the middle of an escape sequence
static bool handle_escape_sequence(const char data)
{
    static uint8_t escape_byte_index = 0;
    if (data == ESCAPE_CHAR_CODE) {
        escape_byte_index = 1;
        return true;
    }
    if (escape_byte_index == 1) {
        if (data == '[') {
            escape_byte_index = 2;
        }
        else {
            usb_puts("Unrecognized escape sequence!");
            escape_byte_index = 0;
        }
        return true;
    }
    if (escape_byte_index == 2) {
        if (data == 'A') {
            print_command_from_history(-1);
        }
        if (data == 'B') {
            print_command_from_history(1);
        }
        escape_byte_index = 3;
        return true;
    }
    if (escape_byte_index == 3) {
        usb_puts("");
        escape_byte_index = 0;

        // Execute selected command if the enter key was hit
        if (data == '\r') {
            execute_command(cmd_history[cmd_history_index]);
            ++cmd_history_index;
            cmd_history_index %= CMD_HISTORY_SIZE;
        }
        return true;
    }
    return false;
}

/// \brief      Processes a command character
/// \details    If the supplied character is a carriage return, the command line read so far is
///             executed, otherwise the character is simply appended to a (circular!) buffer.
static inline void process_command_char(void)
{
    // Fetch a character from the USB data buffer
    char data = usb_getc();

    // Handle escape sequence if any and abort
    if (handle_escape_sequence(data)) {
        return;
    }

    // Echo back the received character if desired and possible
    if (echo_on) {
        if (data == '\r') {
            usb_puts("");
        }
        else if (0 <= data && data < 128) {
            usb_putc(data);
        }
    }

    // Parse and execute commands if the enter key was hit
    if (data == '\r') {
        strncpy(cmd_history[cmd_history_index], cmd_buffer, CMD_BUFFER_SIZE);
        ++cmd_history_index;
        cmd_history_index %= CMD_HISTORY_SIZE;
        execute_command(cmd_buffer);
        return;
    }

    // Rewind buffer index if backspace was received
    if (data == '\b') {
        if (cmd_buffer_index > 0) {
            --cmd_buffer_index;
        }
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
        usb_printf("Expected CRC checksum: %x" USB_NEWLINE, expected_crc);

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
        usb_printf("Writing temporary application page [%3u/%3u]..." USB_NEWLINE,
                   temp_app_addr / SPM_PAGESIZE + 1,
                   num_pages);
        */
        usb_printf("Writing temporary application page [%3u", temp_app_addr / SPM_PAGESIZE + 1);
        usb_printf("/%3u/%3u]..." USB_NEWLINE, num_pages, MAX_PAGE_NUM);
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
            usb_printf("CRC checksum mismatch: %x != %x" USB_NEWLINE, expected_crc, actual_crc);
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
    echo_on = true;
}

void init_serial_communication(const struct serial_command * const commands, uint8_t commands_size)
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
