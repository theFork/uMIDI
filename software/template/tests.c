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
 * Test suite for the uMIDI firmware.
 */

#include "gpio.h"
#include "leds.h"
#include "midi.h"
#include "pwm.h"
#include "tests.h"

#include <avr/wdt.h>
#include <util/delay.h>


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void run_test(void (*function_p)(void))
{
    (*function_p)();
    _delay_ms(100);
    wdt_reset();
}

void run_test_suite(void)
{
    // LED test cases
    run_test(&set_led_test);
    run_test(&toggle_led_test);
    run_test(&flash_led_test);
    run_test(&blink_led_test);

    // GPIO test cases
    run_test(&set_gpio_test);
    run_test(&clear_gpio_test);
    run_test(&toggle_gpio_test);
    run_test(&toggle_gpio_test);

    // MIDI test cases
    run_test(&send_program_change_test);
    run_test(&send_control_change_test);
    run_test(&send_note_on_test);
    run_test(&send_note_off_test);

    // PWM test cases
    run_test(&apply_duty_cycle_test);
}

//---------------- LED tests ----------------//
void blink_led_test(void)
{
    blink_led(LED_GREEN);
}

void flash_led_test(void)
{
    flash_led(LED_RED);
}

void set_led_test(void)
{
    set_led(LED_RED, true);
    set_led(LED_GREEN, true);
}

void toggle_led_test(void)
{
    toggle_led(LED_RED);
    toggle_led(LED_GREEN);
}

//---------------- GPIO tests ----------------//
void clear_gpio_test(void)
{
    gpio_set(gpio.header3.pin6, false);
}

void set_gpio_test(void)
{
    gpio_set(gpio.header3.pin6, true);
}

void toggle_gpio_test(void)
{
    gpio_toggle(gpio.header3.pin8);
}

//---------------- MIDI tests ----------------//
void send_control_change_test(void)
{
    send_control_change(69, 23);
}

void send_note_off_test(void)
{
    send_note_off(41);
}

void send_note_on_test(void)
{
    send_note_on(41);
}

void send_program_change_test(void)
{
    send_program_change(3);
}

//---------------- PWM tests ----------------//
void apply_duty_cycle_test(void)
{
    apply_duty_cycle(63);
}
