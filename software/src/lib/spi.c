/// \file
/// \brief  SPI emulation module

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

#include "gpio.h"
#include "spi.h"


////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

/// \brief      Internal pointer to the SPI configuration
static const struct spi_config* spi_config;


////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void init_spi_module(const struct spi_config * const config)
{
    spi_config = config;

    // Configure GPIO pins
    configure_gpio_pin(spi_config->clk_pin, GPIO_OUTPUT);
    if (spi_config->miso_pin) {
        configure_gpio_pin(spi_config->miso_pin, GPIO_INPUT);
    }
    if (spi_config->mosi_pin) {
        configure_gpio_pin(spi_config->mosi_pin, GPIO_OUTPUT);
    }
    if (spi_config->ncs_pin) {
        configure_gpio_pin(spi_config->ncs_pin, GPIO_OUTPUT);
    }

    // Set initial clock state
    gpio_set(*spi_config->clk_pin, spi_config->clk_polarity);

    // Initialize chip select line
    gpio_set(*spi_config->ncs_pin, true);
}

uint16_t spi_transceive(const uint16_t output)
{
    // Allocate buffer for the result
    uint16_t read_buffer = 0;

    if (spi_config->clk_phase) {
        for (int8_t i=spi_config->word_length-1; i>=0; --i) {
            // Toggle clock
            gpio_toggle(*spi_config->clk_pin);

            // Set data line
            gpio_set(*spi_config->mosi_pin, output & _BV(i));

            // Wait
            _delay_us(1);

            // Toggle clock
            gpio_toggle(*spi_config->clk_pin);

            // Read incoming data
            read_buffer |= gpio_get(*spi_config->miso_pin) << i;

            // Wait
            _delay_us(1);
        }
    }
    else {
        for (int8_t i=spi_config->word_length-1; i>=0; --i) {
            // Set data line
            gpio_set(*spi_config->mosi_pin, output & _BV(i));

            // Wait
            _delay_us(1);

            // Toggle clock
            gpio_toggle(*spi_config->clk_pin);

            // Read incoming data
            read_buffer |= gpio_get(*spi_config->miso_pin) << i;

            // Wait
            _delay_us(1);

            // Toggle clock
            gpio_toggle(*spi_config->clk_pin);
        }
    }

    return read_buffer;
}

void spi_select(const bool select)
{
    gpio_set( *spi_config->ncs_pin, !select);
}
