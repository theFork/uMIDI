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
#include <avr/eeprom.h>

#include "lib/adc.h"
#include "lib/leds.h"
#include "lib/math.h"
#include "lib/midi.h"
#include "lib/usb.h"

#include "config.h"
#include "expression.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

/// \brief      The latest known expression value
static uint8_t current_expression_value = 0;

/// \brief      The latest known expression value
static uint16_t last_adc_value = 0;

/// \brief      Status variable for expression value console echo
static bool echo = false;

static bool switch_state = false;

static struct linear_range calibration_function = {
        .from = 0,
        .to = 127,
        .slope = 1L << 16 // fixed from int
};

uint16_t adc_offset = 0;

uint16_t adc_offset_eemem EEMEM;
uint16_t from_eemem EEMEM;
uint16_t to_eemem EEMEM;
uint32_t slope_eemem EEMEM;

static uint8_t mute_enabled;
uint8_t mute_enabled_eemem EEMEM;

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

bool exec_cal(const char* command)
{
    if (strlen(command) != 7) {
        usb_puts(PSTR("Malformed command"));
        return false;
    }

    bool echo_state = echo;
    if (!strncmp(command+4, "adc", 3)) {
        usb_puts(PSTR("Calibrating ADC offset..."));
        adc_offset = calibrate_adc_offset(expression_conversion.channel);
        usb_puts(PSTR("done."));
    }
    else if (!strncmp(command+4, "dmp", 3)) {
        usb_printf(PSTR("Offset: %u" USB_NEWLINE), adc_offset);
        usb_printf(PSTR("Min: %u" USB_NEWLINE), calibration_function.from);
        usb_printf(PSTR("Max: %u" USB_NEWLINE), calibration_function.to);
    }
    else if (!strncmp(command+4, "max", 3)) {
        echo = false;
        usb_puts(PSTR("Updating max ADC value and updating linear function range..."));

        calibration_function.to = last_adc_value;
        init_linear_to_midi(&calibration_function);

        usb_puts(PSTR("done."));
    }
    else if (!strncmp(command+4, "min", 3)) {
        echo = false;
        usb_puts(PSTR("Saving min ADC value and updating linear functino range..."));

        calibration_function.from = last_adc_value;
        init_linear_to_midi(&calibration_function);

        usb_puts(PSTR("done."));
    }
    else if (!strncmp(command+4, "sav", 3)) {
        usb_puts(PSTR("Storing current calibration values..."));
        eeprom_write_word(&adc_offset_eemem, adc_offset);
        eeprom_write_word(&from_eemem,       calibration_function.from);
        eeprom_write_word(&to_eemem,         calibration_function.to);
        eeprom_write_dword(&slope_eemem,     calibration_function.slope);
        usb_puts(PSTR("done."));
    }
    echo = echo_state;

    return true;
}

bool exec_echo(const char* command)
{
    if (strlen(command) < 7 || strlen(command) > 8) {
        usb_puts(PSTR("Malformed command" USB_NEWLINE));
        return false;
    }

    // Enable echo via global variable
    if (!strncmp(command+5, "on", 2)) {
        echo = true;
    }
    else if (!strncmp(command+5, "off", 2)) {
        echo = false;
    }

    return true;
}

bool exec_mute(const char* command)
{
    if (strlen(command) < 7 || strlen(command) > 9) {
        usb_puts(PSTR("Malformed command" USB_NEWLINE));
        return false;
    }

    if (!strncmp(command+5, "on", 2)) {
        eeprom_write_byte(&mute_enabled_eemem, true);
        mute_enabled = true;
        usb_puts(PSTR("Muting enabled, setting stored." USB_NEWLINE));
    }
    else if (!strncmp(command+5, "off", 2)) {
        eeprom_write_byte(&mute_enabled_eemem, false);
        mute_enabled = false;
        usb_puts(PSTR("Muting disabled, setting stored." USB_NEWLINE));
    }
    else if (!strncmp(command+5, "stat", 2)) {
        if (mute_enabled) {
            usb_puts(PSTR("Muting currently enabled." USB_NEWLINE));
        } else {
            usb_puts(PSTR("Muting currently disabled." USB_NEWLINE));
        }
    }
    return true;
}

void handle_enable_switch(void)
{
    // Save switch state on first run to ensure the switch is
    // initially off
    static bool first_run = true;
    if (first_run) {
        switch_state = gpio_get(ENABLE_SWITCH_PIN);
        first_run = false;
        return;
    }

    // Poll switch
    bool current_switch_state = gpio_get(gpio.header3.pin7);

    // If switch has changed...
    if (switch_state != current_switch_state) {
        switch_state = current_switch_state;

        // ... toggle status LED
        status_led.state.active = !status_led.state.active;
        gpio_set(STATUS_LED_PIN, status_led.state.active);

        // Use LED state to remember current status
        send_enable_message(status_led.state.active);
    }
}

void init_expression_module(void)
{
    // Read and set adc offset from EEPROM
    adc_offset = eeprom_read_word(&adc_offset_eemem);
    set_adc_offset(adc_offset);

    // Read calibration data from EEPROM
    calibration_function.from = eeprom_read_word(&from_eemem);
    calibration_function.to = eeprom_read_word(&to_eemem);
    calibration_function.slope = eeprom_read_dword(&slope_eemem);

    // Read mute setting from EEPROM
    mute_enabled = eeprom_read_byte(&mute_enabled_eemem);
}

void trigger_expression_conversion(void)
{
    trigger_adc(expression_conversion.channel);
}

void update_expression_value(uint16_t new_adc_value) {
    if (new_adc_value != last_adc_value) {
        // Update stored values
        last_adc_value = new_adc_value;
        current_expression_value = linear_to_midi(&calibration_function, new_adc_value);
        if (current_expression_value > MIDI_MAX_VALUE) {
            current_expression_value = MIDI_MAX_VALUE;
        }

        // If muting enabled only transmit if status LED is on
        if ((mute_enabled && status_led.state.active) || !mute_enabled) {
            send_control_change(69, current_expression_value);
            flash_led(LED_RED);
        }

        // If echo enabled, always print
        if (echo) {
            usb_printf(PSTR("Expression value: %3u" USB_NEWLINE), current_expression_value);
        }
    }
}
