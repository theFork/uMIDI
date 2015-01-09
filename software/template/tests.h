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
 * Header for the uMIDI firmware test suite.
 */

#ifndef _TESTS_H
#define _TESTS_H


//---------------- includes ----------------//


//---------------- constants ----------------//


//---------------- data types ----------------//


//---------------- functions and procedures ----------------//
void runTest(void (*)(void));
void runTestSuite(void);

// GPIO tests
void clearGPIOsTest(void);
void setGPIOsTest(void);
void toggleGPIOsTest(void);

// MIDI TX tests
void sendControlChangeTest(void);
void sendNoteOffTest(void);
void sendNoteOnTest(void);
void sendProgramChangeTest(void);

// PWM tests
void applyDutyCycleTest(void);


//---------------- EOF ----------------//
#endif // _TESTS_H
