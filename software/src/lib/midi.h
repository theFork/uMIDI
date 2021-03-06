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

#ifndef _MIDI_H
#define _MIDI_H


//---------------- includes ----------------//
#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>

#include "gpio.h"


//---------------- constants ----------------//

/// \brief      MIDI baudrate
#define     BAUD_RATE                   31250

/// \brief      UART used for MIDI I/O
#define     MIDI_UART                   USARTE0

/// \brief      Bitmask for the channel in the MIDI status byte
#define     MIDI_CHANNEL_MASK           0x0f

/// \brief      Bitmask for the message type in the MIDI status byte
#define     MIDI_MESSAGE_TYPE_MASK      0xf0

/// \brief      Maximum MIDI value
#define     MIDI_MAX_VALUE              127


//---------------- data types ----------------//

/// \brief      Type for valid MIDI values [0..127]
typedef uint8_t midi_value_t;

/// \brief      MIDI status byte values (second nibble)
enum midi_channel
{
    MIDI_CHANNEL_01                 = 0x00,     ///< MIDI channel 1
    MIDI_CHANNEL_02                 = 0x01,     ///< MIDI channel 2
    MIDI_CHANNEL_03                 = 0x02,     ///< MIDI channel 3
    MIDI_CHANNEL_04                 = 0x03,     ///< MIDI channel 4
    MIDI_CHANNEL_05                 = 0x04,     ///< MIDI channel 5
    MIDI_CHANNEL_06                 = 0x05,     ///< MIDI channel 6
    MIDI_CHANNEL_07                 = 0x06,     ///< MIDI channel 7
    MIDI_CHANNEL_08                 = 0x07,     ///< MIDI channel 8
    MIDI_CHANNEL_09                 = 0x08,     ///< MIDI channel 9
    MIDI_CHANNEL_10                 = 0x09,     ///< MIDI channel 10
    MIDI_CHANNEL_11                 = 0x0a,     ///< MIDI channel 11
    MIDI_CHANNEL_12                 = 0x0b,     ///< MIDI channel 12
    MIDI_CHANNEL_13                 = 0x0c,     ///< MIDI channel 13
    MIDI_CHANNEL_14                 = 0x0d,     ///< MIDI channel 14
    MIDI_CHANNEL_15                 = 0x0e,     ///< MIDI channel 15
    MIDI_CHANNEL_16                 = 0x0f      ///< MIDI channel 16
};

/// \brief      MIDI status byte values (first nibble)
enum midi_message_type
{
    MIDI_MSG_TYPE_CONTROL_CHANGE    = 0xb0,     ///< Control change message
    MIDI_MSG_TYPE_NOTE_OFF          = 0x80,     ///< Note off message
    MIDI_MSG_TYPE_NOTE_ON           = 0x90,     ///< Note on message
    MIDI_MSG_TYPE_PROGRAM_CHANGE    = 0xc0,     ///< Program change message
};

/// \brief      States in the MIDI state machine
enum midi_state
{
    MIDI_STATE_IDLE,                            ///< No pending MIDI message bytes
    MIDI_STATE_CONTROL_CHANGE_NUMBER,           ///< Control change message status byte read; awaiting controller number
    MIDI_STATE_CONTROL_CHANGE_VALUE,            ///< Control change message controller number read; awaiting value
    MIDI_STATE_NOTE_OFF_NUMBER,                 ///< Note off message status byte read; awaiting note number
    MIDI_STATE_NOTE_OFF_VALUE,                  ///< Note off message note number read; awaiting velocity
    MIDI_STATE_NOTE_ON_NUMBER,                  ///< Note on message status byte read; awaiting note number
    MIDI_STATE_NOTE_ON_VALUE,                   ///< Note on message note number read; awaiting velocity
    MIDI_STATE_PROGRAM_CHANGE,                  ///< Program change message status byte read
};

/// \brief      Pointers to MIDI message handling callbacks
/// \details    These event handlers are automatically called by #ISR(USARTE0_RXC_vect) whenever
///             a MIDI message was received successfully.
/// \see        ISR(USARTE0_RXC_vect)
struct midi_event_handlers
{
    void (*control_change)(midi_value_t controller, midi_value_t value);
                                                ///< Callback for control changes
                                                ///< \param controller
                                                ///<    the changed MIDI controller's number
                                                ///< \param value
                                                ///<    the new MIDI controller value

