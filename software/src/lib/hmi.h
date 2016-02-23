/// \file
/// \brief  TODO

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
/// \brief      TODO
typedef uint8_t hmi_led_pattern_t;

/// \brief      TODO
enum hmi_led
{
    HMI_LED1,               ///< TODO
    HMI_LED2,               ///< TODO
    HMI_LED3,               ///< TODO
    HMI_LED4,               ///< TODO
    HMI_LED5,               ///< TODO
    HMI_LED6,               ///< TODO
    HMI_LED7,               ///< TODO
    HMI_LED8                ///< TODO
};

/// \brief      TODO
enum hmi_bar_graph_percentage
{
    HMI_BAR_000_PERCENT,    ///< TODO
    HMI_BAR_013_PERCENT,    ///< TODO
    HMI_BAR_025_PERCENT,    ///< TODO
    HMI_BAR_038_PERCENT,    ///< TODO
    HMI_BAR_050_PERCENT,    ///< TODO
    HMI_BAR_063_PERCENT,    ///< TODO
    HMI_BAR_075_PERCENT,    ///< TODO
    HMI_BAR_100_PERCENT     ///< TODO
};

/// \brief      Configuration structure for the HMI module
struct hmi_config
{
    const struct gpio_header* input_header;     ///< the pin header that is connected to the HMI board's X2 header
    const struct gpio_header* output_header;    ///< the pin header that is connected to the HMI board's X1 header
    void (* button1_handler)(void);             ///< this handler is called when button 1 is pressed
    void (* button2_handler)(void);             ///< this handler is called when button 2 is pressed
    void (* encoder1cw_handler)(void);          ///< this handler is called when encoder 1 is rotated clockwise
    void (* encoder1ccw_handler)(void);         ///< this handler is called when encoder 1 is rotated counter-clockwise
    void (* encoder1push_handler)(void);        ///< this handler is called when the switch in encoder 1 is pressed
    void (* encoder2cw_handler)(void);          ///< this handler is called when encoder 2 is rotated clockwise
    void (* encoder2ccw_handler)(void);         ///< this handler is called when encoder 2 is rotated counter-clockwise
    void (* encoder2push_handler)(void);        ///< this handler is called when the switch in encoder 2 is pressed
};


//---------------- functions and procedures ----------------//

/// \brief      Initializes the HMI module according to the provided configuration
/// \details    TODO
/// \param      config
///                 the HMI module configuration
void init_hmi_module(const struct hmi_config* config);

/// \brief      Uses the HMI's LEDs to display a nice "bar graph"
/// \param      percentage
///                 the percentage to display
void show_bar_graph(enum hmi_bar_graph_percentage percentage);

/// \brief      Displays the specified pattern on the HMI's LEDs
/// \param      pattern
///                 the pattern to show TODO
/// \see        TODO
void show_led_pattern(hmi_led_pattern_t pattern);

/// \brief      Uses the HMI's LEDs to display a nice "bar graph"
/// \param      percentage
///                 the percentage to display
void set_hmi_led(enum hmi_led led, bool value);

/// \brief      Polls the HMI inputs and executes registered callbacks if an event occurred
/// \details    TODO
void poll_hmi(void);


//---------------- EOF ----------------//
#endif // _HMI_H
