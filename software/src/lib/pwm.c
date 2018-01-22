/// \file
/// \brief  PWM configuration and service functions

/*
 * Copyright 2012-2018 Sebastian Neuser
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
#include "midi.h"
#include "pwm.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

static struct {
    struct TC1_struct * const tcc;
    const uint8_t             ccen_mask;
    register16_t * const      cmp;
    register16_t * const      cmpbuf;
    uint16_t (*cmp_from_midi)(midi_value_t value);
} PWMS[] = {
    { .tcc =         &TCC1, .ccen_mask = TC1_CCAEN_bm, .cmp = &TCC1.CCA, .cmpbuf = &TCC1.CCABUF, },
    { .tcc =         &TCC1, .ccen_mask = TC1_CCBEN_bm, .cmp = &TCC1.CCB, .cmpbuf = &TCC1.CCBBUF, },
    { .tcc = (void*) &TCD0, .ccen_mask = TC0_CCAEN_bm, .cmp = &TCD0.CCA, .cmpbuf = &TCD0.CCABUF, },
    { .tcc = (void*) &TCD0, .ccen_mask = TC0_CCBEN_bm, .cmp = &TCD0.CCB, .cmpbuf = &TCD0.CCBBUF, },
    { .tcc = (void*) &TCD0, .ccen_mask = TC0_CCCEN_bm, .cmp = &TCD0.CCC, .cmpbuf = &TCD0.CCCBUF, },
    { .tcc = (void*) &TCD0, .ccen_mask = TC0_CCDEN_bm, .cmp = &TCD0.CCD, .cmpbuf = &TCD0.CCDBUF, },
};



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void init_pwm(const enum pwm pwm, uint16_t (* const conversion_function)(midi_value_t value))
{
    // Store conversion function pointer
    PWMS[pwm].cmp_from_midi = conversion_function;

    // Do not prescale the system clock (=> 32 MHz)
    PWMS[pwm].tcc->CTRLA = TC_CLKSEL_DIV1_gc;

    // Select dual slope PWM mode and enable OC output
    PWMS[pwm].tcc->CTRLB = TC_WGMODE_DSBOTH_gc | PWMS[pwm].ccen_mask;

    // Set TOP value
    PWMS[pwm].tcc->PER = PWM_MAX_DUTY;

    // Set initial compare value to TOP
    *PWMS[pwm].cmp    = PWM_MAX_DUTY;
    *PWMS[pwm].cmpbuf = PWM_MAX_DUTY;
}

void set_pwm_duty_cycle(const enum pwm pwm, const midi_value_t duty)
{
    *PWMS[pwm].cmpbuf = PWMS[pwm].cmp_from_midi(duty);
}
