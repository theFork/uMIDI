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
 * MIDI I/O module header and interface.
 */

#ifndef _MIDI_H
#define _MIDI_H


//---------------- includes ----------------//
#include <stdbool.h>
#include <stdint.h>

#include <avr/io.h>

//---------------- constants ----------------//
// UART used for MIDI I/O
#define     MIDI_UART                   USARTE0
// MIDI baudrate
#define     BAUD_RATE                   31250

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
enum midi_state
{
    IDLE,
    NOTE_OFF,
    NOTE_ON,
    CONTROL_CHANGE,
    PROGRAM_CHANGE,
};


//---------------- functions and procedures ----------------//
static inline bool uart_is_ready(void) {
    return MIDI_UART.STATUS & USART_DREIF_bm;
}

static inline void uart_write(uint8_t data) {
    while (!uart_is_ready());
    MIDI_UART.DATA = data;
}

void initialize_midi_module(void);
void send_control_change(uint8_t, uint8_t);
void send_note_off(uint8_t);
void send_note_on(uint8_t);
void send_program_change(uint8_t);


//---------------- EOF ----------------//
#endif // _MIDI_H
