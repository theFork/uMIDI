/*
 * Copyright 2014,2015 Sebastian Neuser
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
 * Timer module implementation of the uMIDI firmware
*/

#include "adc.h"
#include "leds.h"
#include "pwm.h"
#include "timer.h"

#include <avr/io.h>
#include <avr/interrupt.h>


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

extern const struct adc_conversion_config expression_conversion;



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void configure_task_timer(void) {
    // Prescale clock to 500 kHz
    TCC0.CTRLA = TC_CLKSEL_DIV64_gc;

    // Set TOP value to achieve 2 kHz clock
    TCC0.PER = 249;

    // Enable timer overflow interrupt
    TCC0.INTCTRLA = TC_OVFINTLVL_LO_gc;
}



////////////////////////////////////////////////////////////////
//                    I N T E R R U P T S                     //
////////////////////////////////////////////////////////////////

ISR(TCC0_OVF_vect)
{
    // Disable interrupts
    cli();

    ////
    // High frequency tasks
    ////
    update_pwm();

    ////
    // Mid frequency tasks
    ////
    static uint16_t prescaler_fast = 0;
    ++prescaler_fast;
    if (prescaler_fast >= F_TIMER / F_TASK_MID) {
        prescaler_fast = 0;

        trigger_adc(expression_conversion.channel);
    }

    ////
    // Low frequency tasks
    ////
    static uint16_t prescaler_slow = 0;
    ++prescaler_slow;
    if (prescaler_slow >= F_TIMER / F_TASK_SLOW) {
        prescaler_slow = 0;

        update_leds();
    }

    // Enable interrupts
    sei();
}
