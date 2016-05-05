/// \file
/// \brief      Expression pedal implementation

/*
 * Copyright 2015 Sebastian Neuser
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

#include <stdbool.h>
#include <string.h>

#include "lib/adc.h"
#include "lib/leds.h"
#include "lib/midi.h"
#include "lib/usb.h"

#include "config.h"
#include "expression.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

/// \brief      The latest known expression value
static uint8_t current_value = 0;

static bool enable_state = false;
static bool switch_state = false;



////////////////////////////////////////////////////////////////
//         P R I V A T E   I M P L E M E N T A T I O N        //
////////////////////////////////////////////////////////////////

/// \brief      Send a MIDI note on/off message
/// \param      enable
///                 true sends "note on"; false sends "note off"
static void send_enable_message(bool enable)
{
    if (enable) {
        send_note_on(MIDI_NOTE_ENABLE_WAH, MIDI_MAX_VALUE);
    }
    else {
        send_note_off(MIDI_NOTE_ENABLE_WAH, MIDI_MAX_VALUE);
    }
}



////////////////////////////////////////////////////////////////
//          P U B L I C   I M P L E M E N T A T I O N         //
////////////////////////////////////////////////////////////////

bool exec_calibration(const char* command)
{
    if (strlen(command) != 9) {
        usb_puts("Malformed command" USB_NEWLINE);
        return false;
    }

    // Measure offset and initialize sample buffer
    calibrate_adc_offset(expression_conversion.channel);
    return true;
}

void handle_enable_switch(void)
{
    // Only save switch state on first run
    static bool first_run = true;
    if (first_run) {
        switch_state = gpio_get(ENABLE_SWITCH_PIN);
        first_run = false;
        return;
    }

    // Poll switch
    bool current_switch_state = gpio_get(gpio.header3.pin7);
    if (switch_state != current_switch_state) {
        switch_state = current_switch_state;

        // Broadcast change over MIDI and toggle LED
        enable_state = !enable_state;
        gpio_set(STATUS_LED_PIN, enable_state);
        send_enable_message(enable_state);
    }
}

void trigger_expression_conversion(void)
{
    trigger_adc(expression_conversion.channel);
}

void update_expression_value(uint16_t new_value) {
    if (new_value != current_value) {
        current_value = new_value;
        flash_led(LED_RED);
        send_control_change(69, new_value);
    }
}
