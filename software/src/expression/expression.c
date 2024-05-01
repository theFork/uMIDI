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
#include <stdlib.h>
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

/// \brief      ADC calibration offset
uint16_t adc_offset = 0;

/// \brief      The latest known expression value
static uint8_t current_expression_value = 0;

/// \brief      Status variable for expression value console echo
static bool echo = false;

static bool enable_switch_state = false;

/// \brief      The current pedal mode
//  \see        expression_modes
static enum expression_modes current_mode = MODE_CC_ONLY;

uint16_t adc_offset_eemem EEMEM;
uint16_t from_eemem EEMEM;
uint16_t to_eemem EEMEM;

static uint8_t midi_controller_cc_only;
uint8_t midi_controller_cc_only_eemem EEMEM;
static uint8_t midi_controller_note_and_cc;
uint8_t midi_controller_note_and_cc_eemem EEMEM;
static uint8_t midi_note_enable;
uint8_t midi_note_enable_eemem EEMEM;

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
        send_note_on(midi_note_enable, MIDI_MAX_VALUE);
    }
    else {
        send_note_off(midi_note_enable, MIDI_MAX_VALUE);
    }
}



////////////////////////////////////////////////////////////////
//          P U B L I C   I M P L E M E N T A T I O N         //
////////////////////////////////////////////////////////////////

bool exec_cal(const char* command)
{
    if (strlen(command) != 7) {
        goto cal_fail;
    }

    bool echo_state = echo;
    if (!strncmp(command+4, "adc", 3)) {
        usb_puts(PSTR("Calibrating ADC offset..."));
        adc_offset = calibrate_adc_offset(expression_conversion.channel);
        usb_puts(PSTR("done."));
    }
    else if (!strncmp(command+4, "dmp", 3)) {
        usb_printf(PSTR("Offset: %u" USB_NEWLINE), adc_offset);
        usb_printf(PSTR("Min: %u" USB_NEWLINE), adc_config.input_range.from);
        usb_printf(PSTR("Max: %u" USB_NEWLINE), adc_config.input_range.to);
    }
    else if (!strncmp(command+4, "max", 3)) {
        echo = false;
        usb_puts(PSTR("Updating max ADC value and updating linear function range..."));
        set_adc_channel0_max_value();
        usb_puts(PSTR("done."));
    }
    else if (!strncmp(command+4, "min", 3)) {
        echo = false;
        usb_puts(PSTR("Saving min ADC value and updating linear function range..."));
        set_adc_channel0_min_value();
        usb_puts(PSTR("done."));
    }
    else if (!strncmp(command+4, "sav", 3)) {
        usb_puts(PSTR("Storing current calibration values..."));
        eeprom_write_word(&adc_offset_eemem, adc_offset);
        eeprom_write_word(&from_eemem,       adc_config.input_range.from);
        eeprom_write_word(&to_eemem,         adc_config.input_range.to);
        usb_puts(PSTR("done."));
    }
    else {
        goto cal_fail;
    }
    echo = echo_state;
    return true;

cal_fail:
    usb_puts(PSTR("Malformed command"));
    return false;
}

bool exec_echo(const char* command)
{
    if (strlen(command) < 7 || strlen(command) > 8) {
        goto echo_fail;
    }

    // Enable echo via global variable
    if (!strncmp(command+5, "on", 2)) {
        echo = true;
    }
    else if (!strncmp(command+5, "off", 2)) {
        echo = false;
    }
    else {
        goto echo_fail;
    }
    return true;

echo_fail:
    usb_puts(PSTR("Malformed command" USB_NEWLINE));
    return false;
}

bool exec_ctrl(const char* command)
{
    // Length check
    if (strlen(command) < 8 || strlen(command) > 16) {
        goto ctrl_fail;
    }

    // Get
    if (!strncmp(command+5, "get", 3)) {
        if (strlen(command) == 8) {
            usb_printf(PSTR("CC-only mode     CC:   %3u" USB_NEWLINE), midi_controller_cc_only);
            usb_printf(PSTR("NOTE-and-CC mode CC:   %3u" USB_NEWLINE), midi_controller_note_and_cc);
            usb_printf(PSTR("NOTE-and-CC mode NOTE: %3u" USB_NEWLINE), midi_note_enable);
            return true;
        }
        goto ctrl_fail;
    }
    else if (strncmp(command+5, "set", 3) || strlen(command) < 14) {
        goto ctrl_fail;
    }

    // Set
    uint8_t value = atoi(command+13);
    if (!strncmp(command+9, "ccc", 3)) {
        midi_controller_cc_only = value;
        eeprom_write_byte(&midi_controller_cc_only_eemem, value);
        usb_printf(PSTR("Saved CC number %3u for CC-only mode" USB_NEWLINE), value);
    }
    else if (!strncmp(command+9, "ncc", 3)) {
        midi_controller_note_and_cc = value;
        eeprom_write_byte(&midi_controller_note_and_cc_eemem, value);
        usb_printf(PSTR("Saved CC number %3u for NOTE-and-CC mode" USB_NEWLINE), value);
    }
    else if (!strncmp(command+9, "ncn", 3)) {
        midi_note_enable = value;
        eeprom_write_byte(&midi_note_enable_eemem, value);
        usb_printf(PSTR("Saved NOTE number %3u for NOTE-and-CC mode" USB_NEWLINE), value);
    }
    else {
        goto ctrl_fail;
    }
    return true;

ctrl_fail:
    usb_puts(PSTR("Malformed command" USB_NEWLINE));
    return false;
}

