/// \file
/// \brief  I2C interface

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

#ifndef _I2C_H
#define _I2C_H


//---------------- includes ----------------//

#include <stdbool.h>
#include <stdint.h>

#include "gpio.h"


//---------------- constants ----------------//


//---------------- data types ----------------//

/// \brief  Configuration structure for the I2C port
struct i2c_config {
    uint32_t baudrate; ///< The baudrate for the I2C bus in [Hz].
};


//---------------- functions and procedures ----------------//

/// \brief      Configures the I2C module
/// \details    Initializes all specified GPIO ports and the clock.
/// \param      config
///                 the I2C configuration
void init_i2c_module(const struct i2c_config* config);

/// \brief      Reads a byte from the previously selected slave
/// \returns    a byte read from the bus
/// \see        i2c_start
uint8_t i2c_read_received_byte(void);

/// \brief      Sends a byte to the previously selected slave
/// \param      output
///                 the output word
/// \see        i2c_start
void i2c_write_byte(uint8_t output);

/// \brief      Initiates communication with a slave by sending a START condition.
/// \param      address
///                 the slaves bus addres in 7-bit (un-shifted) format
/// \param      read
///                 Set to `false` if you want to write data to a slave.
void i2c_start(uint8_t address, bool read);

/// \brief      Terminates a transaction by issuing a STOP condition.
void i2c_stop(void);


//---------------- EOF ----------------//
#endif // _I2C_H
