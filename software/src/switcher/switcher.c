/// \file
/// \brief      Implementation of the MIDI switcher module

/*
 * Copyright 2016 Simon Gansen
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

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include "lib/background_tasks.h"
#include "lib/gpio.h"
#include "lib/leds.h"
#include "lib/program.h"
#include "lib/usb.h"

#include "config.h"
#include "switcher.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

union program_data
{
    // Either raw data in the memory ...
    uint16_t word;

    // ... or named bits
    struct {
        bool tuner      : 1;
        bool loop1      : 1;
        bool loop2      : 1;
        bool loop3      : 1;
        bool loop4      : 1;
        bool loop5      : 1;
        bool switch1    : 1;
        bool switch2    : 1;
    } bit;
};

/// \brief  Switch configuration of the current program
static union program_data current_program;

/// \brief  The current switch configuration
static union program_data effective_program;


////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void execute_program(uint32_t program_data)
{
    current_program.word = program_data;
    effective_program.word = program_data;

    usb_printf("Applying relays configuration: 0x%02x" USB_NEWLINE, current_program.word);

    gpio_set(GPIO_OUT_TUNE_MUTE, current_program.bit.tuner);
    gpio_set(GPIO_OUT_LOOP1, current_program.bit.loop1);
    gpio_set(GPIO_OUT_LOOP2, current_program.bit.loop2);
    gpio_set(GPIO_OUT_LOOP3, current_program.bit.loop3);
    gpio_set(GPIO_OUT_LOOP4, current_program.bit.loop4);
    gpio_set(GPIO_OUT_LOOP5, current_program.bit.loop5);
    gpio_set(GPIO_OUT_SWITCH1, current_program.bit.switch1);
    gpio_set(GPIO_OUT_SWITCH2, current_program.bit.switch2);

}

bool exec_load(const char* command)
{
    // Abort if the command is malformed
    if (strlen(command) < 6 || command[4] != ' ') {
        usb_puts("Malformed command" USB_NEWLINE);
        return false;
    }

    uint8_t number = atoi(command+5);
    enter_program(number);
    return true;
}

bool exec_relay(const char* command)
{
    // Abort if the command is malformed
    if (strlen(command) != 8 || command[3] != ' ' || command[6] != ' ') {
        usb_puts("Malformed command" USB_NEWLINE);
        return false;
    }

    // Select relay (corresponding gpio) to manipulate
    struct gpio_pin selected_gpio;
    uint8_t program_bit_index;

    // Tune/Mute
    if (command[4] == 't' && command[5] == 'm') {
        selected_gpio = GPIO_OUT_TUNE_MUTE;
        program_bit_index = 0;
    }
    // Loopers
    else if (command[4] == 'l') {
        switch (command[5]) {
            case '1':
                selected_gpio = GPIO_OUT_LOOP1;
                program_bit_index = 1;
                break;
            case '2':
                selected_gpio = GPIO_OUT_LOOP2;
                program_bit_index = 2;
                break;
            case '3':
                selected_gpio = GPIO_OUT_LOOP3;
                program_bit_index = 3;
                break;
            case '4':
                selected_gpio = GPIO_OUT_LOOP4;
                program_bit_index = 4;
                break;
            case '5':
                selected_gpio = GPIO_OUT_LOOP5;
                program_bit_index = 5;
                break;
            default:
                usb_puts("No such looper relay" USB_NEWLINE);
                return false;
        }
    }
    // Switches
    else if (command[4] == 's') {
        switch (command[5]) {
            case '1':
                selected_gpio = GPIO_OUT_SWITCH1;
                program_bit_index = 6;
                break;
            case '2':
                selected_gpio = GPIO_OUT_SWITCH2;
                program_bit_index = 7;
                break;
            default:
                usb_puts("No such switch relay" USB_NEWLINE);
                return false;
        }
    }
    else {
        usb_puts("Unrecognized relay" USB_NEWLINE);
        return false;
    }

    // Parse action and switch relay
    bool action = false;
    if (command[7] == 'a') {
        action = true;
        effective_program.word |= _BV(program_bit_index);
    }
    else {
        effective_program.word &=~ _BV(program_bit_index);
    }
    gpio_set(selected_gpio, action);
    return true;
}

bool exec_save(const char* command)
{
    // Abort if the command is malformed
    if (strlen(command) != 4) {
        usb_puts("Malformed command" USB_NEWLINE);
        return false;
    }

    update_current_program(effective_program.word);
    return true;
}

void handle_program_change(uint8_t program)
{
    enter_program(program);
    flash_led_multiple(&save_led, 1);
    usb_printf("Entering program %u" USB_NEWLINE, program);
}

void unknown_midi_message_handler(void)
{
    flash_led_multiple(&save_led, 2);
    usb_puts("Ignoring unknown MIDI message");
}

void poll_switches(void)
{
    // Handle save switch
    if (poll_gpio_input_timeout(GPIO_IN_SAVE_SWITCH, GPIO_INPUT_PULLUP, 10) == GPIO_INPUT_EVENT_LONG) {
        // Abort if the program is unchanged
        if (current_program.word == effective_program.word) {
            return;
        }

        usb_puts("Saving program");
        current_program.word = effective_program.word;
        update_current_program(current_program.word);
        flash_led_multiple(&save_led, 3);
        return;
    }

    // Handle program-related switches
    for (uint8_t switch_index=0; switch_index<8; ++switch_index) {
        if (poll_gpio_input(*gpio_mappings[8+switch_index].pin, GPIO_INPUT_PULLUP)) {
            usb_printf("Switch #%u was pressed" USB_NEWLINE, switch_index);

            // Update current program
            effective_program.word ^= _BV(switch_index);

            // Toggle relays
            gpio_toggle(*gpio_mappings[switch_index].pin);
        }
    }

    // Indicate modified program
    if (current_program.word != effective_program.word) {
        blink_led(&save_led, F_TASK_SLOW/4);
    }
    else if (save_led.state.mode != LED_FLASH) {
        set_led(&save_led, false);
    }
}
