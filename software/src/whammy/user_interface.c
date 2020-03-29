/// \file
/// \brief      Main user interface

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

#include <stdlib.h>

#include "lib/adafruit_display.h"
#include "lib/gpio.h"
#include "lib/hmi.h"
#include "lib/led_matrix.h"
#include "lib/program.h"
#include "lib/usb.h"

#include "config.h"
#include "user_interface.h"
#include "whammy_controller.h"


////////////////////////////////////////////////////////////////
//               P R I V A T E   D E F I N E S                //
////////////////////////////////////////////////////////////////

#define WAVE_BMP_XSIZE      7
#define WAVE_BMP_YSIZE      5
#define WAVE_BMP_XOFFS      1
#define WAVE_BMP_YOFFS      0

/// \brief      Common settings for waveform bitmaps
/// \details    Intended for calls of led_matrix_show_bitmap
#define WAVE_BMP_SETTINGS   WAVE_BMP_XOFFS, WAVE_BMP_YOFFS, WAVE_BMP_XSIZE, WAVE_BMP_YSIZE,\
                            ADAFRUIT_DISPLAY_COLOR_ORANGE



////////////////////////////////////////////////////////////////
//              P R I V A T E   T Y P E D E F S               //
////////////////////////////////////////////////////////////////

enum hmi_layer
{
    HMI_LAYER_NORMAL,
    HMI_LAYER_PATTERN,
    HMI_LAYER_COUNT
};

enum setting
{
    SETTING_PROGRAM,
    SETTING_SPEED,
    SETTING_AMPLITUDE,
    SETTING_CONTROL_MODE,
    SETTING_WHAMMY_MODE,
    SETTING_COUNT
};

enum status_flag
{
    STATUS_FLAG_MIDI_RX,
    STATUS_FLAG_STORE,
    STATUS_FLAG_TAPPING,
    STATUS_FLAG_USB_RX,
    STATUS_FLAG_COUNT
};



////////////////////////////////////////////////////////////////
//          F O R W A R D   D E C L A R A T I O N S           //
////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

static enum hmi_layer selected_hmi_layer = HMI_LAYER_NORMAL;
static uint8_t active_pattern_step = 0;
static uint8_t selected_pattern_step = 0;
static enum setting selected_setting = SETTING_PROGRAM;
static bool status_flags[STATUS_FLAG_COUNT] = { false, };



///////////////////////////////////////////////////////////////////
// S T A T I C   F U N C T I O N S   A N D   P R O C E D U R E S //
///////////////////////////////////////////////////////////////////

static void display_character(const uint8_t position, const char character)
{
    // Write character to the pixelbuffer
    uint8_t start_column = 0;
    struct led_matrix* led_matrix = NULL;
    enum adafruit_display_color color = ADAFRUIT_DISPLAY_COLOR_ORANGE;
    switch (position) {
        case 0: // leftmost
            led_matrix = &led_matrix_l;
            start_column = 0;
            color = ADAFRUIT_DISPLAY_COLOR_RED;
            break;
        case 1: // middle-left
            led_matrix = &led_matrix_l;
            start_column = 4;
            break;
        case 2: // middle-right
            led_matrix = &led_matrix_r;
            start_column = 0;
            break;
        case 3: // rightmost
            led_matrix = &led_matrix_r;
            start_column = 4;
            break;
        default:
            // Abort if index is out of bounds
            return;
    }

    // Clear previous character including padding to the left
    uint8_t end_column = start_column + LED_MATRIX_CHAR_BITMAP_WIDTH;
    led_matrix_clear_area(led_matrix, start_column, 0,
                                      end_column, LED_MATRIX_CHAR_BITMAP_HEIGHT-1);

    // If whitespace was requested, we're done
    if (character == ' ') {
        return;
    }

    // Display character right-aligned in the cleared box
    led_matrix_show_character(led_matrix, character, start_column+1, 0, color);
}

static void display_string(const char* const str)
{
    uint8_t length = MIN(4, strlen(str));
    for (uint8_t i=0; i<length; ++i) {
        display_character(i, str[i]);
    }
}

static void display_number(const uint8_t number)
{
    display_character(3,  number %  10       );
    display_character(2, (number % 100) /  10);
    display_character(1,  number        / 100);
}

static void display_ctrl_mode(enum ui_ctrl_mode mode)
{
    switch (mode) {
        case UI_CTRL_MODE_RANDOM:
            display_string("MRAN");
            break;
        case UI_CTRL_MODE_MOMENTARY:
            display_string("MBND");
            break;
        case UI_CTRL_MODE_OFF:
            display_string("MOFF");
            break;
        case UI_CTRL_MODE_NORMAL:
            display_string("MNRM");
            break;
        case UI_CTRL_MODE_LIMIT:
            display_string("MLIM");
            break;
        case UI_CTRL_MODE_DETUNE:
            display_string("MFIX");
            break;
        default:
            mode -= UI_CTRL_MODE_PATTERN_01 - 1;
            display_string("MP");
            display_character(2, mode/10);
            display_character(3, mode%10);
            break;
    }
}

