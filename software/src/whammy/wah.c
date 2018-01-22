/// \file
/// \brief      Wah-wah module implementation

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

#include "lib/gpio.h"
#include "lib/lookup_tables.h"
#include "lib/math.h"
#include "lib/pwm.h"

#include "config.h"
#include "wah.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

static bool enable_state = false;



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

static inline uint16_t conversion_function(uint8_t midi_value)
{
    fixed_t exp_value = fixed_from_int(lookup_exp(midi_value));
    return fixed_to_int(exp_value / PWM_MAX_DUTY * WAH_PWM_RANGE) + WAH_PWM_MIN_DUTY;
}

void enable_wah(bool enable)
{
    enable_state = enable;
    gpio_set(WAH_BYPASS_PIN, enable);
}

void init_wah_module(void)
{
    // Setup linear conversion function
    init_pwm(WAH_PWM, &conversion_function);
    set_pwm_duty_cycle(WAH_PWM, 0);
}

bool is_wah_enabled(void)
{
    return enable_state;
}

void toggle_wah(void)
{
    enable_wah(!enable_state);
}
