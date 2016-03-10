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

#include "encoder.h"
#include "gpio.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

/// \brief      Interprets encoder inputs and updates the supplied encoder state
/// \details    This helper function implements a 3-phase rotary encoder "protocol".
/// \param      state
///                 the state of the encoder
/// \param      inputA
///                 the current input on the encoder's A terminal
/// \param      inputB
///                 the current input on the encoder's B terminal
/// \returns    the direction the encoder was rotated in or ENCODER_ACTION_NONE
static enum encoder_action advance_3phase_encoder(struct encoder_state* const state, const bool inputA, const bool inputB)
{
    enum encoder_action action = ENCODER_ACTION_NONE;
    // Direction: clockwise
    if ( (!state->inputA && !state->inputB && !inputA &&  inputB) ||
         (!state->inputA &&  state->inputB &&  inputA &&  inputB) ||
         ( state->inputA &&  state->inputB && !inputA && !inputB) )
    {
        action = ENCODER_ACTION_CW;
        ++state->counter;
    }

    // Direction: counter-clockwise
    else if ( ( state->inputA &&  state->inputB && !inputA &&  inputB) ||
              (!state->inputA &&  state->inputB && !inputA && !inputB) ||
              (!state->inputA && !state->inputB &&  inputA &&  inputB) )
    {
        action = ENCODER_ACTION_CCW;
        --state->counter;
    }

    // Update input encoder->state variables
    state->inputA = inputA;
    state->inputB = inputB;

    return action;
}

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
    if (poll_gpio_input(encoder->config.inputSwitch, GPIO_INPUT_PULLUP)) {
        if (encoder->config.push_callback != NULL) {
            encoder->config.push_callback();
        }
        return ENCODER_ACTION_PUSH;
    }

    // Abort if encoder position is unchanged
    bool inputA = gpio_get(*encoder->config.inputA);
    bool inputB = gpio_get(*encoder->config.inputB);
    if (encoder->state.inputA == inputA && encoder->state.inputB == inputB) {
        return ENCODER_ACTION_NONE;
    }

    enum encoder_action action;
    action = advance_3phase_encoder(&encoder->state, inputA, inputB);

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
