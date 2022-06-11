/// \file
/// \brief      Implementation of an application specific module
/// \see        app.h

/*
 * Copyright 2022 Sebastian Neuser
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

#include "lib/gpio.h"
#include "lib/leds.h"
#include "lib/usb.h"

#include "config.h"
#include "wah.h"
#include "app.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

/// \brief      Handler for the `enable` command
bool exec_enable(const char* command)
{
    if (strlen(command) != 8) {
        usb_puts(PSTR("Malformed command" USB_NEWLINE));
        return false;
    }

    bool enable = command[7] == 't';
    configure_gpio_pin(&ENABLE_PIN, GPIO_OUTPUT);
    gpio_set(ENABLE_PIN, false);
    _delay_ms(100);
    configure_gpio_pin(&ENABLE_PIN, GPIO_INPUT_PULLUP);
    return true;
}

/// \brief      Handler for the `led` command
bool exec_led(const char* command)
{
    // Abort if the command is malformed
    if (strlen(command) != 7 || command[3] != ' ' || command[5] != ' ') {
        usb_puts(PSTR("Malformed command" USB_NEWLINE));
        return false;
    }

    // Parse LED(s) to manipulate
    struct led* led;
    switch (command[4]) {
    case 'g':
        led = led_green;
        break;

    case 'r':
        led = led_red;
        break;

    default:
        usb_puts(PSTR("No such LED" USB_NEWLINE));
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
        usb_puts(PSTR("No such action" USB_NEWLINE));
        return false;
    }

    return true;
}
