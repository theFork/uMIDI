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

#include "config.h"
#include "lib/gpio.h"
#include "lib/leds.h"
#include "lib/midi.h"
#include "lib/wave.h"
#include "sequencer.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

static uint8_t              controller_number;
static struct wave          wave;

static struct gpio_pin**    status_leds;
static uint8_t              status_leds_size;



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

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
    init_wave(&wave,
                    config->waveform,
                    config->speed,
                    MIDI_MAX_VALUE,
                    0);
}

void update_sequencer(void)
{
    static uint8_t old_value = 0;
    uint8_t new_value = update_wave(&wave);
    if (new_value != old_value) {
        if (wave.state.step_counter == 0) {
            gpio_drive_high(*status_leds[0]);
        }
        else {
            gpio_drive_low(*status_leds[0]);
        }

        old_value = new_value;
        send_control_change(controller_number, new_value);
    }
}

void update_sequencer_leds(void)
{
    static uint8_t prescaler = 0;
    if (++prescaler < 10) {
        return;
    }
    prescaler = 0;

    /*
    static uint8_t led_index = 0;
    gpio_toggle(*status_leds[led_index]);
    ++led_index;
    led_index %= status_leds_size;
    */
}