static void display_selected_setting(void)
{
    uint8_t number = 0;
    switch (selected_setting) {
        case SETTING_AMPLITUDE:
            number = get_current_amplitude();
            display_character(0, 'A');
            display_number(number);
            break;
        case SETTING_CONTROL_MODE:
            number = get_current_ctrl_mode();
            display_ctrl_mode(number);
            break;
        case SETTING_PROGRAM:
            number = get_current_program_number()+1;
            display_character(0, 'P');
            display_number(number);
            break;
        case SETTING_SPEED:
            number = get_current_speed();
            display_character(0, 'S');
            display_number(number);
            break;
        case SETTING_WHAMMY_MODE:
            number = get_current_whammy_mode();
            display_character(0, 'W');
            display_number(number);
            break;
        default:
            break;
    }
}

static void update_status_leds(void)
{
    static uint16_t led_ttl[STATUS_FLAG_COUNT] = { 0, };
    for (enum status_flag flag = STATUS_FLAG_MIDI_RX; flag < STATUS_FLAG_COUNT; ++flag) {
        // Reset TTL and acknowledge the flag if set
        if (status_flags[flag]) {
            status_flags[flag] = false;
            // "tapping"-LED is synchronized with the tap-tempo timeout and red on-board LED
            led_ttl[flag] = flag == STATUS_FLAG_TAPPING ? 400 : 10;
        }

        // Display flag and reduce TTL
        if (led_ttl[flag] > 0) {
            --led_ttl[flag];
            led_matrix_set_pixel(&led_matrix_l, flag, 0, ADAFRUIT_DISPLAY_COLOR_GREEN);
        }
        gpio_set(STORE_LED_PIN, led_ttl[STATUS_FLAG_STORE]);
        gpio_set(TEMPO_LED_PIN, led_ttl[STATUS_FLAG_TAPPING]);
    }
}

void visualize_pattern_step(uint8_t index, const midi_value_t value)
{
    // Highlight selected step
    enum adafruit_display_color color = ADAFRUIT_DISPLAY_COLOR_GREEN;
    if (index == active_pattern_step) {
        color = ADAFRUIT_DISPLAY_COLOR_RED;
    }
    else if (index == selected_pattern_step) {
        color = ADAFRUIT_DISPLAY_COLOR_ORANGE;
    }

    struct led_matrix* led_matrix = &led_matrix_l;
    index *= 2;
    if (index >= 8) {
        index -= 8;
        led_matrix = &led_matrix_r;
    }

    uint8_t height = value / 16 + 1;
    led_matrix_clear_area(led_matrix, index, 0, index+1, 8);
    led_matrix_draw_rectangle(led_matrix, index, 8-height, index+1, 8, color);
}

static void adjust_current_pattern_step_value(int8_t delta)
{
        uint8_t pattern_number = get_current_pattern_number();
        struct sequencer_step step = get_pattern_step(pattern_number, selected_pattern_step);
        step.channel = WHAMMY_CTRL_MIDI_CHANNEL;
        step.type = MIDI_MSG_TYPE_CONTROL_CHANGE;
        step.data0 = WHAMMY_MIDI_CC_NUMBER;
        step.data1 += delta;
        step.data1 %= MIDI_MAX_VALUE + 1;
        set_whammy_ctrl_pattern_step(selected_pattern_step, &step);
        visualize_pattern_step(selected_pattern_step, step.data1);
}

static void display_current_pattern(void)
{
    // Clear settings display
    led_matrix_clear_area(&led_matrix_l, 0, 0, 7, 7);
    led_matrix_clear_area(&led_matrix_r, 0, 0, 7, 7);

    enum sequencer_pattern_number pattern_number = get_current_pattern_number();
    for (uint8_t index=0; index < get_pattern_length(pattern_number); ++index) {
        struct sequencer_step step = get_pattern_step(pattern_number, index);
        visualize_pattern_step(index, step.data1);
    }
}

static void adjust_pattern_setting(int8_t delta)
{
    uint8_t pattern_number = get_current_pattern_number();
    uint8_t length = get_pattern_length(pattern_number);
    selected_pattern_step += delta;
    if (selected_pattern_step == 0xff) {
        selected_pattern_step = 0;
        if (length > 2) {
            set_pattern_length(pattern_number, length-1);
        }
    }
    if (selected_pattern_step == length) {
        set_pattern_length(pattern_number, length+1);
    }
}


////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void clear_value_display(void)
{
    for (uint8_t column=0; column<8; ++column) {
        led_matrix_set_pixel(&led_matrix_l, 6, column, ADAFRUIT_DISPLAY_COLOR_BLACK);
        led_matrix_set_pixel(&led_matrix_r, 6, column, ADAFRUIT_DISPLAY_COLOR_BLACK);
        led_matrix_set_pixel(&led_matrix_l, 7, column, ADAFRUIT_DISPLAY_COLOR_BLACK);
        led_matrix_set_pixel(&led_matrix_r, 7, column, ADAFRUIT_DISPLAY_COLOR_BLACK);
    }
}

void signal_midi_rx(void)
{
    status_flags[STATUS_FLAG_MIDI_RX] = true;
}

