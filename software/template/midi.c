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
 * MIDI I/O implementation and ISR.
 */

#include "main.h"
#include "midi.h"

#include <avr/interrupt.h>
#include <avr/io.h>


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

extern struct exec_state    state;



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void configureUSART(void)
{
    // Enable RXEN and RXC interrupt
    USARTC0.CTRLA = USART_RXCINTLVL0_bm;
    USARTC0.CTRLB = USART_RXEN_bm;

    // Set baud rate
    // 32e6 / (16 * 31250) - 1 = 63
    // F_osc       baudrate     BSEL
    USARTC0.BAUDCTRLA = 63;
    USARTC0.BAUDCTRLB = 0;
}

void sendControlChange(uint8_t controller, uint8_t value) {
    // send control change status byte
    usart_write((uint8_t) MIDI_CONTROL_CHANGE | MIDI_TX_CHANNEL);

    // send controller number
    usart_write(controller);

    // send value
    usart_write(value);
}

void sendNoteOff(uint8_t note) {
    // send note off status byte
    usart_write((uint8_t) MIDI_NOTE_OFF | MIDI_TX_CHANNEL);

    // send note number
    usart_write(note);

    // send maximum velocity
    usart_write(MIDI_MAX_VALUE);
}

void sendNoteOn(uint8_t note) {
    // send note on status byte
    usart_write((uint8_t) MIDI_NOTE_ON | MIDI_TX_CHANNEL);

    // send note number
    usart_write(note);

    // send maximum velocity
    usart_write(MIDI_MAX_VALUE);
}

void sendProgramChange(uint8_t pnum) {
    // send program change status byte
    usart_write((uint8_t) MIDI_PROGRAM_CHANGE | MIDI_TX_CHANNEL);

    // send program number
    usart_write(pnum);
}



////////////////////////////////////////////////////////////////
//                    I N T E R R U P T S                     //
////////////////////////////////////////////////////////////////

ISR(USARTC0_RXC_vect)
{
    // Disable interrupts
    cli();

    // Fetch data
    uint8_t data = USARTC0.DATA;

    switch (state.midi) {
        ////
        // MIDI status byte
        ////
        case IDLE:
            if ( (data & MIDI_COMMAND_MASK) == MIDI_NOTE_OFF )
                state.midi = NOTE_OFF;
            else if ( (data & MIDI_COMMAND_MASK) == MIDI_NOTE_ON )
                state.midi = NOTE_ON;
            else if ( (data & MIDI_COMMAND_MASK) == MIDI_CONTROL_CHANGE )
                state.midi = CONTROL_CHANGE;
            else if ( (data & MIDI_COMMAND_MASK) == MIDI_PROGRAM_CHANGE )
                state.midi = PROGRAM_CHANGE;
            break;


        ////
        // MIDI data byte 0
        ////
        case NOTE_OFF:
            switch(data) {
            }
            state.midi = IDLE;
            break;


        case NOTE_ON:
            switch(data) {
            }
            state.midi = IDLE;
            break;


        case PROGRAM_CHANGE:
            state.midi = IDLE;
            break;


        case CONTROL_CHANGE:
            switch(data) {
                default:
                    state.midi = IDLE;
            }
            break;


        ////
        // MIDI data byte 1
        ////


        ////
        // unimplemented / illegal
        ////
        default:
            state.midi = IDLE;
            break;
    }

    // enable interrupts
    sei();
}
