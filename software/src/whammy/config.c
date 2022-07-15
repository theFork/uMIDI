/// \file
/// \brief      Device configuration

/*
 * Copyright 2015 Sebastian Neuser
 *
 * This file is part of the uMIDI firmware.
 *
 * the uMIDI firmware is free software: you can redistribute it and/or modify
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

/*
 * Device configuration.
 * @author Sebastian Neuser
*/

#include <stddef.h>

#include "lib/background_tasks.h"
#include "lib/gpio.h"
#include "lib/hmi.h"
#include "lib/i2c.h"
#include "lib/led_matrix.h"
#include "lib/leds.h"
#include "lib/midi.h"
#include "lib/sequencer.h"
#include "lib/serial_communication.h"
#include "lib/usb.h"
#include "lib/wave.h"

#include "config.h"
#include "cli.h"
#include "midi_handlers.h"
#include "user_interface.h"
#include "whammy_controller.h"


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

//---------------- Commands ----------------//
struct serial_command serial_commands[] = {
    { .cmd_string = cmd_string_ampl,    .help_string = help_string_ampl,    .handler = &exec_ampl   },
    { .cmd_string = cmd_string_backup,  .help_string = help_string_backup,  .handler = &exec_backup },
    { .cmd_string = cmd_string_cpy,     .help_string = help_string_cpy,     .handler = &exec_cpy    },
    { .cmd_string = cmd_string_dump,    .help_string = help_string_dump,    .handler = &exec_dump   },
    { .cmd_string = cmd_string_mode,    .help_string = help_string_mode,    .handler = &exec_mode   },
    { .cmd_string = cmd_string_patlen,  .help_string = help_string_patlen,  .handler = &exec_patlen },
    { .cmd_string = cmd_string_patmod,  .help_string = help_string_patmod,  .handler = &exec_patmod },
    { .cmd_string = cmd_string_pgm,     .help_string = help_string_pgm,     .handler = &exec_pgm    },
    { .cmd_string = cmd_string_reset,   .help_string = help_string_reset,   .handler = &exec_reset  },
    { .cmd_string = cmd_string_speed,   .help_string = help_string_speed,   .handler = &exec_speed  },
    { .cmd_string = cmd_string_store,   .help_string = help_string_store,   .handler = &exec_store  },
    { .cmd_string = cmd_string_wipe,    .help_string = help_string_wipe,    .handler = &exec_wipe   },
    { .cmd_string = cmd_string_wham,    .help_string = help_string_wham,    .handler = &exec_wham   },
};
uint8_t serial_commands_size = sizeof(serial_commands) / sizeof(struct serial_command);

//---------------- GPIO ----------------//
struct gpio_mapping gpio_mappings[] = {
    { .pin = &STORE_LED_PIN, .type = GPIO_OUTPUT },             // Store LED
    { .pin = &TEMPO_LED_PIN, .type = GPIO_OUTPUT },             // Tempo LED
};
uint8_t gpio_mappings_size = sizeof(gpio_mappings)/sizeof(struct gpio_mapping);

//---------------- HMI ----------------//
struct hmi_config hmi_config = {
    .input_header = &gpio.header1,
    .output_header = NULL,
    .long_input_threashold = 15,
    .button1_short_handler = NULL,
    .button1_long_handler = NULL,
    .button1_interrupt_handler = NULL,
    .button2_short_handler = NULL,
    .button2_long_handler = &store_setup,
    .button2_interrupt_handler = NULL,
    .encoder1cw_handler = &value2_increment,
    .encoder1ccw_handler = &value2_decrement,
    .encoder1push_handler = NULL,
    .encoder2cw_handler = &value1_increment,
    .encoder2ccw_handler = &value1_decrement,
    .encoder2push_handler = NULL,
};


//---------------- I2C ----------------//
struct i2c_config i2c_config = {
    .baudrate = 400000L,
};


//---------------- LED matrix ----------------//
struct led_matrix led_matrix_l = {
    .config = {
        .address    = ADAFRUIT_DISPLAY_ADDRESS(1),
        .mode       = ADAFRUIT_DISPLAY_MODE_STATIC,
        .brightness = ADAFRUIT_DISPLAY_BRIGHTNESS_08_OF_16,
    },
};

struct led_matrix led_matrix_r = {
    .config = {
        .address    = ADAFRUIT_DISPLAY_ADDRESS(2),
        .mode       = ADAFRUIT_DISPLAY_MODE_STATIC,
        .brightness = ADAFRUIT_DISPLAY_BRIGHTNESS_08_OF_16,
    },
};


//---------------- MIDI ----------------//
struct midi_config midi_config = {
    .event_handlers = {
        .control_change = handle_midi_control_change,
        .note_off       = handle_midi_note_off,
        .note_on        = handle_midi_note_on,
        .program_change = handle_midi_program_change
    },
    .omni_mode  = true,
    .rx_channel = WHAMMY_MIDI_CHANNEL,
    .signal_rx  = true,
    .tx_channel = WHAMMY_MIDI_CHANNEL,
};


//---------------- State machine ----------------//
background_task_t high_frequency_tasks[] = {
    &serial_communication_task,
    &poll_hmi,
    &update_controller_value,
};
uint8_t high_frequency_tasks_size = sizeof(high_frequency_tasks)/sizeof(background_task_t);

background_task_t mid_frequency_tasks[] = {
    &usb_main_task,
};
uint8_t mid_frequency_tasks_size = sizeof(mid_frequency_tasks)/sizeof(background_task_t);

background_task_t low_frequency_tasks[] = {
    &update_displays,
    &update_leds,
};
uint8_t low_frequency_tasks_size = sizeof(low_frequency_tasks)/sizeof(background_task_t);