    void (*note_off)(midi_value_t note, midi_value_t velocity);
                                                ///< Callback for note off messges
                                                ///< \param note
                                                ///<    the MIDI note that was turned off
                                                ///< \param velocity
                                                ///<    velocity of the key press

    void (*note_on)(midi_value_t note, midi_value_t velocity);
                                                ///< Callback for not on messages
                                                ///< \param note
                                                ///<    the MIDI note that was turned on
                                                ///< \param velocity
                                                ///<    velocity of the key press

    void (*program_change)(midi_value_t program);
                                                ///< Callback for program changes
                                                ///< \param note
                                                ///<    the MIDI program to load
};

/// \brief      MIDI transceiver module configuration
struct midi_config
{
    struct midi_event_handlers  event_handlers; ///< MIDI event handler callbacks
    enum midi_channel           rx_channel;     ///< MIDI receive channel
    enum midi_channel           tx_channel;     ///< MIDI transmit channel
    bool                        omni_mode;      ///< Setting this flag enables Omni mode
    bool                        signal_rx;      ///< If set to `true`, the red on-board LED flashes
                                                ///< on every received MIDI message
};


//---------------- functions and procedures ----------------//

/// \brief      Checks if the UART is ready to send data
/// \return     `true` if the UART is ready; `false` otherwise
static inline bool uart_is_ready(void) {
    return MIDI_UART.STATUS & USART_DREIF_bm;
}

/// \brief      Sends one byte of date over the UART
/// \param      data
///                 the data byte to send
static inline void uart_write(uint8_t data) {
    while (!uart_is_ready());
    MIDI_UART.DATA = data;
}

/// \brief      Initializes the MIDI transceiver module
/// \details    Configures the UART, enables the RXC interrupt and registers MIDI message handlers.
/// \param      config
///                 the MIDI module configuration to apply
void init_midi_module(const struct midi_config* config);

/// \brief      Reads the MIDI channel configuration from the solder jumpers
/// \param      jumpers
///                 the the solder jumpers configuration struct
/// \return     the selected MIDI channel
enum midi_channel read_midi_channel_from_jumpers(const struct jumpers * jumpers);

/// \brief      Sends a control change message
/// \param      controller
///                 the MIDI controller number
/// \param      value
///                 the new value for the controller
void send_control_change(midi_value_t controller, midi_value_t value);

/// \brief      Sends an arbitrary MIDI message
/// \details    If the second data byte is not required for the specified message type, the
///             parameter is simply ignored.
/// \param      channel
///                 the MIDI channel to send on
/// \param      type
///                 the type of the MIDI message to send
/// \param      data0
///                 the first data byte (e.g. MIDI controller number for a control change)
/// \param      data1
///                 the second data byte (optional, e.g. value for a control change)
void send_midi_message(enum midi_channel channel, enum midi_message_type type, midi_value_t data0, midi_value_t data1);

/// \brief      Sends a note off message
/// \param      note
///                 the MIDI note number
///< \param     velocity
///<                velocity of the key press
void send_note_off(midi_value_t note, midi_value_t velocity);

/// \brief      Sends a note on message
/// \param      note
///                 the MIDI note number
///< \param     velocity
///<                velocity of the key press
void send_note_on(midi_value_t note, midi_value_t velocity);

/// \brief      Sends a program change message
/// \param      program
///                 the new MIDI program to load
void send_program_change(midi_value_t program);

/// \brief      Updates the midi receive channel
/// \param      channel
///                 the channel to listen on
void set_midi_rx_channel(enum midi_channel channel);

/// \brief      Updates the midi transmit channel
/// \param      channel
///                 the channel to talk on
void set_midi_tx_channel(enum midi_channel channel);

/// \brief      Puts the device into Omni mode
/// \details    In Omni mode the device handles all received MIDI messages regardless of the
///             channel. When not in Omni mode, only messages on the configured receive channel
///             are processed.
/// \param      enable
///                 if `true`, Omni mode is enabled; if `false`, it is disabled
void set_omni_mode(bool enable);


//---------------- EOF ----------------//
#endif // _MIDI_H
