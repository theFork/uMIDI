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

#include "gpio.h"
#include "midi.h"
#include "leds.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

/// \brief      The MIDI event handlers
/// \see        init_midi_module
static struct midi_event_handlers* event_handlers;

/// \brief      Current state of the MIDI state machine
static enum midi_state midi_state = MIDI_STATE_IDLE;

/// \brief      Omni mode state
/// \see        init_midi_module
static bool omni_mode;

/// \brief      The MIDI receive channel
/// \see        init_midi_module
static enum midi_channel rx_channel;

/// \brief      The MIDI transmit channel
/// \see        init_midi_module
static enum midi_channel tx_channel;

/// \brief      If set to `true`, the red on-board LED flashes on every received MIDI message
/// \see        init_midi_module
static bool signal_rx;



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void init_midi_module(const struct midi_config* const config)
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

    // Save MIDI RX/TX channels, omni mode state and event handlers
    rx_channel = config->rx_channel;
    tx_channel = config->tx_channel;
    omni_mode = config->omni_mode;
    signal_rx = config->signal_rx;
    event_handlers = (struct midi_event_handlers*) &config->event_handlers;
}

enum midi_channel read_midi_channel_from_jumpers(const struct jumpers * jumpers)
{
    return gpio_get(jumpers->jp5) << 3
         | gpio_get(jumpers->jp4) << 2
         | gpio_get(jumpers->jp3) << 1
         | gpio_get(jumpers->jp2);
}

void send_control_change(midi_value_t controller, midi_value_t value)
{
    send_midi_message(tx_channel, MIDI_MSG_TYPE_CONTROL_CHANGE, controller & 0x7f, value & 0x7f);
}

void send_midi_message(enum midi_channel channel, enum midi_message_type type, midi_value_t data0, midi_value_t data1)
{
    // Send status byte
    uart_write(type | channel);

    // Send data byte0
    uart_write(data0 & 0x7f);

    // Send data byte1 only if required
    if (type != MIDI_MSG_TYPE_PROGRAM_CHANGE) {
        uart_write(data1 & 0x7f);
    }
}

void send_note_off(midi_value_t note, midi_value_t velocity)
{
    send_midi_message(tx_channel, MIDI_MSG_TYPE_NOTE_OFF, note & 0x7f, velocity & 0x7f);
}

void send_note_on(midi_value_t note, midi_value_t velocity)
{
    send_midi_message(tx_channel, MIDI_MSG_TYPE_NOTE_ON, note & 0x7f, velocity & 0x7f);
}

void send_program_change(midi_value_t pnum)
{
    send_midi_message(tx_channel, MIDI_MSG_TYPE_PROGRAM_CHANGE, pnum & 0x7f, 0);
}

void set_midi_rx_channel(enum midi_channel channel)
{
    rx_channel = channel;
}

void set_midi_tx_channel(enum midi_channel channel)
{
    tx_channel = channel;
}

void set_omni_mode(bool enable)
{
    omni_mode = enable;
}



////////////////////////////////////////////////////////////////
//                    I N T E R R U P T S                     //
////////////////////////////////////////////////////////////////

/// \brief      Updates the MIDI state machine according to the supplied data byte
/// \param      data
///                 the MIDI data byte to be parsed
static void handle_status_byte(midi_value_t data)
{
    // If the device is not in omni mode, ignore messages on foreign channels
    if (!omni_mode && (data & MIDI_CHANNEL_MASK) != rx_channel) {
        return;
    }

    switch (data & MIDI_MESSAGE_TYPE_MASK) {
    case MIDI_MSG_TYPE_NOTE_OFF:
        midi_state = MIDI_STATE_NOTE_OFF_NUMBER;
        break;

    case MIDI_MSG_TYPE_NOTE_ON:
        midi_state = MIDI_STATE_NOTE_ON_NUMBER;
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
/// \details    This routine reads and parses MIDI data arriving on the USART and calls the
///             callback functions registered during module initialization. For the time being, all
///             MIDI packets are handled, regardless of the MIDI channel, so uMIDI devices are
///             effectively in OMNI mode.
/// \see        init_midi_module
ISR(USARTE0_RXC_vect)
{
    // Disable interrupts
    cli();

    if (signal_rx) {
        flash_led(LED_RED);
    }

    // Fetch data
    uint8_t data = MIDI_UART.DATA;

    static midi_value_t first_data_byte = 0;
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
    case MIDI_STATE_CONTROL_CHANGE_NUMBER:
        first_data_byte = data;
        midi_state = MIDI_STATE_CONTROL_CHANGE_VALUE;
        break;

    case MIDI_STATE_NOTE_OFF_NUMBER:
        first_data_byte = data;
        midi_state = MIDI_STATE_NOTE_OFF_VALUE;
        break;

    case MIDI_STATE_NOTE_ON_NUMBER:
        first_data_byte = data;
        midi_state = MIDI_STATE_NOTE_ON_VALUE;
        break;

    case MIDI_STATE_PROGRAM_CHANGE:
        if (event_handlers->program_change != NULL) {
            event_handlers->program_change(data);
        }
        midi_state = MIDI_STATE_IDLE;
        break;


    ////
    // MIDI data byte 1
    ////
    case MIDI_STATE_CONTROL_CHANGE_VALUE:
        if (event_handlers->control_change != NULL) {
            event_handlers->control_change(first_data_byte, data);
        }
        midi_state = MIDI_STATE_IDLE;
        break;

    case MIDI_STATE_NOTE_OFF_VALUE:
        if (event_handlers->note_off != NULL) {
            event_handlers->note_off(first_data_byte, data);
        }
        midi_state = MIDI_STATE_IDLE;
        break;

    case MIDI_STATE_NOTE_ON_VALUE:
        if (event_handlers->note_on != NULL) {
            event_handlers->note_on(first_data_byte, data);
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
