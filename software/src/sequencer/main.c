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

#include "lib/encoder.h"
#include "lib/gpio.h"
#include "lib/leds.h"
#include "lib/midi.h"
#include "lib/background_tasks.h"
#include "lib/system.h"

#include "config.h"
#include "sequencer.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

// Initialization and endless loop
int main( void )
{
    // Configure clock and timers
    configure_system_clock();

    // Initialize modules
    init_gpio_module(&gpio_config);
    init_leds_module();
    init_midi_module(&midi_config);
    init_encoder_module(&encoder_config);
    init_sequencer_module(&sequencer_config, sequencer_leds, sequencer_leds_size);
    init_background_tasks(high_frequency_tasks, high_frequency_tasks_size,
                                mid_frequency_tasks, mid_frequency_tasks_size,
                                low_frequency_tasks, low_frequency_tasks_size);


    // set watchdog for 128ms
    wdt_enable(WDT_PER_128CLK_gc);

    // enable interrupts
    PMIC.CTRL = PMIC_LOLVLEN_bm;
    sei();

    // Blink green LED
    blink_led(LED_GREEN, F_TASK_SLOW);

    // Main loop
    while (true) {
        process_background_tasks();
        wdt_reset();
    }

    return 0;
}
