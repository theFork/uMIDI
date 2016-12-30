/// \file
/// \brief      Whammy module implementation

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
#include "lib/hmi.h"
#include "lib/led_matrix.h"
#include "lib/program.h"
#include "lib/sequencer.h"
#include "lib/usb.h"

#include "config.h"
#include "user_interface.h"
#include "whammy_controller.h"


////////////////////////////////////////////////////////////////
//               P R I V A T E   D E F I N E S                //
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
//              P R I V A T E   T Y P E D E F S               //
////////////////////////////////////////////////////////////////

enum hmi_layer
{
    HMI_LAYER_DEFAULT,
    HMI_LAYER_MODE,
    HMI_LAYER_PROGRAM,
    HMI_LAYER_COUNT
};



////////////////////////////////////////////////////////////////
//          F O R W A R D   D E C L A R A T I O N S           //
////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

static enum hmi_layer hmi_layer = 0;



///////////////////////////////////////////////////////////////////
// S T A T I C   F U N C T I O N S   A N D   P R O C E D U R E S //
///////////////////////////////////////////////////////////////////

static void visualize_value(uint8_t value)
{
    clear_displays();

    struct led_matrix* led_matrix = &led_matrix_l;
    if (value >= 8) {
        value -= 8;
        led_matrix = &led_matrix_r;
    }
    led_matrix_set_pixel(led_matrix, 7, value, hmi_layer+1);
}



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void clear_displays(void)
{
    for (uint8_t column=0; column<8; ++column) {
        led_matrix_set_pixel(&led_matrix_l, 7, column, ADAFRUIT_DISPLAY_COLOR_BLACK);
        led_matrix_set_pixel(&led_matrix_r, 7, column, ADAFRUIT_DISPLAY_COLOR_BLACK);
    }
}

void cycle_hmi_layer(void)
{
    ++hmi_layer;
    hmi_layer %= HMI_LAYER_COUNT;
}

bool exec_ampl(const char* command)
{
    // Abort if the command is malformed
    if (strlen(command) < 6 || command[4] != ' ') {
        usb_puts(PSTR("Malformed command"));
        return false;
    }

    midi_value_t amplitude = atoi(command+5);
    amplitude %= MIDI_MAX_VALUE + 1;
    set_whammy_ctrl_amplitude(amplitude);
    return true;
}

bool exec_backup(const char* command)
{
    // Abort if the command is malformed
    if (strlen(command) != 6) {
        usb_puts(PSTR("Malformed command"));
        return false;
    }

    usb_puts(PSTR("Programs:"));
    for (uint8_t i=0; i<PROGRAM_BANK_COUNT; ++i) {
        usb_puts_S(export_bank(i));
    }

    // Dump patterns
    usb_puts(PSTR(USB_NEWLINE "Patterns:"));
    for (uint8_t i=0; i<SEQUENCER_PATTERNS; ++i) {
        usb_puts_S(export_pattern(i));
    }

    return true;
}

bool exec_dump(const char* command)
{
    if (strlen(command) != 6 || command[4] != ' ') {
        usb_puts(PSTR("Malformed command"));
        return false;
    }

    switch (command[5]) {
        case 'p':
            dump_current_program();
            break;

        case 'P':
            dump_current_pattern();
            break;

        default:
            usb_puts(PSTR("Malformed command"));
            return false;
    }

    return true;
}

bool exec_factory_reset(const char* command)
{
    // Abort if the command is malformed
    if (strlen(command) != 12) {
        usb_puts(PSTR("Malformed command"));
        return false;
    }

    reset_whammy_patterns();
    return true;
}

bool exec_mode(const char* command)
{
    if (strlen(command) < 6 || command[4] != ' ') {
        usb_puts(PSTR("Malformed command"));
        return false;
    }

    switch (*(command+5)) {
        case 'N':
            select_next_mode();
            return true;

        case 'P':
            select_previous_mode();
            return true;

        case 'b':
            enter_bypass_mode();
            return true;

        case 'd':
            enter_detune_mode();
            return true;

        case 'm':
            enter_momentary_mode();
            return true;

        case 'p': {
            uint8_t number = atoi(command+7) - 1;
            if (number < SEQUENCER_PATTERNS) {
                enter_pattern_mode(number);
                return true;
            }
        }

        case 'w': {
            uint8_t number = atoi(command+7);
            if (number <= WAVE_RANDOM) {
                enter_wave_mode(number);
                return true;
            }
        }
    }

    usb_puts(PSTR("Unknown parameter" USB_NEWLINE));
    return false;
}

bool exec_patcpy(const char* command)
{
    if (strlen(command) < 8 || command[6] != ' ') {
        usb_puts(PSTR("Malformed command"));
        return false;
    }

    uint8_t number = atoi(command+7);
    copy_whammy_ctrl_pattern(number);

    return true;
}

bool exec_patlen(const char* command)
{
    if (strlen(command) < 8 || command[6] != ' ') {
        usb_puts(PSTR("Malformed command"));
        return false;
    }

    uint8_t length = atoi(command+7);
    set_whammy_ctrl_pattern_length(length);

    return true;
}

