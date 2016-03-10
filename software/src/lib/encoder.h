/// \file
/// \brief  Interface for rotary encoders

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
 * Header for encoders
*/

#ifndef _ENCODER_H
#define _ENCODER_H


//---------------- includes ----------------//
#include <stdbool.h>
#include "gpio.h"


//---------------- data types ----------------//

/// \brief      An encoder action
enum encoder_action
{
    ENCODER_ACTION_NONE,                ///< :-( Nothing happened
    ENCODER_ACTION_PUSH,                ///< Encoder was pushed
    ENCODER_ACTION_CW,                  ///< Encoder was rotated clockwise
    ENCODER_ACTION_CCW,                 ///< Encoder was rotated counter-clockwise
};

/// \brief      Valid encoder types
enum encoder_type
{
    ENCODER_TYPE_3_PHASE = 3,           ///< 3-phase rotary encoder
    ENCODER_TYPE_4_PHASE = 4,           ///< 4-phase rotary encoder
};

/// \brief      Configuration of an encoder
struct encoder_config
{
    const struct gpio_pin* inputA;      ///< The GPIO pin that input A is connected to
    const struct gpio_pin* inputB;      ///< The GPIO pin that input A is connected to
    const struct gpio_pin* inputSwitch; ///< The GPIO pin that the push button is connected to.
                                        ///< Set this to `NULL` if your encoder does not have a
                                        ///< momentary switch.

    const enum encoder_type type;       ///< Encoder type (# of phases)

    void (*cw_callback  )(void);        ///< Callback for clockwise rotation or `NULL`
    void (*ccw_callback )(void);        ///< Callback for counter-clockwise rotation or `NULL`
    void (*push_callback)(void);        ///< Callback for the push button or `NULL`
};

/// \brief      Internal state of an encoder
/// \details    The state is used to determine direction of the rotation.
/// \see        poll_encoder
struct encoder_state
{
    bool                inputA;         ///< Input A state
    bool                inputB;         ///< Input B state
    int8_t              counter;        ///< Pulse counter
};

/// \brief      An encoder instance
struct encoder
{
    struct encoder_config   config;     ///< Encoder configuration
    struct encoder_state    state;      ///< Encoder state
};


//---------------- functions and procedures ----------------//

/// \brief      Initializes the encoder
/// \details    Configures GPIO pins and registers the callbacks.
/// \param      encoder
///                 the encoder instance to initialize
/// \see        encoder_config
void init_encoder(struct encoder* encoder);

/// \brief      Polls the encoder inputs
/// \details    Checks if the encoder was rotated or pushed and updates the saved state. This
///             function also executes registered callbacks, so you can define those in the
///             encoder configuration passed to #init_encoder and register this function in a
///             background task, discarding its return value.
/// \param      encoder
///                 the encoder to poll
/// \return     the detected action or ENCODER_ACTION_NONE
enum encoder_action poll_encoder(struct encoder* encoder);


//---------------- EOF ----------------//
#endif // _ENCODER_H
