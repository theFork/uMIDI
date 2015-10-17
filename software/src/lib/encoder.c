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

/*
 * Encoder port configuration and procedures
*/

#include <stddef.h>
#include <util/delay.h>
#include "encoder.h"
#include "gpio.h"
#include "leds.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

struct encoder_state state = {
    false,
    false,
    0
};

struct gpio_pin* inputA;
struct gpio_pin* inputB;
struct gpio_pin* inputSwitch;

static void (*cw_callback)(void);
static void (*ccw_callback)(void);
static void (*push_callback)(void);


////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void initialize_encoder_module(struct encoder_config* config)
{
    inputA = config->inputA;
    inputB = config->inputB;
    inputSwitch = config->inputSwitch;
    cw_callback = config->cw_callback;
    ccw_callback = config->ccw_callback;
    push_callback = config->push_callback;
}

void poll_encoder(void)
{
    // TODO Determine direction and update counter
    uint8_t action = ENCODER_ACTION_NONE;

    // Poll switch
    if (!gpio_get(*inputSwitch) && push_callback != NULL) {
        // De-bounce
        _delay_ms( 25 );
        while (!gpio_get(*inputSwitch));
        action = ENCODER_ACTION_PUSH;
        push_callback();
    }

    // Abort if the position is unchanged
    if (state.inputA == gpio_get(*inputA) && state.inputB == gpio_get(*inputB)) {
        return;
    }

    // Direction: counter-clockwise
    if ( ( state.inputA &&  state.inputB && !gpio_get(*inputA) &&  gpio_get(*inputB)) ||
         (!state.inputA &&  state.inputB && !gpio_get(*inputA) && !gpio_get(*inputB)) ||
         (!state.inputA && !state.inputB &&  gpio_get(*inputA) &&  gpio_get(*inputB)) )
    {
        action = ENCODER_ACTION_CCW;
        state.counter--;
    }
    // Direction: clockwise
    else if ( (!state.inputA && !state.inputB && !gpio_get(*inputA) &&  gpio_get(*inputB)) ||
              (!state.inputA &&  state.inputB &&  gpio_get(*inputA) &&  gpio_get(*inputA)) ||
              ( state.inputA &&  state.inputB && !gpio_get(*inputA) && !gpio_get(*inputB)) )
    {
        action = ENCODER_ACTION_CW;
        state.counter++;
    }

    // Third impulse received
    if ( state.counter <= -2 || state.counter >= 2 ){
        // De-bounce and reinitialize state variables
        //_delay_ms( 25 );
        state.inputA = gpio_get(*inputA);
        state.inputB = gpio_get(*inputB);
        state.counter = 0;

        // Call back
        if (action == ENCODER_ACTION_CW) {
            if (cw_callback != NULL) cw_callback();
        }
        else if (action == ENCODER_ACTION_CCW) {
            if (cw_callback != NULL) ccw_callback();
        }
    }

    // Just update state variables
    state.inputA = gpio_get(*inputA);
    state.inputB = gpio_get(*inputB);
}
