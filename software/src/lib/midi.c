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

#include <stddef.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "midi.h"
#include "leds.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

static enum midi_state midi_state = IDLE;
static struct midi_event_handlers* event_handlers;



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void initialize_midi_module(const struct midi_event_handlers* handlers)
{
    // Configure TXD0
    PORTE.DIR |= PIN3_bm;
    PORTE.OUT |= 1<<PIN3_bp;

    // Set baud rate
    // 32e6 / (16 * 31250) - 1 = 63
    // F_osc       baudrate     BSEL
    MIDI_UART.BAUDCTRLA = 63;
    MIDI_UART.BAUDCTRLB = 0;

    // Enable RXEN and RXC interrupt
    MIDI_UART.CTRLA = USART_RXCINTLVL_LO_gc;
    MIDI_UART.CTRLB = USART_RXEN_bm | USART_TXEN_bm;

    // Save MIDI event handlers
    event_handlers = (struct midi_event_handlers*) handlers;
}

void send_control_change(uint8_t controller, uint8_t value) {
    // send control change status byte
    uart_write((uint8_t) MIDI_CONTROL_CHANGE | MIDI_TX_CHANNEL);

    // send controller number
    uart_write(controller);

    // send value
    uart_write(value);
}

void send_note_off(uint8_t note) {
    // send note off status byte
    uart_write((uint8_t) MIDI_NOTE_OFF | MIDI_TX_CHANNEL);

    // send note number
    uart_write(note);

    // send maximum velocity
    uart_write(MIDI_MAX_VALUE);
}

void send_note_on(uint8_t note) {
    // send note on status byte
    uart_write((uint8_t) MIDI_NOTE_ON | MIDI_TX_CHANNEL);

    // send note number
    uart_write(note);

    // send maximum velocity
    uart_write(MIDI_MAX_VALUE);
}

void send_program_change(uint8_t pnum) {
    // send program change status byte
    uart_write((uint8_t) MIDI_PROGRAM_CHANGE | MIDI_TX_CHANNEL);

    // send program number
    uart_write(pnum);
}



////////////////////////////////////////////////////////////////
//                    I N T E R R U P T S                     //
////////////////////////////////////////////////////////////////

static void handle_status_byte(uint8_t data) {
    if ( (data & MIDI_COMMAND_MASK) == MIDI_NOTE_OFF ) {
        midi_state = NOTE_OFF;
    }
    else if ( (data & MIDI_COMMAND_MASK) == MIDI_NOTE_ON ) {
        midi_state = NOTE_ON;
    }
    else if ( (data & MIDI_COMMAND_MASK) == MIDI_CONTROL_CHANGE ) {
        midi_state = CONTROL_CHANGE_NUMBER;
    }
    else if ( (data & MIDI_COMMAND_MASK) == MIDI_PROGRAM_CHANGE ) {
        midi_state = PROGRAM_CHANGE;
    }
}

ISR(USARTE0_RXC_vect)
{
    // Disable interrupts
    cli();

    flash_led(LED_RED);

    // Fetch data
    uint8_t data = MIDI_UART.DATA;

    static uint8_t current_controller;
    switch (midi_state) {
    ////
    // MIDI status byte
    ////
    case IDLE:
        handle_status_byte(data);
        break;


    ////
    // MIDI data byte 0
    ////
    case NOTE_OFF:
        if (event_handlers->note_off != NULL) {
            event_handlers->note_off(data);
        }
        midi_state = IDLE;
        break;


    case NOTE_ON:
        if (event_handlers->note_on != NULL) {
            event_handlers->note_on(data);
        }
        midi_state = IDLE;
        break;


    case PROGRAM_CHANGE:
        if (event_handlers->program_change != NULL) {
            event_handlers->program_change(data);
        }
        midi_state = IDLE;
        break;


    case CONTROL_CHANGE_NUMBER:
        current_controller = data;
        midi_state = CONTROL_CHANGE_VALUE;
        break;


    case CONTROL_CHANGE_VALUE:
        if (event_handlers->control_change != NULL) {
            event_handlers->control_change(current_controller, data);
        }
        midi_state = IDLE;
        break;


    ////
    // MIDI data byte 1
    ////


    ////
    // unimplemented / illegal
    ////
    default:
        midi_state = IDLE;
        break;
    }

    // enable interrupts
    sei();
}