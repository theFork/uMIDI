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

#include "lib/gpio.h"
#include "lib/math.h"
#include "lib/midi.h"

#include "wah.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

static bool enable_state = false;

static struct linear_range pwm_range;



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

static inline uint16_t linear_function(uint8_t midi_value)
{
    return linear_from_midi(&pwm_range, midi_value);
}

void enable_wah(bool enable)
{
    if (enable_state == enable) {
        return;
    }

    if (enable) {
        gpio_set(WAH_ENABLE_PIN, true);
        _delay_ms(4);
        gpio_set(WAH_ENABLE_PIN, false);
    } else {
        gpio_set(WAH_DISABLE_PIN, true);
        _delay_ms(4);
        gpio_set(WAH_DISABLE_PIN, false);
    }
    enable_state = enable;
}

void init_wah_module(void)
{
    // Setup linear conversion function
    pwm_range.from = 0;
    pwm_range.to = PWM_MAX_DUTY;
    init_linear_from_midi(&pwm_range);

    // Initialize wah PWM
    init_pwm(WAH_PWM, &linear_function);
    set_pwm_duty_cycle(WAH_PWM, pwm_range.from);
}

void set_wah_frequency(midi_value_t value)
{
    set_pwm_duty_cycle(WAH_PWM, value);
}
