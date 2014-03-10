/*
 * Copyright 2012-2014 Sebastian Neuser
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

#include "midi.h"
#include "gpio.h"
#include "pwm.h"
#include "tests.h"

#include <avr/wdt.h>
#include <util/delay.h>


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

extern gpio_t gpio;



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void runTest(void (*function_p)(void))
{
    (*function_p)();
    _delay_ms(100);
    wdt_reset();
}

void runTestSuite(void)
{
    // GPIO test cases
    runTest(&setGPIOsTest);
    runTest(&clearGPIOsTest);
    runTest(&toggleGPIOsTest);
    runTest(&toggleGPIOsTest);

    // MIDI test cases
    runTest(&sendProgramChangeTest);
    runTest(&sendControlChangeTest);
    runTest(&sendNoteOnTest);
    runTest(&sendNoteOffTest);

    // PWM test cases
    runTest(&applyDutyCycleTest);
}

//---------------- GPIO tests ----------------//
void clearGPIOsTest(void)
{
    gpio_set(gpio.portC.pin4, false);
    gpio_set(gpio.portC.pin6, false);
}

void setGPIOsTest(void)
{
    gpio_set(gpio.portC.pin4, true);
    gpio_set(gpio.portC.pin6, true);
}

void toggleGPIOsTest(void)
{
    gpio_toggle(gpio.portC.pin4);
    gpio_toggle(gpio.portC.pin6);
}

//---------------- MIDI tests ----------------//
void sendControlChangeTest(void)
{
    sendControlChange(69, 23);
}

void sendNoteOffTest(void)
{
    sendNoteOff(41);
}

void sendNoteOnTest(void)
{
    sendNoteOn(41);
}

void sendProgramChangeTest(void)
{
    sendProgramChange(3);
}

//---------------- PWM tests ----------------//
void applyDutyCycleTest(void)
{
    applyDutyCycle(63);
}
