/// \file
/// \brief  Implementation of a 3-phase rotary encoder with momentary switch

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

#include <stddef.h>
#include <util/delay.h>
#include "encoder.h"
#include "gpio.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

#define ENC_INPUT_A     gpio_get(*encoder->config.inputA)
#define ENC_INPUT_B     gpio_get(*encoder->config.inputB)
#define ENC_STATE_A     encoder->state.inputA
#define ENC_STATE_B     encoder->state.inputB



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void init_encoder(struct encoder* const encoder)
{
    // Configure GPIO pins
    configure_gpio_pin(encoder->config.inputA, GPIO_INPUT_PULLUP);
    configure_gpio_pin(encoder->config.inputB, GPIO_INPUT_PULLUP);
    configure_gpio_pin(encoder->config.inputSwitch, GPIO_INPUT_PULLUP);

    // Initialize encoder state
    encoder->state.inputA = false;
    encoder->state.inputB = false;
    encoder->state.counter = 0;
}

enum encoder_action poll_encoder(struct encoder* const encoder)
{
    // Poll switch
    if (!gpio_get(*encoder->config.inputSwitch)) {
        // De-bounce
        _delay_ms( 25 );
        while (!gpio_get(*encoder->config.inputSwitch));
        if (encoder->config.push_callback != NULL) {
            encoder->config.push_callback();
        }
        return ENCODER_ACTION_PUSH;
    }

    // Abort if the position is unchanged
    if (ENC_STATE_A == ENC_INPUT_A && ENC_STATE_B == ENC_INPUT_B) {
        return ENCODER_ACTION_NONE;
    }

    enum encoder_action action = ENCODER_ACTION_NONE;

    // Direction: clockwise
    if ( (!ENC_STATE_A && !ENC_STATE_B && !ENC_INPUT_A &&  ENC_INPUT_B) ||
         (!ENC_STATE_A &&  ENC_STATE_B &&  ENC_INPUT_A &&  ENC_INPUT_B) ||
         ( ENC_STATE_A &&  ENC_STATE_B && !ENC_INPUT_A && !ENC_INPUT_B) )
    {
        action = ENCODER_ACTION_CW;
        ++encoder->state.counter;
    }

    // Direction: counter-clockwise
    else if ( ( ENC_STATE_A &&  ENC_STATE_B && !ENC_INPUT_A &&  ENC_INPUT_B) ||
              (!ENC_STATE_A &&  ENC_STATE_B && !ENC_INPUT_A && !ENC_INPUT_B) ||
              (!ENC_STATE_A && !ENC_STATE_B &&  ENC_INPUT_A &&  ENC_INPUT_B) )
    {
        action = ENCODER_ACTION_CCW;
        --encoder->state.counter;
    }

    // Update input encoder->state variables
    ENC_STATE_A = ENC_INPUT_A;
    ENC_STATE_B = ENC_INPUT_B;

    // On reception of the third pulse in the same direction
    if (encoder->state.counter <= -3 || encoder->state.counter >= 3) {
        // Reset counter
        encoder->state.counter = 0;

        // Call back
        if (action == ENCODER_ACTION_CW && encoder->config.cw_callback != NULL) {
            encoder->config.cw_callback();
        }
        else if (action == ENCODER_ACTION_CCW && encoder->config.ccw_callback != NULL) {
            encoder->config.ccw_callback();
        }

        return action;
    }

    return ENCODER_ACTION_NONE;
}
