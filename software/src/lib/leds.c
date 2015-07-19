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
    .active     = false,
    .mode       = LED_STATIC,
    .prescaler  = 0,
    .counter    = 0
};

static struct led_state red_led =
{
    .active     = false,
    .mode       = LED_STATIC,
    .prescaler  = 0,
    .counter    = 0
};

static struct led_state *leds[LED_COUNT];



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void initialize_leds_module(void)
{
    // Configure AVR ports
    LED_PORT.DIRSET = _BV(RED_LED_BIT) | _BV(GREEN_LED_BIT);

    // Populate led map
    leds[RED_LED_BIT] = &red_led;
    leds[GREEN_LED_BIT] = &green_led;
}

static inline void apply_led(enum led pin, bool value)
{
    leds[pin]->active = value;
    if (value) {
        LED_PORT.OUTSET = _BV(pin);
    }
    else {
        LED_PORT.OUTCLR = _BV(pin);
    }
}

static void set_or_update_blinking_led_state(struct led_state *led, uint8_t prescaler)
{
    // Abort if neither mode or prescaler have changed
    if (led->mode == LED_BLINK && led->prescaler == prescaler) {
        return;
    }

    led->mode = LED_BLINK;
    led->prescaler = prescaler;
    led->counter = 0;
}

void blink_led(enum led led, uint8_t prescaler)
{
    if (led == LED_ALL) {
        set_or_update_blinking_led_state(&red_led, prescaler);
        set_or_update_blinking_led_state(&green_led, prescaler);
    }
    else {
        set_or_update_blinking_led_state(leds[led], prescaler);
    }
}

void flash_led(enum led led)
{
    if (led == LED_ALL) {
        red_led.mode = LED_FLASH;
        green_led.mode = LED_FLASH;
    }
    else {
        leds[led]->mode = LED_FLASH;
    }
}

void set_led(enum led led, bool value)
{
    if (led == LED_ALL) {
        green_led.mode = LED_STATIC;
        apply_led(LED_GREEN, value);
        red_led.mode = LED_STATIC;
        apply_led(LED_RED, value);
    }
    else {
        leds[led]->mode = LED_STATIC;
        apply_led(led, value);
    }
}

void toggle_led(enum led led)
{
    if (led == LED_ALL) {
        green_led.mode = LED_STATIC;
        apply_led(LED_GREEN, !green_led.active);
        red_led.mode = LED_STATIC;
        apply_led(LED_RED, !red_led.active);
    }
    else {
        leds[led]->mode = LED_STATIC;
        apply_led(led, !leds[led]->active);
    }
}

void update_leds(void)
{
    // Iterate LEDs
    uint8_t i;
    for (i=0; i<LED_COUNT; i++) {
        switch (leds[i]->mode) {
        case LED_FLASH:
            if (leds[i]->active) {
                leds[i]->mode = LED_STATIC;
                apply_led(i, false);
            }
            else {
                apply_led(i, true);
            }
            break;

        case LED_BLINK:
            ++leds[i]->counter;
            if (leds[i]->counter >= leds[i]->prescaler) {
                leds[i]->counter = 0;

                apply_led(i, !leds[i]->active);
            }
            break;

        default:
            break;
        }
    }
}
