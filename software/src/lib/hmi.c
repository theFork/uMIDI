/// \file
/// \brief  Implementation of the [uMIDI-HMI](https://github.com/haggl/uMIDI-hmi) module

/*
 * Copyright 2016 Sebastian Neuser
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

#include <stddef.h>

#include "encoder.h"
#include "gpio.h"
#include "hmi.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

/// \brief      Global HMI config struct
/// \details    Holds pointers to used GPIO headers and callbacks.
/// \see        init_hmi_module
static const struct hmi_config* hmi_config;

/// \brief      Internal reference to encoder 1
static struct encoder encoder1 = {
    .config = {
        .type = ENCODER_TYPE_4_PHASE,
    },
};

/// \brief      Internal reference to encoder 2
static struct encoder encoder2 = {
    .config = {
        .type = ENCODER_TYPE_4_PHASE,
    },
};



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void init_hmi_module(const struct hmi_config * const config)
{
    // Save pointer to configuration containing pointers to the callbacks
    hmi_config = config;

    // Initialize GPIO ports
    const struct gpio_pin* pin_pointer = &hmi_config->input_header->pin2;
    for (uint8_t i=0; i<sizeof(struct gpio_header)/sizeof(struct gpio_pin); ++i) {
        configure_gpio_pin(pin_pointer, GPIO_INPUT_PULLUP);
        ++pin_pointer;
    }
    pin_pointer = &hmi_config->output_header->pin2;
    for (uint8_t i=0; i<sizeof(struct gpio_header)/sizeof(struct gpio_pin); ++i) {
        configure_gpio_pin(pin_pointer, GPIO_OUTPUT);
        ++pin_pointer;
    }

    // Configure encoders
    encoder1.config.inputA = &hmi_config->input_header->pin5;
    encoder1.config.inputB = &hmi_config->input_header->pin7;
    encoder1.config.inputSwitch = &hmi_config->input_header->pin3;
    encoder1.config.cw_callback = hmi_config->encoder1cw_handler;
    encoder1.config.ccw_callback = hmi_config->encoder1ccw_handler;
    encoder1.config.push_callback = hmi_config->encoder1push_handler;
    init_encoder(&encoder1);

    encoder2.config.inputA = &hmi_config->input_header->pin6;
    encoder2.config.inputB = &hmi_config->input_header->pin4;
    encoder2.config.inputSwitch = &hmi_config->input_header->pin2;
    encoder2.config.cw_callback = hmi_config->encoder2cw_handler;
    encoder2.config.ccw_callback = hmi_config->encoder2ccw_handler;
    encoder2.config.push_callback = hmi_config->encoder2push_handler;
    init_encoder(&encoder2);
}

void show_bar_graph(const enum hmi_bar_graph_percentage percentage)
{
    hmi_led_pattern_t pattern = 0xff;
    pattern <<= 8 - percentage;
    show_led_pattern(pattern);
}

void show_led_pattern(const hmi_led_pattern_t pattern)
{
    set_hmi_led(HMI_LED1, pattern & _BV(7));
    set_hmi_led(HMI_LED2, pattern & _BV(6));
    set_hmi_led(HMI_LED3, pattern & _BV(5));
    set_hmi_led(HMI_LED4, pattern & _BV(4));
    set_hmi_led(HMI_LED5, pattern & _BV(3));
    set_hmi_led(HMI_LED6, pattern & _BV(2));
    set_hmi_led(HMI_LED7, pattern & _BV(1));
    set_hmi_led(HMI_LED8, pattern & _BV(0));
}

void set_hmi_led(const enum hmi_led led, const bool value)
{
    const struct gpio_pin * const pin_pointer = &hmi_config->output_header->pin2 + led;
    gpio_set(*pin_pointer, value);
}

void poll_hmi(void)
{
    // Poll switches
    if (hmi_config->button1_handler != NULL) {
        if (poll_gpio_input(hmi_config->input_header->pin9, GPIO_INPUT_PULLUP)) {
            hmi_config->button1_handler();
        }
    }

    if (hmi_config->button2_handler != NULL) {
        if (poll_gpio_input(hmi_config->input_header->pin8, GPIO_INPUT_PULLUP)) {
            hmi_config->button2_handler();
        }
    }

    // Poll encoders
    poll_encoder(&encoder1);
    poll_encoder(&encoder2);
}
