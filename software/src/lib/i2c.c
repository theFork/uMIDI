/// \file
/// \brief  I2C interface

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

#include <stdbool.h>
#include <stdint.h>
#include <util/delay.h>

#include "i2c.h"


////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

/// \brief      Internal pointer to the I2C configuration
// TODO: Do we really need to store this?
static const struct i2c_config* i2c_config;

/// \brief      Transmission window length (9 bit word) in [us]
// TODO: This should be computed from baudrate and word length. Also, the use of _delay_us() is
//       clumsy and stupid. We should come up with something better.
static uint16_t tx_window_length_us = 180;


////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void i2c_start(const uint8_t address, bool read)
{
    TWIC.MASTER.STATUS = TWI_MASTER_BUSSTATE_OWNER_gc;
    TWIC.MASTER.ADDR = (address << 1) + read;
    _delay_us(tx_window_length_us);
}

void i2c_stop(void)
{
    TWIC.MASTER.CTRLC = TWI_MASTER_CMD_STOP_gc;
}

void init_i2c_module(const struct i2c_config * const config)
{
    i2c_config = config;

    // Set baudrate
    TWIC.MASTER.BAUD = (F_CPU / (2 * config->baudrate)) - 5;

    // Enable master and interrupts
    TWIC.MASTER.CTRLA |= TWI_MASTER_ENABLE_bm
                      |  TWI_MASTER_INTLVL_HI_gc
                      |  TWI_MASTER_RIEN_bm
                      |  TWI_MASTER_WIEN_bm;

    // Enter idle status
    TWIC.MASTER.STATUS = TWI_MASTER_BUSSTATE_IDLE_gc;
}

uint8_t i2c_read_received_byte(void)
{
    _delay_us(tx_window_length_us);
    return TWIC.MASTER.DATA;
}

void i2c_write_byte(const uint8_t output)
{
    TWIC.MASTER.DATA = output;
    _delay_us(tx_window_length_us);
}
