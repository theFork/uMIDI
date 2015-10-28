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


//---------------- constants ----------------//

/// \brief      MIDI baudrate
#define     BAUD_RATE                   31250

/// \brief      UART used for MIDI I/O
#define     MIDI_UART                   USARTE0

/// \brief      Bitmask for the MIDI status byte
#define     MIDI_COMMAND_MASK           0xf0

/// \brief      Maximum MIDI value
#define     MIDI_MAX_VALUE              127


//---------------- data types ----------------//

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
    MIDI_STATE_NOTE_OFF,                        ///< Note off message status byte read
    MIDI_STATE_NOTE_ON,                         ///< Note on message status byte read
    MIDI_STATE_CONTROL_CHANGE_NUMBER,           ///< Control change message status byte read
    MIDI_STATE_CONTROL_CHANGE_VALUE,            ///< Control change message controller number read
    MIDI_STATE_PROGRAM_CHANGE,                  ///< Program change message status byte read
};

/// \brief      Pointers to MIDI message handling callbacks
/// \details    These event handlers are automatically called by #ISR(USARTE0_RXC_vect) whenever
///             a MIDI message was received successfully.
/// \see        ISR(USARTE0_RXC_vect)
struct midi_event_handlers
{
    void (*control_change)(uint8_t controller, uint8_t value);
                                                ///< Callback for control changes
                                                ///< \param controller
                                                ///<    the changed MIDI controller's number
                                                ///< \param value
                                                ///<    the new MIDI controller value

    void (*note_off)(uint8_t note);             ///< Callback for note off messges
                                                ///< \param note
                                                ///<    the MIDI note that was turned off

    void (*note_on)(uint8_t note);              ///< Callback for not on messages
                                                ///< \param note
                                                ///<    the MIDI note that was turned on

    void (*program_change)(uint8_t program);    ///< Callback for program changes
                                                ///< \param note
                                                ///<    the MIDI program to load
};

/// \brief      MIDI transceiver module configuration
struct midi_config
{
    struct midi_event_handlers  event_handlers; ///< MIDI event handler callbacks
    enum midi_channel           tx_channel;     ///< MIDI transmit channel
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
void initialize_midi_module(const struct midi_config* config);

/// \brief      Sends a control change message
/// \param      controller
///                 the MIDI controller number
/// \param      value
///                 the new value for the controller
void send_control_change(uint8_t controller, uint8_t value);

/// \brief      Sends a note off message
/// \param      note
///                 the MIDI note number
void send_note_off(uint8_t note);

/// \brief      Sends a note on message
/// \param      note
///                 the MIDI note number
void send_note_on(uint8_t note);

/// \brief      Sends a program change message
/// \param      program
///                 the new MIDI program to load
void send_program_change(uint8_t program);


//---------------- EOF ----------------//
#endif // _MIDI_H
