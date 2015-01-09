/*
 * Copyright 2015 Sebastian Neuser
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

/*
 * LED GPIO port configuration and procedures
*/

#include "leds.h"

#include <avr/interrupt.h>


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

static struct led_state green_led =
{
    .active = false,
    .mode = LED_STATIC
};

static struct led_state red_led =
{
    .active = false,
    .mode = LED_STATIC
};

static struct led_state *leds[LED_COUNT];



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void initializeLedsModule(void)
{
    // Configure AVR ports
    LED_PORT.DIRSET = _BV(RED_LED_BIT) | _BV(GREEN_LED_BIT);

    // Populate led map
    leds[RED_LED_BIT] = &red_led;
    leds[GREEN_LED_BIT] = &green_led;
}

static inline void applyLed(enum led pin, bool value)
{
    leds[pin]->active = value;
    if (value) {
        LED_PORT.OUTSET = _BV(pin);
    }
    else {
        LED_PORT.OUTCLR = _BV(pin);
    }
}


void blinkLed(enum led led)
{
    switch (led) {
    case LED_RED:
        red_led.mode = LED_BLINK;
        break;
    case LED_GREEN:
        green_led.mode = LED_BLINK;
        break;
    case LED_ALL:
        red_led.mode = LED_BLINK;
        green_led.mode = LED_BLINK;
        break;
    default:
        break;
    }
}

void flashLed(enum led led)
{
    switch (led) {
    case LED_RED:
        red_led.mode = LED_FLASH;
        break;
    case LED_GREEN:
        green_led.mode = LED_FLASH;
        break;
    case LED_ALL:
        red_led.mode = LED_FLASH;
        green_led.mode = LED_FLASH;
        break;
    default:
        break;
    }
}

void setLed(enum led led, bool value)
{
    if (led == LED_ALL) {
        green_led.mode = LED_STATIC;
        applyLed(LED_GREEN, value);
        red_led.mode = LED_STATIC;
        applyLed(LED_RED, value);
    }
    else {
        leds[led]->mode = LED_STATIC;
        applyLed(led, value);
    }
}

void toggleLed(enum led led)
{
    if (led == LED_ALL) {
        green_led.mode = LED_STATIC;
        applyLed(LED_GREEN, !green_led.active);
        red_led.mode = LED_STATIC;
        applyLed(LED_RED, !red_led.active);
    }
    else {
        leds[led]->mode = LED_STATIC;
        applyLed(led, !leds[led]->active);
    }
}

void updateLeds(void)
{
    // Flashing
    uint8_t i;
    for (i=0; i<LED_COUNT; i++) {
        if (leds[i]->mode == LED_FLASH) {
            if (leds[i]->active) {
                leds[i]->mode = LED_STATIC;
                applyLed(i, false);
            }
            else {
                applyLed(i, true);
            }
        }
    }

    // Blinking
    static uint8_t blink_prescaler = 0;
    ++blink_prescaler;
    if (blink_prescaler >= BLINK_SPEED) {
        blink_prescaler = 0;

        for (i=0; i<LED_COUNT; i++) {
            if (leds[i]->mode == LED_BLINK) {
                applyLed(i, !leds[i]->active);
            }
        }
    }
}
