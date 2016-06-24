/// \file
/// \brief      SPI emulation module
/// \details    Note: At the moment only a single instance is supported!
///                   Also, the clock frequency is hard-coded to 500kHz.

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

#ifndef _SPI_H
#define _SPI_H


//---------------- includes ----------------//

#include <stdbool.h>
#include <stdint.h>

#include "gpio.h"


//---------------- constants ----------------//


//---------------- data types ----------------//

/// \brief  Configuration structure for the SPI emulation module
struct spi_config {
    bool                   clk_phase;     ///< Clock phase
    bool                   clk_polarity;  ///< Clock polarity
    uint8_t                word_length;   ///< Word length
    const struct gpio_pin* clk_pin;       ///< The GPIO pin connected to the clock line
    const struct gpio_pin* miso_pin;      ///< The GPIO pin connected to the MISO line (may be NULL if not present)
    const struct gpio_pin* mosi_pin;      ///< The GPIO pin connected to the MOSI line (may be NULL if not present)
    const struct gpio_pin* ncs_pin;       ///< The GPIO pin connected to the (inverted) chip select line (may be NULL)
};


//---------------- functions and procedures ----------------//

/// \brief      Configures the SPI module
/// \details    Initializes all specified GPIO ports and the clock line
/// \param      config
///                 the SPI configuration
void init_spi_module(const struct spi_config* config);

/// \brief      Sends and receives a word (full duplex)
/// \param      output
///                 the output word
/// \returns    the received word, if any
uint16_t spi_transceive(uint16_t output);

/// \brief      Controls the chip select line
/// \param      select
///                 if `true`, the line is pulled down
void spi_select(bool select);




//---------------- EOF ----------------//
#endif // _SPI_H
