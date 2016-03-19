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
//              G L O B A L   V A R I A B L E S               //
////////////////////////////////////////////////////////////////

struct led* led_red;

struct led* led_green;



////////////////////////////////////////////////////////////////
//             P R I V A T E   V A R I A B L E S              //
////////////////////////////////////////////////////////////////

///< Internal array of pointers to known leds
static struct led * leds[32] = { 0, };

///< Number of registered LEDs
static uint8_t registered_leds_count = 0;



////////////////////////////////////////////////////////////////
//                    PRIVATE FUNCTIONS                       //
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
    gpio_set(*led->pin, value);
}

/// \brief      Initializes or updates an LED's blinking mode
/// \details    Aborts if neither mode nor prescaler have changed
/// \param      led
///                 state variable of the led
/// \param      prescaler
///                 Prescaler for the blinking frequency
static void set_or_update_blinking_led_state(struct led_state * const state, uint8_t prescaler)
{
    // Abort if neither mode nor prescaler have changed
    if (state->mode == LED_BLINK && state->prescaler == prescaler) {
        return;
    }

    // Update mode and prescaler and restart counter
    state->mode = LED_BLINK;
    state->prescaler = prescaler;
    state->counter = 0;
}



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void init_leds_module(void)
{
    // Register onboard LEDs
    const struct gpio_pin green_led_pin = { .port = &PORTE, .bit = 0 };
    struct led green_led_tmp = { .pin = &green_led_pin, };
    const struct gpio_pin red_led_pin = { .port = &PORTE, .bit = 1 };
    struct led red_led_tmp = { .pin = &red_led_pin, };
    register_led(&green_led_tmp);
    register_led(&red_led_tmp);
}

void register_led(struct led * const led)
{
    // Configure AVR port
    configure_gpio_pin(led->pin, GPIO_OUTPUT);

    // Add to leds array
    leds[registered_leds_count] = led;
    ++registered_leds_count;

    // Initialize state
    led->state.active = false;
    led->state.mode = LED_STATIC;
    led->state.prescaler = 0;
    led->state.counter = 0;
}

void blink_led(struct led* const led, const uint8_t prescaler)
{
    // Set blink mode and update prescaler
    set_or_update_blinking_led_state(&led->state, prescaler);
}

void flash_led(struct led* const led)
{
    led->state.mode = LED_FLASH;
    led->state.counter = 1;
}

void flash_led_multiple(struct led * const led, const uint8_t count)
{
    led->state.mode = LED_FLASH;
    led->state.counter = count;
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

static void update_flashing_led(uint8_t index)
{
    // Divide call frequency by 5
    static uint8_t prescaler = 0;
    ++prescaler;
    if (prescaler < 5) {
        return;
    }
    prescaler = 0;

    // Enable the LED in even calls
    if (!leds[index]->state.active) {
        apply_led(leds[index], true);
    }
    // Disable the LED in odd calls
    else {
        --leds[index]->state.counter;
        apply_led(leds[index], false);

        // Reset if the required blink count was reached
        if (leds[index]->state.counter == 0) {
            leds[index]->state.mode = LED_STATIC;
        }
    }
}

void update_leds(void)
{
    // Iterate LEDs
    uint8_t i;
    for (i=0; i<registered_leds_count; ++i) {
        switch (leds[i]->state.mode) {
        case LED_FLASH:
            update_flashing_led(i);
            break;

        case LED_BLINK:
            // Increment counter
            ++leds[i]->state.counter;

            // When the counter reaches the prescaler value
            if (leds[i]->state.counter >= leds[i]->state.prescaler) {
                // Reset the counter
                leds[i]->state.counter = 0;

                // Toggle LED
                apply_led(leds[i], !leds[i]->state.active);
            }
            break;

        default:
            break;
        }
    }
}
