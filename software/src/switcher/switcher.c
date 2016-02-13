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


////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void execute_program(uint16_t program_data)
{
    union program_data program;
    program.word = program_data;

    gpio_set(GPIO_OUT_TUNE_MUTE, program.bit.tuner);
    gpio_set(GPIO_OUT_LOOP1, program.bit.loop1);
    gpio_set(GPIO_OUT_LOOP2, program.bit.loop2);
    gpio_set(GPIO_OUT_LOOP3, program.bit.loop3);
    gpio_set(GPIO_OUT_LOOP4, program.bit.loop4);
    gpio_set(GPIO_OUT_LOOP5, program.bit.loop5);
    gpio_set(GPIO_OUT_SWITCH1, program.bit.switch1);
    gpio_set(GPIO_OUT_SWITCH2, program.bit.switch2);

}

/// \brief      Handler for the `led` command
bool exec_led(const char* command)
{
    // Abort if the command is malformed
    if (strlen(command) != 7 || command[3] != ' ' || command[5] != ' ') {
        usb_puts("Malformed command" USB_NEWLINE);
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
        usb_puts("No such LED" USB_NEWLINE);
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
        usb_puts("No such action" USB_NEWLINE);
        return false;
    }

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
    // TODO: Create global aliases for the gpio pins
    struct gpio_pin selected_gpio;

    // Tune/Mute
    if (command[4] == 't' && command[5] == 'm') {
        selected_gpio = GPIO_OUT_TUNE_MUTE;
    }
    // Loopers
    else if (command[4] == 'l') {
        flash_led(LED_RED);
        switch (command[5]) {
            case '1':
                selected_gpio = GPIO_OUT_LOOP1;
                break;
            case '2':
                selected_gpio = GPIO_OUT_LOOP2;
                break;
            case '3':
                selected_gpio = GPIO_OUT_LOOP3;
                break;
            case '4':
                selected_gpio = GPIO_OUT_LOOP4;
                break;
            case '5':
                selected_gpio = GPIO_OUT_LOOP5;
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
                break;
            case '2':
                selected_gpio = GPIO_OUT_SWITCH2;
                break;
            default:
                usb_puts("No such switch relay" USB_NEWLINE);
                return false;
        }
    }
    else {
        flash_led(LED_RED);
        usb_puts("Unrecognized relay" USB_NEWLINE);
        return false;
    }

    // Parse action and switch relay
    bool action = false;
    if (command[7] == 'a') {
        action = true;
    }
    gpio_set(selected_gpio, action);
    return true;
}

void handle_program_change(uint8_t program)
{
    flash_led(LED_RED);
    enter_program(program);
}
