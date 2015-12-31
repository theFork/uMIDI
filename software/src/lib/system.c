/// \file
/// \brief  System configuration procedures

/*
 * Copyright 2015 Sebastian Neuser
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

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include "system.h"
#include "leds.h"


////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void configure_system_clock(void)
{
    // Enable internal 32 MHz oscillator
    OSC.CTRL |= OSC_RC32MEN_bm;

    while ((OSC.STATUS & OSC_RC32MRDY_bm) == 0);

    // Select internal 32 MHz oscillator
    CCP = CCP_IOREG_gc;
    CLK.CTRL = CLK_SCLKSEL_RC32M_gc;
}

void enable_usb_pll(void)
{
    // Set the multiplication factor and clock reference for the PLL.
    // The USB clock runs at 48 MHz.
    // 32 MHz / 4 = 4 MHz, 48 MHz / 4 MHz = 6 (factor).
    OSC.PLLCTRL = (OSC_PLLSRC_RC32M_gc | (6 << OSC_PLLFAC_gp));

    // Enable and wait for PLL
    OSC.CTRL |= OSC_PLLEN_bm;
    while ((OSC.STATUS & OSC_PLLRDY_bm) == 0);
}

void panic(uint16_t delay_red_ms, uint16_t delay_green_ms)
{
    cli();
    wdt_disable();
    init_leds_module();

    uint16_t cnt_rd = 0;
    uint16_t cnt_gn = 0;
    for(;;) {
        _delay_ms(1);
        ++cnt_rd;
        ++cnt_gn;

        if (delay_red_ms != 0 && cnt_rd == delay_red_ms) {
            cnt_rd = 0;
            toggle_led(LED_RED);
        }
        if (delay_green_ms != 0 && cnt_gn == delay_green_ms) {
            cnt_gn = 0;
            toggle_led(LED_GREEN);
        }
    }
}

void wdt_reenable(void)
{
    wdt_enable(WDT_PER_128CLK_gc);
}
