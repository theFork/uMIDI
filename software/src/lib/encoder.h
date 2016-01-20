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
    ENCODER_ACTION_NONE,                ///< Nothing happened :-(
    ENCODER_ACTION_PUSH,                ///< Encoder was pushed
    ENCODER_ACTION_CW,                  ///< Encoder was rotated clockwise
    ENCODER_ACTION_CCW,                 ///< Encoder was rotated counter-clockwise
};

/// \brief      Configuration of an encoder
struct encoder_config
{
    struct gpio_pin*    inputA;         ///< Input A
    struct gpio_pin*    inputB;         ///< Input B
    struct gpio_pin*    inputSwitch;    ///< Push button

    void (*cw_callback  )(void);        ///< Callback for clockwise rotation
    void (*ccw_callback )(void);        ///< Callback for counter-clockwise rotation
    void (*push_callback)(void);        ///< Callback for the push button
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


//---------------- functions and procedures ----------------//

/// \brief      Initializes the encoder
/// \details    Saves the GPIO pins and registers the callbacks.
/// \param      config
///                 the encoder configuration
/// \see        encoder_config
void init_encoder_module(const struct encoder_config* config);

/// \brief      Polls the encoder
/// \details    Checks if the encoder was rotated or pushed and updates the saved state.
/// \return     the detected action or ENCODER_ACTION_NONE
enum encoder_action poll_encoder(void);


//---------------- EOF ----------------//
#endif // _ENCODER_H