bool exec_patmod(const char* command)
{
    if (strlen(command) != 22 || command[6] != ' ' || command[9] != ' ' || command[12] != ' '
                              || command[14] != ' ' || command[18] != ' ') {
        usb_puts(PSTR("Malformed command"));
        return false;
    }

    uint8_t step_index = atoi(command+7) - 1;
    if (step_index >= SEQUENCER_STEPS_PER_PATTERN) {
        usb_puts(PSTR("Invalid pattern index"));
        return false;
    }

    struct sequencer_step step = {0,};
    step.channel = atoi(command+10) - 1;
    switch (*(command+13)) {
        case 'c':
            step.type = MIDI_MSG_TYPE_CONTROL_CHANGE;
            break;
        case 'f':
            step.type = MIDI_MSG_TYPE_NOTE_OFF;
            break;
        case 'n':
            step.type = MIDI_MSG_TYPE_NOTE_ON;
            break;
        case 'p':
            step.type = MIDI_MSG_TYPE_PROGRAM_CHANGE;
            break;
        default:
            break;
    }
    step.data0 = atoi(command+15);
    step.data1 = atoi(command+19);

    set_whammy_ctrl_pattern_step(step_index, &step);

    return true;
}

bool exec_patwipe(const char* command)
{
    if (strlen(command) != 7) {
        usb_puts(PSTR("Malformed command"));
        return false;
    }

    wipe_whammy_ctrl_pattern();

    return true;
}

bool exec_pgm(const char* command)
{
    // Abort if the command is malformed
    if (strlen(command) < 5 || command[3] != ' ') {
        usb_puts(PSTR("Malformed command"));
        return false;
    }

    // Update current program if 'u' was given as argument
    if (command[4] == 'u') {
        save_current_program();
        return true;
    }

    uint8_t program = atoi(command+4);
    usb_printf(PSTR("Entering program #%u" USB_NEWLINE), program);
    enter_program(program-1);
    return true;
}

bool exec_speed(const char* command)
{
    if (strlen(command) < 7 || command[5] != ' ') {
        usb_puts(PSTR("Malformed command"));
        return false;
    }

    midi_value_t speed = atoi(command+6);
    speed %= MIDI_MAX_VALUE + 1;
    set_whammy_ctrl_speed(speed);
    return true;
}

bool exec_store(const char* command)
{
    // Abort if the command is malformed
    if (strlen(command) < 11 || command[5] != ' ' || command[7] != ' '
    ||  (command[6] != 'P' && command[6] != 'p')) {
        usb_puts(PSTR("Malformed command"));
        return false;
    }

    // Extract pattern or program number
    uint8_t number = strtol(&command[8], NULL, 16);

    if (command[6] == 'P') {
        usb_printf(PSTR("Storing pattern #%d..." USB_NEWLINE), number);
        import_pattern(number, &command[10]);
    }
    if (command[6] == 'p') {
        --number;
        usb_printf(PSTR("Storing program #%d..." USB_NEWLINE), number);
        write_program(number, strtol(&command[10], NULL, 16));
    }

    return true;
}

bool exec_tap(const char* command)
{
    register_tap();
    return true;
}

bool exec_wham(const char* command)
{
    // Abort if the command is malformed
    if (strlen(command) < 6 || command[4] != ' ') {
        usb_puts(PSTR("Malformed command"));
        return false;
    }

    set_whammy_mode(atoi(command+5));
    return true;
}

void store_setup(void)
{
    usb_puts(PSTR("Storing current setup"));
    save_current_program();
}

void update_displays(void)
{
    led_matrix_flush(&led_matrix_l);
    led_matrix_flush(&led_matrix_r);
}

void value1_decrement(void)
{
    switch (hmi_layer) {
        case HMI_LAYER_DEFAULT:
            adjust_amplitude(-1);
            break;
        case HMI_LAYER_MODE:
            select_previous_mode();
            break;
        case HMI_LAYER_PROGRAM:
            usb_printf(PSTR("Selected program %u" USB_NEWLINE), adjust_program(-1)+1);
            break;
        default:
            break;
    }
}

void value1_increment(void)
{
    switch (hmi_layer) {
        case HMI_LAYER_DEFAULT:
            adjust_amplitude(1);
            break;
        case HMI_LAYER_MODE:
            select_next_mode();
            break;
        case HMI_LAYER_PROGRAM:
            usb_printf(PSTR("Selected program %u" USB_NEWLINE), adjust_program(1)+1);
            break;
        default:
            break;
    }
}

void value2_decrement(void)
{
    switch (hmi_layer) {
        case HMI_LAYER_DEFAULT:
            adjust_speed(-1);
            break;
        case HMI_LAYER_MODE:
            adjust_whammy_mode(-1);
            break;
        case HMI_LAYER_PROGRAM:
            break;
        default:
            break;
    }
}

void value2_increment(void)
{
    switch (hmi_layer) {
        case HMI_LAYER_DEFAULT:
            adjust_speed(1);
            break;
        case HMI_LAYER_MODE:
            adjust_whammy_mode(1);
            break;
        case HMI_LAYER_PROGRAM:
            break;
        default:
            break;
    }
}

void visualize_sequencer(const uint8_t value)
{
    visualize_value(value);
}

void visualize_wave(const uint8_t value)
{
    visualize_value(value / 8);
}
