/// \file
/// \brief      Expression pedal implementation

/*
 * Copyright 2015 Sebastian Neuser
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

#include "lib/adc.h"
#include "lib/leds.h"
#include "lib/midi.h"

#include "config.h"
#include "expression.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

/// \brief      The latest known expression value
static uint8_t current_value = 0;



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void update_expression_value(uint16_t new_value) {
    if (new_value != current_value) {
        current_value = new_value;
        flash_led(LED_RED);
        send_control_change(69, new_value);
    }
}

void trigger_expression_conversion(void)
{
    trigger_adc(expression_conversion.channel);
}
