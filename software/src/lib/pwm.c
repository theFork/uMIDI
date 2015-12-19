/// \file
/// \brief  PWM configuration and service functions

/*
 * Copyright 2012-2015 Sebastian Neuser
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
#include <avr/io.h>
#include <avr/interrupt.h>

#include "gpio.h"
#include "lookup_tables.h"
#include "math.h"
#include "pwm.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////


static uint16_t (*convert_pwm_range)(uint8_t);

////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void initialize_pwm_module(uint16_t (*conversion_function)(uint8_t))
{
    // Store conversion function pointer
    convert_pwm_range = conversion_function;

    // Do not prescale the system clock (=> 32 MHz)
    TCC1.CTRLA = TC_CLKSEL_DIV1_gc;

    // Select dual slope PWM mode and enable OC1A output
    TCC1.CTRLB = TC_WGMODE_DSBOTH_gc | TC1_CCAEN_bm;

    // Set TOP value
    TCC1.PER = (1<<lookup_table_resolution) - 1;

    // Set initial compare value to TOP
    TCC1.CCA = TCC1.PER;
}

void set_pwm_duty_cycle(uint8_t duty)
{
    TCC1.CCABUF = convert_pwm_range(duty);
}
