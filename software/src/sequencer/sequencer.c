/// \file
/// \brief      Sequencer module implementation

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

#include <util/delay.h>
#include "lib/gpio.h"
#include "lib/hmi.h"
#include "lib/leds.h"
#include "lib/midi.h"
#include "lib/wave.h"
#include "lib/usb.h"

#include "config.h"
#include "sequencer.h"


////////////////////////////////////////////////////////////////
//               P R I V A T E   D E F I N E S                //
////////////////////////////////////////////////////////////////

#define     STRING_NEXT     "next"
#define     STRING_PREV     "prev"



////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

static bool                     running             = false;
static uint8_t                  step_counter        = 0;

static uint8_t                  controller_number;
static struct wave              wave;



///////////////////////////////////////////////////////////////////
// S T A T I C   F U N C T I O N S   A N D   P R O C E D U R E S //
///////////////////////////////////////////////////////////////////

static void step_sequencer_leds(void)
{
    show_led_pattern(0x80 >> step_counter);
    ++step_counter;
    step_counter %= 8;
}



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

bool exec_speed(const char* command)
{
    if (strlen(command) < 7 || command[5] != ' ') {
        usb_puts("Malformed command" USB_NEWLINE);
        return false;
    }

    midi_value_t speed = atoi(command+6);
    speed %= MIDI_MAX_VALUE + 1;
    usb_printf("Setting waveform speed to %u" USB_NEWLINE, speed);
    set_speed(&wave, speed);
    return true;
}

bool exec_tap(const char* command)
{
    register_tap();
    return true;
}

bool exec_waveform(const char* command)
{
    if (strlen(command) < 13 || command[8] != ' ') {
        usb_puts("Malformed command" USB_NEWLINE);
        return false;
    }

    if (strncmp(STRING_NEXT, command+9, sizeof(STRING_NEXT)) == 0) {
        select_next_waveform();
        return true;
    }
    if (strncmp(STRING_PREV, command+9, sizeof(STRING_PREV)) == 0) {
        select_previous_waveform();
        return true;
    }

    usb_puts("Unknown parameter" USB_NEWLINE);
    return false;
}

/// \brief      Constructs and prints out a nice string announcing the selected waveform
/// \param      waveform
///                 the newly selected waveform
static void echo_selected_waveform(enum waveform waveform)
{
    // Initialize string buffer
    char waveform_string[16] = "";

    switch (waveform) {
    case WAVE_SINE:
        snprintf(waveform_string, sizeof(waveform_string), "sine");
        break;
    case WAVE_TRIANGLE:
        snprintf(waveform_string, sizeof(waveform_string), "triangle");
        break;
    case WAVE_SAW_UP:
        snprintf(waveform_string, sizeof(waveform_string), "saw up");
        break;
    case WAVE_SAW_DOWN:
        snprintf(waveform_string, sizeof(waveform_string), "saw down");
        break;
    case WAVE_SQUARE:
        snprintf(waveform_string, sizeof(waveform_string), "square");
        break;
    case WAVE_STAIRS:
        snprintf(waveform_string, sizeof(waveform_string), "stairs");
        break;
    case WAVE_RANDOM:
        snprintf(waveform_string, sizeof(waveform_string), "random");
        break;
    default:
        snprintf(waveform_string, sizeof(waveform_string), "pattern %u", waveform-WAVE_PATTERN_01+1);
    }

    usb_printf("Switching to waveform: %s" USB_NEWLINE, waveform_string);
}

void select_next_waveform(void)
{
    enum waveform waveform = wave.settings.waveform;
    if (waveform == WAVE_PATTERN_20) {
        waveform = WAVE_SINE;
    }
    else {
        ++waveform;
    }
    echo_selected_waveform(waveform);
    set_waveform(&wave, waveform);
}

void select_previous_waveform(void)
{
    enum waveform waveform = wave.settings.waveform;
    if (waveform == WAVE_SINE) {
        waveform = WAVE_PATTERN_20;
    }
    else {
        --waveform;
    }
    echo_selected_waveform(waveform);
    set_waveform(&wave, waveform);
}

void handle_control_change(uint8_t controller_number, uint8_t value)
{
    //send_control_change(controller_number, value);
    if (controller_number == 69) {
        set_waveform(&wave, value % WAVE_PATTERN_03);
    }
    else if (controller_number == 70) {
        set_speed(&wave, value);
    }
    else if (controller_number == 80) {
        wave.settings.amplitude = value;
    }
    else if (controller_number == 81) {
        wave.settings.offset = value;
    }
}

void init_sequencer_module(struct sequencer_config* config)
{
    controller_number = config->controller_number;
    init_wave(&wave, config->waveform, config->speed, MIDI_MAX_VALUE, 0);
    configure_tap_tempo_wave(&wave);
}

void start_or_stop_sequencer(void)
{
    // If stopped, just set the "running?" flag
    if (!running) {
        running = true;
        return;
    }

    // Otherwise, reset the sequencer
    running = false;
    step_counter = 0;
    show_bar_graph(step_counter);
    reset_wave(&wave);
}

void update_sequencer(void)
{
    // Abort if the 'running?' flag is not set
    if (!running) {
        return;
    }

    // Compute new output value and if it changed, update LEDs
    static uint8_t old_value = 0;
    uint8_t new_value = update_wave(&wave);
    if (new_value != old_value) {
        old_value = new_value;
        send_control_change(controller_number, new_value);
        step_sequencer_leds();
    }
}

void increase_speed(void)
{
    // Add one BPM to the current wave speed as long as we are below 300 BPM
    fixed_t bpm = wave.settings.frequency * 60;
    if (bpm <= fixed_from_int(300)) {
        bpm += fixed_from_int(1);
        set_frequency(&wave, bpm/60);
        usb_printf("Setting wave speed to %u BPM" USB_NEWLINE, fixed_to_int(bpm));
    }
}

void decrease_speed(void)
{
    // Subtract one BPM from the current wave speed as long as we are above 15 BPM
    fixed_t bpm = wave.settings.frequency * 60;
    if (bpm >= fixed_from_int(15)) {
        bpm -= fixed_from_int(1);
        set_frequency(&wave, bpm/60);
        usb_printf("Setting wave speed to %u BPM" USB_NEWLINE, fixed_to_int(bpm));
    }
}

void tap_tempo(void)
{
    register_tap();
}
