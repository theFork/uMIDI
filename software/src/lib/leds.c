/// \file /// \brief  Handling of the on-board LEDs

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

#include "leds.h"

#include <avr/interrupt.h>


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

const struct led led_red = {
       .pin = { .port = &PORTE, .bit = 0 },
       .state = { 0, } // Static, off
};

const struct led led_green = {
       .pin = { .port = &PORTE, .bit = 1 },
       .state = { 0, } // Static, off
};



////////////////////////////////////////////////////////////////
//                    PRIVATE DEFINES                         //
////////////////////////////////////////////////////////////////

/// \brief  The AVR port the LEDs are connected to
#define     LED_PORT                PORTE

/// \brief  The bit number in GPIO registers controlling the red LED
#define     RED_LED_BIT             0

/// \brief  The bit number in GPIO registers controlling the green LED
#define     GREEN_LED_BIT           1



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

/// \brief          Enables or disables an LED by writing the corresponding GPIO output register
/// \param      pin
///                 the LED to enable / disable
/// \param      value
///                 `true` enables the LED; `false` disables it
static inline void apply_led(struct led * const led , bool value)
{
    /* TODO
    // Update state varable
    leds[pin]->active = value;

    // Write GPIO output register
    if (value) {
        LED_PORT.OUTSET = _BV(pin);
    }
    else {
        LED_PORT.OUTCLR = _BV(pin);
    }
    */
}

/// \brief      Initializes or updates an LED's blinking mode
/// \details    Aborts if neither mode nor prescaler have changed
/// \param      led
///                 state variable of the led
/// \param      prescaler
///                 Prescaler for the blinking frequency
static void set_or_update_blinking_led_state(struct led_state * const led, uint8_t prescaler)
{
    /* TODO
    // Abort if neither mode nor prescaler have changed
    if (led->mode == LED_BLINK && led->prescaler == prescaler) {
        return;
    }

    // Update mode and prescaler and restart counter
    l   ed->mode = LED_BLINK;
    led->prescaler = prescaler;
    led->counter = 0;
    */
}

void init_leds_module(void)
{
    // Configure AVR ports
    LED_PORT.DIRSET = _BV(RED_LED_BIT) | _BV(GREEN_LED_BIT);
}

void blink_led(struct led* const led, const uint8_t prescaler)
{
    // Set blink mode and update prescaler
    /*
    if (led == LED_ALL) {
        set_or_update_blinking_led_state(&red_led, prescaler);
        set_or_update_blinking_led_state(&green_led, prescaler);
    }
    else {
        set_or_update_blinking_led_state(leds[led], prescaler);
    }
    */
}

void flash_led(struct led* const led)
{
    // TODO
    // leds[led]->mode = LED_FLASH;
}

void set_led(struct led* const led, const bool value)
{
    // leds[led]->mode = LED_STATIC;
    // apply_led(led, value);
}

void toggle_led(struct led* const led)
{
    // leds[led]->mode = LED_STATIC;
    // apply_led(led, !leds[led]->active);
}

void update_leds(void)
{
    /* TODO
    // Iterate LEDs TODO
    uint8_t i;
    for (i=0; i<LED_COUNT; i++) {
        switch (leds[i]->mode) {
        case LED_FLASH:
            // Enable the LED in the first call
            if (!leds[i]->active) {
                apply_led(i, true);
            }
            // Disable the LED in the second call
            else {
                leds[i]->mode = LED_STATIC;
                apply_led(i, false);
            }
            break;

        case LED_BLINK:
            // Increment counter
            ++leds[i]->counter;

            // When the counter reaches the prescaler value
            if (leds[i]->counter >= leds[i]->prescaler) {
                // Reset the counter
                leds[i]->counter = 0;

                // Toggle LED
                apply_led(i, !leds[i]->active);
            }
            break;

        default:
            break;
        }
    }
    */
}
