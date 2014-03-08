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
 * MIDI I/O module header and interface.
 */

#ifndef _MIDI_H
#define _MIDI_H


//---------------- includes ----------------//


//---------------- constants ----------------//
// MIDI baudrate
#define     BAUD                        31250

// maximum MIDI value
#define     MIDI_MAX_VALUE              127

// MIDI transmit channel
#define     MIDI_TX_CHANNEL             0x01

// MIDI status byte
#define     MIDI_COMMAND_MASK           0xf0
#define     MIDI_CONTROL_CHANGE         0xb0
#define     MIDI_NOTE_OFF               0x80
#define     MIDI_NOTE_ON                0x90
#define     MIDI_PROGRAM_CHANGE         0xc0

// MIDI data byte 0 for NOTE ON commands
#define     CMD_PEDAL_MODE              0x00

// MIDI data byte 0 for CONTROL CHANGE commands
#define     CTRL_CUTOFF                 0x45


//---------------- data types ----------------//
// codes for MIDI state machine
typedef enum {
    IDLE,
    NOTE_OFF,
    NOTE_ON,
    CONTROL_CHANGE,
    PROGRAM_CHANGE,
} midi_state_t;


//---------------- functions and procedures ----------------//
void configureUSART(void);
void sendCC(uint8_t, uint8_t);
void sendPC(uint8_t);


//---------------- EOF ----------------//
#endif // _MIDI_H
