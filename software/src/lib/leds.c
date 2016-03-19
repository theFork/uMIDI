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

#include "gpio.h"
#include "leds.h"

#include <avr/interrupt.h>


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

struct led led_red = {
       .pin = { .port = &PORTE, .bit = 0 },
       .state = { 0, } // Static, off
};

struct led led_green = {
       .pin = { .port = &PORTE, .bit = 1 },
       .state = { 0, } // Static, off
};

///< Internal array of pointers to known leds
static struct led * leds[32] = {&led_red, &led_green, };

///< Number of registered LEDs -1
static uint8_t last_registered_led_index = 1;



////////////////////////////////////////////////////////////////
//                    PRIVATE DEFINES                         //
////////////////////////////////////////////////////////////////


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
    // Update state varable
    led->state.active = value;

    // Write GPIO output register
    gpio_set(led->pin, value);
}

/// \brief      Initializes or updates an LED's blinking mode
/// \details    Aborts if neither mode nor prescaler have changed
/// \param      led
///                 state variable of the led
/// \param      prescaler
///                 Prescaler for the blinking frequency
static void set_or_update_blinking_led_state(struct led_state * const led, uint8_t prescaler)
{
    // Abort if neither mode nor prescaler have changed
    if (led->mode == LED_BLINK && led->prescaler == prescaler) {
        return;
    }

    // Update mode and prescaler and restart counter
    led->mode = LED_BLINK;
    led->prescaler = prescaler;
    led->counter = 0;
}



void init_leds_module(void)
{
    // Configure AVR ports for the onboard LEDs
    configure_gpio_pin(&led_red.pin, GPIO_OUTPUT);
    configure_gpio_pin(&led_green.pin, GPIO_OUTPUT);
}

void register_led(struct led * const led)
{
    // Configure AVR port
    configure_gpio_pin(&led->pin, GPIO_OUTPUT);

    // Add to leds array
    ++last_registered_led_index;
    leds[last_registered_led_index] = led;
}

void blink_led(struct led* const led, const uint8_t prescaler)
{
    // Set blink mode and update prescaler
    set_or_update_blinking_led_state(&led->state, prescaler);
}

void flash_led(struct led* const led)
{
    led->state.mode = LED_FLASH;
}

void set_led(struct led* const led, const bool value)
{
    led->state.mode = LED_STATIC;
    apply_led(led, value);
}

void toggle_led(struct led* const led)
{
    led->state.mode = LED_STATIC;
    apply_led(led, !led->state.active);
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
