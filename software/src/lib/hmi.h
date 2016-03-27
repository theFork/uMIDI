/// \file
/// \brief      API of the [uMIDI-HMI](https://github.com/haggl/uMIDI-hmi) module
/// \details    To use the uMIDI HMI, please follow these steps:
///             1. Connect uMIDI to the HMI board.
///             2. Define event handlers for the HMI inputs.
///             3. Define a #hmi_config and map your event handlers to the callbacks.
///             4. Call #init_hmi_module providing it with your #hmi_config.
///             5. Register #poll_hmi as a mid-frequency background task.

/*
 * Copyright 2016 Sebastian Neuser
 *
 * This file is part of the uMIDI firmware.
 *
 * the uMIDI firmware is free software: you can redistribute it and/or modify
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

#ifndef _HMI_H
#define _HMI_H


//---------------- includes ----------------//
#include <stdbool.h>
#include <stdint.h>

#include "gpio.h"


//---------------- constants ----------------//


//---------------- data types ----------------//
/// \brief      A bitmask that specifies an LED pattern to display
/// \details    Counting from LSB to MSB: 1 &rarr; LED on; 0 &rarr; LED off
typedef uint8_t hmi_led_pattern_t;

/// \brief      Enumeration for the available LEDs on the HMI board
/// \details    Provides a pin mapping to the GPIO header. The values can be
///             used to get a pointer to a specific LED output pin "by name":
///             \code
///                 const struct gpio_pin * const pin_pointer = &hmi_config->output_header->pin2 + HMI_LED3;
///             \endcode
enum hmi_led
{
    HMI_LED1 = 1,           ///< LED1
    HMI_LED2 = 3,           ///< LED2
    HMI_LED3 = 5,           ///< LED3
    HMI_LED4 = 7,           ///< LED4
    HMI_LED5 = 6,           ///< LED5
    HMI_LED6 = 4,           ///< LED6
    HMI_LED7 = 2,           ///< LED7
    HMI_LED8 = 0            ///< LED8
};

/// \brief      Enumeration for possible percentages to display
enum hmi_bar_graph_percentage
{
    HMI_BAR_000_PERCENT,    ///< the bar graph is empty
    HMI_BAR_013_PERCENT,    ///< the bar graph shows 1/8
    HMI_BAR_025_PERCENT,    ///< the bar graph shows 2/8
    HMI_BAR_038_PERCENT,    ///< the bar graph shows 3/8
    HMI_BAR_050_PERCENT,    ///< the bar graph shows 4/8
    HMI_BAR_063_PERCENT,    ///< the bar graph shows 5/8
    HMI_BAR_075_PERCENT,    ///< the bar graph shows 6/8
    HMI_BAR_088_PERCENT,    ///< the bar graph shows 7/8
    HMI_BAR_100_PERCENT     ///< the bar graph is full
};

/// \brief      Configuration structure for the HMI module
struct hmi_config
{
    const struct gpio_header* input_header;     ///< the pin header that is connected to the HMI board's X2 header
    const struct gpio_header* output_header;    ///< the pin header that is connected to the HMI board's X1 header
    uint8_t long_input_threashold;              ///< the time in [s/10] after which a button press is considered "long"
    void (* button1_short_handler)(void);       ///< this handler is called when button 1 is pressed briefly
    void (* button1_long_handler)(void);        ///< this handler is called when button 1 is pressed long
    void (* button2_short_handler)(void);       ///< this handler is called when button 2 is pressed briefly
    void (* button2_long_handler)(void);        ///< this handler is called when button 2 is pressed long
    void (* encoder1cw_handler)(void);          ///< this handler is called when encoder 1 is rotated clockwise
    void (* encoder1ccw_handler)(void);         ///< this handler is called when encoder 1 is rotated counter-clockwise
    void (* encoder1push_handler)(void);        ///< this handler is called when the switch in encoder 1 is pressed
    void (* encoder2cw_handler)(void);          ///< this handler is called when encoder 2 is rotated clockwise
    void (* encoder2ccw_handler)(void);         ///< this handler is called when encoder 2 is rotated counter-clockwise
    void (* encoder2push_handler)(void);        ///< this handler is called when the switch in encoder 2 is pressed
};


//---------------- functions and procedures ----------------//

/// \brief      Initializes the HMI module according to the provided configuration
/// \details    Registers event callbacks and configures GPIO ports used by the HMI module.
/// \param      config
///                 the HMI module configuration
void init_hmi_module(const struct hmi_config* config);

/// \brief      Uses the HMI's LEDs to display a nice "bar graph"
/// \param      percentage
///                 the percentage to display
void show_bar_graph(enum hmi_bar_graph_percentage percentage);

/// \brief      Displays the specified pattern on the HMI's LEDs
/// \param      pattern
///                 the pattern to show
/// \see        hmi_led_pattern_t
void show_led_pattern(hmi_led_pattern_t pattern);

/// \brief      Turns a specific LED on or off
/// \param      led
///                 the LED to control
/// \param      value
///                 true &rarr; LED on; `false` &rarr; LED off
void set_hmi_led(enum hmi_led led, bool value);

/// \brief      Polls the HMI inputs and executes registered callbacks if an event occurred
/// \details    This procedure must be called in a slow background task to be able to process input
///             events from the HMI board.
void poll_hmi(void);


//---------------- EOF ----------------//
#endif // _HMI_H
