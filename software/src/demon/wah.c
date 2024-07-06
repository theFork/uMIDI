/// \file
/// \brief      Wah-wah module implementation

/*
 * Copyright 2022 Sebastian Neuser
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
#include <util/delay.h>
#include <avr/eeprom.h>

#include "lib/gpio.h"
#include "lib/leds.h"
#include "lib/math.h"
#include "lib/midi.h"

#include "config.h"
#include "wah.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

static struct linear_range pwm_range;

uint16_t pwm_max_attenuation EEMEM;

static midi_value_t enable_note;
uint8_t enable_note_eemem EEMEM;



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

static inline uint16_t linear_function(uint8_t midi_value)
{
    return linear_from_midi(&pwm_range, midi_value);
}

void enable_wah(bool enable)
{
    static bool enable_state = false;
    if (enable_state == enable) {
        return;
    }

    if (enable) {
        set_led(LED_GREEN, false);
    }

    gpio_set(ENABLE_PIN, enable);
    gpio_set(STATUS_PIN, enable);

    if (!enable) {
        set_led(LED_GREEN, true);
        blink_led(LED_GREEN, F_TASK_SLOW);
    }

    enable_state = enable;
}

void handle_midi_cc(midi_value_t controller, midi_value_t value)
{
    set_wah_frequency(value);
}

void handle_midi_note_off(midi_value_t note, midi_value_t velocity)
{
    if (note == enable_note) {
        enable_wah(false);
    }
}

void handle_midi_note_on(midi_value_t note, midi_value_t velocity)
{
    if (note == enable_note) {
        enable_wah(true);
    }
}

void init_wah_module(void)
{
    // Setup linear conversion function
    pwm_range.from = 490;
    pwm_range.to = PWM_MAX_DUTY - get_wah_max_attenuation();
    init_linear_from_midi(&pwm_range);
    
    // Initialize wah PWM
    init_pwm(WAH_PWM, &linear_function);
    set_pwm_duty_cycle(WAH_PWM, pwm_range.from);

    enable_note = get_wah_enable_note();
}

void set_wah_max_attenuation(uint16_t value)
{
    pwm_range.to = PWM_MAX_DUTY - value;
    init_linear_from_midi(&pwm_range);
    eeprom_write_word(&pwm_max_attenuation, value);
}

uint16_t get_wah_max_attenuation()
{
    return eeprom_read_word(&pwm_max_attenuation);
}

void set_wah_enable_note(midi_value_t value)
{
    enable_note = value;
    eeprom_write_byte(&enable_note_eemem, value);
}

midi_value_t get_wah_enable_note()
{
    return eeprom_read_byte(&enable_note_eemem);
}

void set_wah_frequency(midi_value_t value)
{
    set_pwm_duty_cycle(WAH_PWM, value);
}