bool exec_mute(const char* command)
{
    if (strlen(command) < 7 || strlen(command) > 9) {
        goto mute_fail;
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
    else {
        goto mute_fail;
    }
    return true;

mute_fail:
    usb_puts(PSTR("Malformed command" USB_NEWLINE));
    return false;
}

bool exec_note(const char* command)
{
    if (strlen(command) < 7 || strlen(command) > 9) {
        goto mute_fail;
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
    else {
        goto mute_fail;
    }
    return true;

mute_fail:
    usb_puts(PSTR("Malformed command" USB_NEWLINE));
    return false;
}

void handle_enable_switch(void)
{
    // Poll switch
    bool current_enable_switch_state = gpio_get(ENABLE_SWITCH_PIN);

    // If switch has changed
    if (enable_switch_state != current_enable_switch_state) {
        enable_switch_state = current_enable_switch_state;
        if (!enable_switch_state) {
            return;
        }

        // Use LED state to remember current toggle status
        status_led.state.active = !status_led.state.active;
        gpio_set(STATUS_LED_PIN, status_led.state.active);
        // Use LED state to remember current status

        // Send NOTE message in wah mode only
        if (MODE_NOTE_AND_CC == current_mode) {
            send_enable_message(status_led.state.active);
        }
    }
}

void handle_mode_select_switch(void)
{
    if (poll_gpio_input(MODE_SELECT_PIN, GPIO_INPUT_PULLUP)) {

        // Wah mode
        if (current_mode == MODE_CC_ONLY) {
            // Visualize mode change
            usb_puts(PSTR("Entered mode: MODE_NOTE_AND_CC" USB_NEWLINE));
            // TODO: need more LEDs!!!11
            set_led(&power_led, true);
            current_mode = MODE_NOTE_AND_CC;
        }

        // CC-only mode
        else if (current_mode == MODE_NOTE_AND_CC) {
            // Visualize mode change.
            usb_puts(PSTR("Entered mode: MODE_CC_ONLY" USB_NEWLINE));
            // TODO: need more LEDs!!!11
            set_led(&power_led, false);
            current_mode = MODE_CC_ONLY;
        }
    }
}

void init_expression_module(void)
{
    // Read and set adc offset from EEPROM
    adc_offset = eeprom_read_word(&adc_offset_eemem);
    set_adc_offset(adc_offset);

    // Read calibration data from EEPROM and modify config
    adc_config.input_range.from = eeprom_read_word(&from_eemem);
    adc_config.input_range.to = eeprom_read_word(&to_eemem);

    // Initialize ADC
    init_adc_module(&adc_config);

    // Configure ADC conversion
    init_adc_conversion(&expression_conversion);
    enable_adc_interrupt(expression_conversion.channel);

    // Read MIDI control setting from EEPROM
    midi_controller_cc_only = eeprom_read_byte(&midi_controller_cc_only_eemem);
    midi_controller_note_and_cc = eeprom_read_byte(&midi_controller_note_and_cc_eemem);
    midi_note_enable = eeprom_read_byte(&midi_note_enable_eemem);

    // Read mute setting from EEPROM
    mute_enabled = eeprom_read_byte(&mute_enabled_eemem);
}

void trigger_expression_conversion(void)
{
    trigger_adc(expression_conversion.channel);
}

void update_expression_value(uint16_t adc_value)
{
    if (adc_value != current_expression_value) {
        // Update stored value
        current_expression_value = adc_value;

        // If muting enabled only transmit if status LED is on
        if ((mute_enabled && status_led.state.active) || !mute_enabled) {
            uint8_t ctrl = (MODE_CC_ONLY == current_mode) ? midi_controller_cc_only : midi_controller_note_and_cc;
            send_control_change(ctrl, current_expression_value);
            flash_led(LED_RED);
        }

        // If echo enabled, always print
        if (echo) {
            usb_printf(PSTR("Expression value: %3u" USB_NEWLINE), current_expression_value);
        }
    }
}
