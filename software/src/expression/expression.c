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

/// \brief      The latest known expression value
static uint8_t current_expression_value = 0;

/// \brief      The current pedal mode
//  \see        expression_modes
static enum expression_modes current_mode = MODE_CC_EXPRESSION;

/// \brief      Status variable for expression value console echo
static bool echo = false;

static bool switch_state = false;

uint16_t adc_offset = 0;

uint16_t adc_offset_eemem EEMEM;
uint16_t from_eemem EEMEM;
uint16_t to_eemem EEMEM;

static uint8_t control;
uint8_t control_eemem EEMEM;

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
    if (strlen(command) < 8 || strlen(command) > 11) {
        goto ctrl_fail;
    }

    if (!strncmp(command+5, "get", 3)) {
        usb_printf(PSTR("MIDI control number: %2u" USB_NEWLINE), control);
    }
    else if (!strncmp(command+5, "set", 3)) {
        control = atoi(command+9);
        eeprom_write_byte(&control_eemem, true);
        usb_printf(PSTR("Switching to MIDI control number %2u" USB_NEWLINE), control);
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

void handle_mode_select_switch(void)
{
    if (poll_gpio_input(MODE_SELECT_PIN, GPIO_INPUT_PULLUP)) {

        // Wah mode
        if (MODE_CC_EXPRESSION == current_mode) {
            // Visualize mode change
            usb_puts(PSTR("Entered mode: MODE_WAH" USB_NEWLINE));
            set_led(&power_led, true);
            current_mode = MODE_WAH;
        }

        // CC-Expression mode
        else if (MODE_WAH == current_mode) {
            // Visualize mode change. TODO need more LEDs!!!11
            usb_puts(PSTR("Entered mode: MODE_CC_EXPRESSION" USB_NEWLINE));
            set_led(&power_led, false);
            current_mode = MODE_CC_EXPRESSION;
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
    control = eeprom_read_byte(&control_eemem);

    // Read mute setting from EEPROM
    mute_enabled = eeprom_read_byte(&mute_enabled_eemem);
}

void trigger_expression_conversion(void)
{
    trigger_adc(expression_conversion.channel);
}

void update_expression_value(uint16_t adc_value) {
    if (adc_value != current_expression_value) {
        // Update stored value
        current_expression_value = adc_value;

        // If muting enabled only transmit if status LED is on
        if ((mute_enabled && status_led.state.active) || !mute_enabled) {
            send_control_change(control, current_expression_value);
            flash_led(LED_RED);
        }

        // If echo enabled, always print
        if (echo) {
            usb_printf(PSTR("Expression value: %3u" USB_NEWLINE), current_expression_value);
        }
    }
}
