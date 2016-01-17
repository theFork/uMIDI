///\file
///\brief   MIDI message transceiver

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

#include <stddef.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "midi.h"
#include "leds.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

/// \brief      Current state of the MIDI state machine
static enum midi_state midi_state = MIDI_STATE_IDLE;

/// \brief      The MIDI transmit channel
/// \see        init_midi_module
static enum midi_channel tx_channel = 1;

/// \brief      The MIDI event handlers
/// \see        init_midi_module
static struct midi_event_handlers* event_handlers;



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void init_midi_module(const struct midi_config* config)
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
    tx_channel = config->tx_channel;
    event_handlers = (struct midi_event_handlers*) &config->event_handlers;
}

void send_control_change(midi_value_t controller, midi_value_t value) {
    // Send control change status byte
    uart_write((uint8_t) MIDI_MSG_TYPE_CONTROL_CHANGE | tx_channel);

    // Send controller number
    uart_write(controller);

    // Send value
    uart_write(value);
}

void send_note_off(midi_value_t note) {
    // Send note off status byte
    uart_write((uint8_t) MIDI_MSG_TYPE_NOTE_OFF | tx_channel);

    // Send note number
    uart_write(note);

    // Send maximum velocity
    uart_write(MIDI_MAX_VALUE);
}

void send_note_on(midi_value_t note) {
    // Send note on status byte
    uart_write((uint8_t) MIDI_MSG_TYPE_NOTE_ON | tx_channel);

    // Send note number
    uart_write(note);

    // Send maximum velocity
    uart_write(MIDI_MAX_VALUE);
}

void send_program_change(midi_value_t pnum) {
    // Send program change status byte
    uart_write((uint8_t) MIDI_MSG_TYPE_PROGRAM_CHANGE | tx_channel);

    // Send program number
    uart_write(pnum);
}



////////////////////////////////////////////////////////////////
//                    I N T E R R U P T S                     //
////////////////////////////////////////////////////////////////

/// \brief      Updates the MIDI state machine according to the supplied data byte
/// \param      data
///                 the MIDI data byte to be parsed
static void handle_status_byte(midi_value_t data) {
    switch (data & MIDI_COMMAND_MASK) {
    case MIDI_MSG_TYPE_NOTE_OFF:
        midi_state = MIDI_STATE_NOTE_OFF;
        break;

    case MIDI_MSG_TYPE_NOTE_ON:
        midi_state = MIDI_STATE_NOTE_ON;
        break;

    case MIDI_MSG_TYPE_CONTROL_CHANGE:
        midi_state = MIDI_STATE_CONTROL_CHANGE_NUMBER;
        break;

    case MIDI_MSG_TYPE_PROGRAM_CHANGE:
        midi_state = MIDI_STATE_PROGRAM_CHANGE;
        break;

    default:
        break;
    }
}

/// \brief      Main interrupt service routine for incoming MIDI messages
/// \details    TODO
ISR(USARTE0_RXC_vect)
{
    // Disable interrupts
    cli();

    flash_led(LED_RED);

    // Fetch data
    uint8_t data = MIDI_UART.DATA;

    static midi_value_t current_controller;
    switch (midi_state) {
    ////
    // MIDI status byte
    ////
    case MIDI_STATE_IDLE:
        handle_status_byte(data);
        break;


    ////
    // MIDI data byte 0
    ////
    case MIDI_STATE_NOTE_OFF:
        if (event_handlers->note_off != NULL) {
            event_handlers->note_off(data);
        }
        midi_state = MIDI_STATE_IDLE;
        break;


    case MIDI_STATE_NOTE_ON:
        if (event_handlers->note_on != NULL) {
            event_handlers->note_on(data);
        }
        midi_state = MIDI_STATE_IDLE;
        break;


    case MIDI_STATE_PROGRAM_CHANGE:
        if (event_handlers->program_change != NULL) {
            event_handlers->program_change(data);
        }
        midi_state = MIDI_STATE_IDLE;
        break;


    case MIDI_STATE_CONTROL_CHANGE_NUMBER:
        current_controller = data;
        midi_state = MIDI_STATE_CONTROL_CHANGE_VALUE;
        break;


    ////
    // MIDI data byte 1
    ////
    case MIDI_STATE_CONTROL_CHANGE_VALUE:
        if (event_handlers->control_change != NULL) {
            event_handlers->control_change(current_controller, data);
        }
        midi_state = MIDI_STATE_IDLE;
        break;


    ////
    // Unimplemented / illegal
    ////
    default:
        midi_state = MIDI_STATE_IDLE;
        break;
    }

    // Enable interrupts
    sei();
}
