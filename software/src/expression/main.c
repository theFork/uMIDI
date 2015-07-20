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

/*
 * Program entry point and main loop of the uMIDI firmware.
 */

#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "lib/adc.h"
#include "lib/gpio.h"
#include "lib/leds.h"
#include "lib/midi.h"
#include "lib/state_machine.h"

#include "config.h"
#include "expression.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

static void configure_system_clock(void)
{
    // Enable internal 32 MHz oscillator
    OSC.CTRL |= OSC_RC32MEN_bm;
    while(!(OSC.STATUS & OSC_RC32MRDY_bm));

    // Select internal 32 MHz oscillator
    CCP = CCP_IOREG_gc;
    CLK.CTRL = CLK_SCLKSEL_RC32M_gc;
}

// Initialization and endless loop
int main( void )
{
    // Configure clock and timers
    configure_system_clock();

    // Initialize modules
    initialize_leds_module();
    initialize_gpio_module(&gpio_config);
    initialize_midi_module(&midi_event_handlers);
    initialize_adc_module(&adc_config, &expression_conversion);
    initialize_state_machine(high_frequency_tasks, high_frequency_tasks_size,
                             mid_frequency_tasks, mid_frequency_tasks_size,
                             low_frequency_tasks, low_frequency_tasks_size);

    // set watchdog for 128ms
    wdt_enable(WDT_PER_128CLK_gc);

    // enable interrupts
    PMIC.CTRL = PMIC_LOLVLEN_bm;
    sei();

    // Enable the ADC interrupt on completion of a conversion
    enable_adc_interrupt(expression_conversion.channel);

    // Blink green LED
    blink_led(LED_GREEN, F_TASK_SLOW);

    // Main loop
    while (true) {
        handle_state_machine();
        wdt_reset();
    }

    return 0;
}
