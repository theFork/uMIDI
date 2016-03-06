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

static uint8_t              controller_number;
static struct wave          wave;

static struct gpio_pin**    status_leds;
static uint8_t              status_leds_size;



///////////////////////////////////////////////////////////////////
// S T A T I C   F U N C T I O N S   A N D   P R O C E D U R E S //
///////////////////////////////////////////////////////////////////

static void step_sequencer_leds(void)
{
    static uint8_t led_index = 0;
    gpio_toggle(*status_leds[led_index]);
    ++led_index;
    led_index %= status_leds_size;
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

bool exec_pattern(const char* command)
{
    if (strlen(command) < 12 || command[7] != ' ') {
        usb_puts("Malformed command" USB_NEWLINE);
        return false;
    }

    enum waveform waveform = wave.settings.waveform;
    if (strncmp(STRING_NEXT, command+8, sizeof(STRING_NEXT)) == 0) {
        ++waveform;
        waveform %= WAVE_PATTERN_08+1;
        usb_printf("Switching to next pattern (%u)" USB_NEWLINE, waveform-WAVE_PATTERN_01);
        goto exec;
    }
    if (strncmp(STRING_PREV, command+8, sizeof(STRING_PREV)) == 0) {
        if (wave.settings.waveform == WAVE_PATTERN_01) {
            waveform = WAVE_PATTERN_08;
        }
        else {
            --waveform;
        }
        usb_printf("Switching to previous pattern (%u)" USB_NEWLINE, waveform-WAVE_PATTERN_01);
        goto exec;
    }
    usb_puts("Unknown parameter" USB_NEWLINE);
    return false;

exec:
    set_waveform(&wave, waveform);
    return true;
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

void init_sequencer_module(struct sequencer_config* config, struct gpio_pin* leds[], uint8_t leds_size)
{
    status_leds = leds;
    status_leds_size = leds_size;
    controller_number = config->controller_number;
    init_wave(&wave, config->waveform, config->speed, MIDI_MAX_VALUE, 0);
    configure_tap_tempo_wave(&wave);
}

void update_sequencer(void)
{
    static uint8_t old_value = 0;
    uint8_t new_value = update_wave(&wave);
    if (new_value != old_value) {
        old_value = new_value;
        send_control_change(controller_number, new_value);
        step_sequencer_leds();
    }
}

void encoder_cw_callback(void)
{
    // Add one BPM to the current wave speed as long as we are below 300 BPM
    fixed_t bpm = wave.settings.frequency * 60;
    if (bpm <= fixed_from_int(300)) {
        bpm += fixed_from_int(1);
        set_frequency(&wave, bpm/60);
        usb_printf("Setting wave speed to %u BPM" USB_NEWLINE, fixed_to_int(bpm));
    }
}

void encoder_ccw_callback(void)
{
    // Subtract one BPM from the current wave speed as long as we are above 15 BPM
    fixed_t bpm = wave.settings.frequency * 60;
    if (bpm >= fixed_from_int(15)) {
        bpm -= fixed_from_int(1);
        set_frequency(&wave, bpm/60);
        usb_printf("Setting wave speed to %u BPM" USB_NEWLINE, fixed_to_int(bpm));
    }
}

void encoder_push_callback(void)
{
    register_tap();
}
