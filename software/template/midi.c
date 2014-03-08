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
 * MIDI I/O implementation and ISR.
 */

#include "main.h"
#include "midi.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/setbaud.h>


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

extern exec_state_t state;



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void configureUSART( void )
{
    // enable RXEN and RXC interrupt
    UCSR0B = _BV(RXCIE0) | _BV(RXEN0) | _BV(TXEN0);

    // apply UBRR value computed by setbaud.h
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
#if USE_2X
    UCSR0A |= (1 << U2X0);
#else
    UCSR0A &= ~(1 << U2X0);
#endif
}

void sendCC(uint8_t controller, uint8_t value) {
    // send program change status byte
    while ( !(UCSR0A & _BV(UDRE0)) );
    UDR0 = (uint8_t) MIDI_CONTROL_CHANGE | MIDI_TX_CHANNEL;

    // send controller number
    while ( !(UCSR0A & _BV(UDRE0)) );
    UDR0 = controller;

    // send value
    while ( !(UCSR0A & _BV(UDRE0)) );
    UDR0 = value;
}

void sendPC(uint8_t pnum) {
    // send program change status byte
    while ( !(UCSR0A & _BV(UDRE0)) );
    UDR0 = (uint8_t) MIDI_PROGRAM_CHANGE | MIDI_TX_CHANNEL;

    // send program number
    while ( !(UCSR0A & _BV(UDRE0)) );
    UDR0 = pnum;
}



////////////////////////////////////////////////////////////////
//                    I N T E R R U P T S                     //
////////////////////////////////////////////////////////////////

ISR(USART_RX_vect)
{
    // disable interrupts
    cli();

    // fetch data
    uint8_t data = UDR0;

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