void signal_usb_rx(void)
{
    status_flags[STATUS_FLAG_USB_RX] = true;
}

void signal_store(void)
{
    status_flags[STATUS_FLAG_STORE] = true;
}

void signal_tap_tempo(void)
{
    status_flags[STATUS_FLAG_TAPPING] = true;
}

void store_setup(void)
{
    signal_store();
    update_displays();
    usb_puts(PSTR("Storing current setup"));
    save_current_program();
}

void toggle_hmi_layer(void)
{
    ++selected_hmi_layer;
    selected_hmi_layer %= HMI_LAYER_COUNT;

    clear_value_display();
}

void tap_tempo(void)
{
    register_tap();
    signal_tap_tempo();
}

void update_displays(void)
{
    if (selected_hmi_layer == HMI_LAYER_NORMAL) {
        display_selected_setting();
        update_status_leds();
    }
    else {
        display_current_pattern();
    }
    led_matrix_flush(&led_matrix_l);
    led_matrix_flush(&led_matrix_r);
}

void value1_decrement(void)
{
    if (selected_hmi_layer == HMI_LAYER_PATTERN) {
        adjust_pattern_setting(-1);
        return;
    }

    // Cyclicly decrement selected setting and update display
    if (selected_setting == 0) {
        selected_setting = SETTING_COUNT;
    }
    --selected_setting;
}

void value1_increment(void)
{
    if (selected_hmi_layer == HMI_LAYER_PATTERN) {
        adjust_pattern_setting(1);
        return;
    }

    // Cyclicly increment selected setting and update display
    ++selected_setting;
    selected_setting %= SETTING_COUNT;
}

void value2_decrement(void)
{
    if (selected_hmi_layer == HMI_LAYER_PATTERN) {
        adjust_current_pattern_step_value(-1);
        return;
    }

    uint8_t number = 0;
    switch (selected_setting) {
        case SETTING_AMPLITUDE:
            number = set_whammy_ctrl_range(-1, true);
            display_character(0, 'A');
            display_number(number);
            break;
        case SETTING_CONTROL_MODE:
            number = select_previous_ctrl_mode();
            display_ctrl_mode(number);
            break;
        case SETTING_PROGRAM:
            number = adjust_program(-1) + 1;
            display_character(0, 'P');
            display_number(number);
            usb_printf(PSTR("Selected program %u" USB_NEWLINE), number);
            break;
        case SETTING_SPEED:
            number = adjust_speed(-1);
            display_character(0, 'S');
            display_number(number);
            break;
        case SETTING_WHAMMY_MODE:
            number = adjust_whammy_mode(-1);
            display_character(0, 'W');
            display_number(number);
            break;
        default:
            break;
    }
}

void value2_increment(void)
{
    if (selected_hmi_layer == HMI_LAYER_PATTERN) {
        adjust_current_pattern_step_value(1);
        return;
    }

    uint8_t number = 0;
    switch (selected_setting) {
        case SETTING_AMPLITUDE:
            number = set_whammy_ctrl_range(1, true);
            display_character(0, 'A');
            display_number(number);
            break;
        case SETTING_CONTROL_MODE:
            number = select_next_ctrl_mode();
            display_ctrl_mode(number);
            break;
        case SETTING_PROGRAM:
            number = adjust_program(1) + 1;
            display_character(0, 'P');
            display_number(number);
            usb_printf(PSTR("Selected program %u" USB_NEWLINE), number);
            break;
        case SETTING_SPEED:
            number = adjust_speed(1);
            display_character(0, 'S');
            display_number(number);
            break;
        case SETTING_WHAMMY_MODE:
            number = adjust_whammy_mode(1);
            display_character(0, 'W');
            display_number(number);
            break;
        default:
            break;
    }
}

void visualize_sequencer(uint8_t value)
{
    if (selected_hmi_layer != HMI_LAYER_NORMAL) {
        active_pattern_step = value;
        return;
    }
    struct led_matrix* led_matrix = &led_matrix_l;
    value *= 2;
    if (value >= 8) {
        value -= 8;
        led_matrix = &led_matrix_r;
    }

    clear_value_display();
    led_matrix_set_pixel(led_matrix, 6, value,   ADAFRUIT_DISPLAY_COLOR_GREEN);
    led_matrix_set_pixel(led_matrix, 6, value+1, ADAFRUIT_DISPLAY_COLOR_GREEN);
    led_matrix_set_pixel(led_matrix, 7, value,   ADAFRUIT_DISPLAY_COLOR_GREEN);
    led_matrix_set_pixel(led_matrix, 7, value+1, ADAFRUIT_DISPLAY_COLOR_GREEN);
}

void visualize_wave(uint8_t value)
{
    if (selected_hmi_layer != HMI_LAYER_NORMAL) {
        return;
    }

    struct led_matrix* led_matrix = &led_matrix_l;
    value /= 8; // 128/16
    if (value >= 8) {
        value -= 8;
        led_matrix = &led_matrix_r;
    }

    clear_value_display();
    led_matrix_set_pixel(led_matrix, 7, value, ADAFRUIT_DISPLAY_COLOR_GREEN);
}
