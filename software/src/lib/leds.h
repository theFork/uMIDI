/// \file
/// \brief  Handling of the on-board LEDs

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

#ifndef _LEDS_H
#define _LEDS_H


//---------------- includes ----------------//
#include <stdbool.h>
#include <avr/io.h>

#include "gpio.h"


//---------------- constants ----------------//

/// \brief  The green onboard LED
extern struct led* led_red;

/// \brief  The green onboard LED
extern struct led* led_green;

/// \brief  TODO: This macro is only for compatibility and should be removed in the future.
#define LED_GREEN   led_green

/// \brief  TODO: This macro is only for compatibility and should be removed in the future.
#define LED_RED     led_red


//---------------- data types ----------------//
/// \brief      Enumeration for the LED mode
/// \details    Depending on the mode, an LED can be turned on/off automatically or manually.
enum led_mode
{
    LED_STATIC,                     ///< The LED can be turned on/off manually
    LED_FLASH,                      ///< The LED flashes briefly and remains off
    LED_BLINK                       ///< The LED blinks once per second
};

/// \brief      Internal state of an LED
/// \details    Used by the service procedures
/// \see        led_mode
struct led_state
{
    bool            active;         ///< When the LED is on, this reads `true`
    enum led_mode   mode;           ///< The mode of the LED
    uint8_t         prescaler;      ///< Prescaler for the counter in blinking mode
    uint8_t         counter;        ///< Counter for the blinking interval
};

/// \brief      Enumeration for the on-board LEDs
/// \details    This enumeration is used to select the LED(s) to switch on/off in the service procedures.
struct led
{
    const struct gpio_pin* const pin;   ///< Gpio pin connected to an LED
    struct led_state state;             ///< Current state of the LED
};


//---------------- functions and procedures ----------------//
/// \brief      Module initialization procedure
/// \details    Writes relevant AVR GPIO configuration for the onboard LEDs
void init_leds_module(void);


/// \brief      Add an LED to the cyclic processed LEDs
/// \details    Writes relevant AVR GPIO configuration for the given LEDs and
///             adds the given pointer that contains the corresponding #gpio_pin
void register_led(struct led * const);

/// \brief      Lets an LED blink in a fixed interval
/// \details    The prescaler parameter controls the blinking frequency: Every \p prescaler th time
///             the {@link update_leds} procedure is called, the LED is toggled.
/// \param      led
///                 the LED that should blink
/// \param      prescaler
///                 controls the blinking frequency
/// \see        led
/// \see        update_leds
void blink_led(struct led* led, uint8_t prescaler);

/// \brief      Briefly flashes an LED
/// \param      led
///                 the LED that should blink
/// \see        led
void flash_led(struct led* led);

/// \brief      Enables or disables an LED
/// \param      led
///                 the LED that should be enabled/disabled
/// \param      enable
///                 `true` switches the LED on; `false` swithes off
/// \see        led
void set_led(struct led* led, bool enable);

/// \brief      Toggles an LED
/// \param      led
///                 the LED that should be toggled
/// \see        led
void toggle_led(struct led* led);

/// \brief      State machine task that updates the LEDs
/// \details    This task must be included in the state machine tasks for the blink and flash modes
///             to work.
/// \see        init_state_machine
void update_leds(void);


//---------------- EOF ----------------//
#endif // _LEDS_H
