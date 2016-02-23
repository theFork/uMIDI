/// \file
/// \brief  TODO

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

#include "hmi.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

static const struct hmi_config* hmi_config;



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void init_hmi_module(const struct hmi_config * const config)
{
    // Save pointer to configuration containing pointers to the callbacks
    hmi_config = config;

    // Initialize GPIO ports
    // TODO
}

void show_bar_graph(enum hmi_bar_graph_percentage percentage)
{
    // TODO
}

void show_led_pattern(hmi_led_pattern_t pattern)
{
    // TODO
}

void set_hmi_led(enum hmi_led led, bool value)
{
    // TODO
}

void poll_hmi(void)
{
    // TODO
}
