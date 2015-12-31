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
#include "leds.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

/// \brief      Internal state used by poll_encoder
static struct encoder_state state = {
    false,
    false,
    0
};

/// \brief      The GPIO pin that output A of the encoder is connected to
static struct gpio_pin* inputA;

/// \brief      The GPIO pin that output B of the encoder is connected to
static struct gpio_pin* inputB;

/// \brief      The GPIO pin that the momentary switch is connected to
static struct gpio_pin* inputSwitch;

/// \brief      Callback for clockwise rotation
static void (*cw_callback)(void);

/// \brief      Callback for counter-clockwise rotation
static void (*ccw_callback)(void);

/// \brief      Callback for button press
static void (*push_callback)(void);


////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void init_encoder_module(struct encoder_config* config)
{
    inputA = config->inputA;
    inputB = config->inputB;
    inputSwitch = config->inputSwitch;
    cw_callback = config->cw_callback;
    ccw_callback = config->ccw_callback;
    push_callback = config->push_callback;
}

enum encoder_action poll_encoder(void)
{
    // Poll switch
    if (!gpio_get(*inputSwitch) && push_callback != NULL) {
        // De-bounce
        _delay_ms( 25 );
        while (!gpio_get(*inputSwitch));
        push_callback();
        return ENCODER_ACTION_PUSH;
    }

    // Abort if the position is unchanged
    if (state.inputA == gpio_get(*inputA) && state.inputB == gpio_get(*inputB)) {
        return ENCODER_ACTION_NONE;
    }

    enum encoder_action action = ENCODER_ACTION_NONE;

    // Direction: clockwise
    if ( (!state.inputA && !state.inputB && !gpio_get(*inputA) &&  gpio_get(*inputB)) ||
              (!state.inputA &&  state.inputB &&  gpio_get(*inputA) &&  gpio_get(*inputA)) ||
              ( state.inputA &&  state.inputB && !gpio_get(*inputA) && !gpio_get(*inputB)) )
    {
        action = ENCODER_ACTION_CW;
        ++state.counter;
    }

    // Direction: counter-clockwise
    else if ( ( state.inputA &&  state.inputB && !gpio_get(*inputA) &&  gpio_get(*inputB)) ||
         (!state.inputA &&  state.inputB && !gpio_get(*inputA) && !gpio_get(*inputB)) ||
         (!state.inputA && !state.inputB &&  gpio_get(*inputA) &&  gpio_get(*inputB)) )
    {
        action = ENCODER_ACTION_CCW;
        --state.counter;
    }

    // Update input state variables
    state.inputA = gpio_get(*inputA);
    state.inputB = gpio_get(*inputB);

    // On reception of the third pulse in the same direction
    if (state.counter <= -3 || state.counter >= 3) {
        // Reset counter
        state.counter = 0;

        // Call back
        if (action == ENCODER_ACTION_CW && cw_callback != NULL) {
            cw_callback();
        }
        else if (action == ENCODER_ACTION_CCW && ccw_callback != NULL) {
            ccw_callback();
        }

        return action;
    }

    return ENCODER_ACTION_NONE;
}
