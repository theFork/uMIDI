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
#include <util/delay.h>

#include "lib/gpio.h"
#include "lib/hmi.h"
#include "lib/leds.h"
#include "lib/midi.h"
#include "lib/program.h"
#include "lib/sequencer.h"
#include "lib/usb.h"
#include "lib/wave.h"

#include "config.h"
#include "whammy.h"


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

static void sequencer_tick_handler(void);



////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

static const struct sequencer_pattern factory_patterns[] = {
    { // SEQUENCER_PATTERN_01
        .length     = 8,
        .steps      = {
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_UNISON, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_OCTAVE, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_UNISON, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_OCTAVE, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_UNISON, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_OCTAVE, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_UNISON, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_OCTAVE, },
        }
    },

    { // SEQUENCER_PATTERN_02
        .length     = 8,
        .steps      = {
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_UNISON, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_1ST_PERFECT_FIFTH, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_1ST_OCTAVE, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_2ND_PERFECT_FIFTH, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_2ND_OCTAVE, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_2ND_PERFECT_FIFTH, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_1ST_OCTAVE, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_1ST_PERFECT_FIFTH, },
        }
    },
    { // SEQUENCER_PATTERN_03
        .length     = 8,
        .steps      = {
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_UNISON, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_OCTAVE, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_OCTAVE, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_UNISON, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_OCTAVE, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_OCTAVE, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_UNISON, },
            { .channel = MIDI_CHANNEL_01, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_OCTAVE, },
        }
    },
};
static const uint8_t factory_patterns_size = sizeof(factory_patterns) / sizeof(struct sequencer_pattern);

static struct sequencer_channel sequencer = {
    .speed          = 80,
    .mode           = SEQUENCER_CHANNEL_MODE_CONTINUOUS,
    .tick_callback  = &sequencer_tick_handler,
};

// Initialized in init_whammy_module()
static struct wave control_wave;

static union whammy_ctrl_program active_program = {
    .field.ctrl_mode = WHAMMY_CTRL_MODE_BYPASS,
};

static enum hmi_layer hmi_layer = 0;



///////////////////////////////////////////////////////////////////
// S T A T I C   F U N C T I O N S   A N D   P R O C E D U R E S //
///////////////////////////////////////////////////////////////////

/// \brief      Adjusts the amplitude of the control wave / the pitch bend target
/// \param      delta
///                 difference from current amplitude
static void adjust_amplitude(int8_t delta)
{
    active_program.field.amplitude += delta;
    active_program.field.amplitude %= MIDI_MAX_VALUE+1;
    control_wave.settings.amplitude = active_program.field.amplitude;
    if (active_program.field.ctrl_mode == WHAMMY_CTRL_MODE_DETUNE) {
        send_control_change(WHAMMY_MIDI_CC_NUMBER, active_program.field.amplitude);
    }
    usb_printf(PSTR("Set amplitude to %u" USB_NEWLINE), active_program.field.amplitude);
    // TODO Adjust pitch bend note
}


/// \brief      Adjusts the speed of all control modes
/// \param      delta
///                 difference from current speed
static void adjust_speed(int8_t delta)
{
    uint8_t speed = adjust_sequencer_speed(&sequencer, delta);
    // TODO Adjust pitch bend speed
    active_program.field.speed = speed;
    set_speed(&control_wave, speed);
    set_sequencer_speed(&sequencer, speed);
    usb_printf(PSTR("Set speed to %u" USB_NEWLINE), speed);
}

/// \brief      Adjusts the whammy pedal mode
/// \param      delta
///                 difference from current mode
static void adjust_whammy_mode(int8_t delta)
{
    active_program.field.pedal_mode += delta;

    // Clamp to meaningful values
    if (active_program.field.pedal_mode > WHAMMY_MODE_CHORDS_OCT_2OCT) {
        if (delta < 0) {
            active_program.field.pedal_mode = WHAMMY_MODE_CHORDS_OCT_2OCT;
        }
        else if (delta > 0) {
            active_program.field.pedal_mode = WHAMMY_MODE_CLASSIC_2OCT_UP;
        }
    }
    else {
        while (active_program.field.pedal_mode > WHAMMY_MODE_CLASSIC_OCT_2OCT
           &&  active_program.field.pedal_mode < WHAMMY_MODE_CHORDS_2OCT_UP) {
            active_program.field.pedal_mode += delta;
        }
    }

    usb_printf(PSTR("Selecting Whammy pedal mode %d" USB_NEWLINE), active_program.field.pedal_mode+1);
    send_program_change(active_program.field.pedal_mode);
}

static void clear_leds(void)
{
    show_led_pattern(0x0);
    set_hmi_led(HMI_LED1, hmi_layer == HMI_LAYER_MODE);
    set_hmi_led(HMI_LED2, hmi_layer == HMI_LAYER_PROGRAM);
}

/// \brief      Prints the selected pattern to the terminal
static void dump_current_pattern(void)
{
    uint8_t length = get_pattern_length(sequencer.pattern);
    usb_printf(PSTR("Pattern length: %u" USB_NEWLINE), length);

    usb_puts(PSTR("Steps:" USB_NEWLINE "## Chan Type Dat0 Dat1"));
    for (uint8_t step_index=0; step_index < length; ++step_index) {
        struct sequencer_step step = get_pattern_step(sequencer.pattern, step_index);

        // Prepare pretty output strings
        char* type_string = "";
        switch (step.type) {
            case MIDI_MSG_TYPE_CONTROL_CHANGE:
                type_string = "CCH";
                break;
            case MIDI_MSG_TYPE_NOTE_OFF:
                type_string = "NOF";
                break;
            case MIDI_MSG_TYPE_NOTE_ON:
                type_string = "NON";
                break;
            case MIDI_MSG_TYPE_PROGRAM_CHANGE:
                type_string = "PCH";
                break;
            default:
                type_string = "INV";
                break;
        }
        char data1_string[5] = {'\0',};
        if (step.type != MIDI_MSG_TYPE_PROGRAM_CHANGE) {
            sprintf(data1_string, "%4u", step.data1);
        }

        usb_printf(PSTR("%2u %4u %4s %4u %s" USB_NEWLINE),
                   step_index+1, step.channel+1, type_string, step.data0, data1_string);
    }
}

/// \brief      Prints the active configuration to the terminal
static void dump_current_program(void)
{
    usb_puts(PSTR("Mode Wham Spee Ampl Wave"));

    // Prepare pretty output strings
    char* mode_string = "";
    switch (active_program.field.ctrl_mode) {
        case WHAMMY_CTRL_MODE_BYPASS:
            mode_string = "OFF";
            break;
        case WHAMMY_CTRL_MODE_DETUNE:
            mode_string = "DET";
            break;
        case WHAMMY_CTRL_MODE_MOMENTARY:
            mode_string = "MOM";
            break;
        case WHAMMY_CTRL_MODE_PATTERN:
            mode_string = "PAT";
            break;
        case WHAMMY_CTRL_MODE_WAVE:
            mode_string = "WAV";
            break;
        default:
            mode_string = "INV";
            break;
    }

    usb_printf(PSTR("%4s %4u %4u %4u %4u" USB_NEWLINE),
               mode_string, active_program.field.pedal_mode, active_program.field.speed,
               active_program.field.amplitude, active_program.field.waveform);
}

/// \brief      Enters bypass (off) mode
static void enter_bypass_mode(void)
{
    stop_sequencer(&sequencer);
    set_waveform(&control_wave, WAVE_OFF);

    usb_puts(PSTR("Enabling bypass"));
    active_program.field.ctrl_mode = WHAMMY_CTRL_MODE_BYPASS;
    send_program_change(WHAMMY_MODE_OFF);
}

/// \brief      Enters static detune mode
static void enter_detune_mode(void)
{
    stop_sequencer(&sequencer);
    set_waveform(&control_wave, WAVE_OFF);

    usb_puts(PSTR("Entering detune mode"));
    active_program.field.ctrl_mode = WHAMMY_CTRL_MODE_DETUNE;
    send_control_change(WHAMMY_MIDI_CC_NUMBER, active_program.field.amplitude);
}

/// \brief      Enters momentary (pitch bend) mode
static void enter_momentary_mode(void)
{
    stop_sequencer(&sequencer);
    set_waveform(&control_wave, WAVE_OFF);

    usb_puts(PSTR("Entering momentary mode"));
    active_program.field.ctrl_mode = WHAMMY_CTRL_MODE_MOMENTARY;
    send_program_change(WHAMMY_MODE_OFF);
}

/// \brief      Enters pattern mode
static void enter_pattern_mode(const enum sequencer_pattern_number pattern)
{
    set_waveform(&control_wave, WAVE_OFF);

    usb_puts(PSTR("Entering pattern mode"));
    active_program.field.ctrl_mode = WHAMMY_CTRL_MODE_PATTERN;
    active_program.field.waveform = pattern;
    set_sequencer_pattern(&sequencer, pattern);
    start_sequencer(&sequencer);
    usb_printf(PSTR("Pattern %d" USB_NEWLINE), pattern+1);
}

/// \brief      Enters wave mode
static void enter_wave_mode(const enum waveform waveform)
{
    stop_sequencer(&sequencer);
    clear_leds();

    usb_puts(PSTR("Entering wave mode"));
    active_program.field.ctrl_mode = WHAMMY_CTRL_MODE_WAVE;
    active_program.field.waveform = waveform;
    set_waveform(&control_wave, waveform);
    usb_printf(PSTR("Waveform %d" USB_NEWLINE), waveform);
}

static void select_next_mode(void)
{
    switch(active_program.field.ctrl_mode) {
        case WHAMMY_CTRL_MODE_BYPASS:
            enter_detune_mode();
            break;

        case WHAMMY_CTRL_MODE_DETUNE:
            enter_momentary_mode();
            break;

        case WHAMMY_CTRL_MODE_MOMENTARY:
            enter_wave_mode(WAVE_SINE);
            break;

        case WHAMMY_CTRL_MODE_WAVE:
            ++active_program.field.waveform;
            if (active_program.field.waveform > WAVE_RANDOM) {
                enter_pattern_mode(SEQUENCER_PATTERN_01);
                break;
            }
            set_waveform(&control_wave, active_program.field.waveform);
            usb_printf(PSTR("Waveform %d" USB_NEWLINE), active_program.field.waveform);
            break;

        case WHAMMY_CTRL_MODE_PATTERN:
            adjust_sequencer_pattern(&sequencer, 1);
            active_program.field.waveform = sequencer.pattern;
            if (sequencer.pattern != SEQUENCER_PATTERN_01) {
                usb_printf(PSTR("Pattern %d" USB_NEWLINE), sequencer.pattern+1);
                break;
            }
            // else fall through

        default:
            enter_bypass_mode();
            break;
    }
}

static void select_previous_mode(void)
{
    switch(active_program.field.ctrl_mode) {
        case WHAMMY_CTRL_MODE_BYPASS:
            enter_pattern_mode(SEQUENCER_PATTERN_20);
            break;

        case WHAMMY_CTRL_MODE_DETUNE:
            enter_bypass_mode();
            break;

        case WHAMMY_CTRL_MODE_MOMENTARY:
            enter_detune_mode();
            break;

        case WHAMMY_CTRL_MODE_WAVE:
            --active_program.field.waveform;
            if (active_program.field.waveform == WAVE_OFF) {
                enter_momentary_mode();
                break;
            }
            set_waveform(&control_wave, active_program.field.waveform);
            usb_printf(PSTR("Waveform %d" USB_NEWLINE), active_program.field.waveform);
            break;

        case WHAMMY_CTRL_MODE_PATTERN:
            adjust_sequencer_pattern(&sequencer, -1);
            if (sequencer.pattern == SEQUENCER_PATTERN_20) {
                enter_wave_mode(WAVE_RANDOM);
                break;
            }
            usb_printf(PSTR("Pattern %d" USB_NEWLINE), sequencer.pattern+1);
            break;

        default:
            enter_bypass_mode();
            break;
    }
}

/// \brief      Updates the HMI led bar to reflect the current sequencer pattern step index
static void sequencer_tick_handler(void)
{
    if (hmi_layer == HMI_LAYER_DEFAULT) {
        show_led_pattern(0x80 >> sequencer.step_index);
    }
}



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void cycle_hmi_layer(void)
{
    ++hmi_layer;
    hmi_layer %= HMI_LAYER_COUNT;
    clear_leds();
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
    usb_printf(PSTR("Setting amplitude to %u" USB_NEWLINE), amplitude);
    // TODO Adjust pitch bend note
    active_program.field.amplitude = amplitude;
    if (active_program.field.ctrl_mode == WHAMMY_CTRL_MODE_DETUNE) {
        send_control_change(WHAMMY_MIDI_CC_NUMBER, active_program.field.amplitude);
    }
    control_wave.settings.amplitude = amplitude;
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

    init_sequencer_patterns(factory_patterns, factory_patterns_size);
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
    copy_pattern(sequencer.pattern, number);

    return true;
}

bool exec_patlen(const char* command)
{
    if (strlen(command) < 8 || command[6] != ' ') {
        usb_puts(PSTR("Malformed command"));
        return false;
    }

    uint8_t length = atoi(command+7);
    set_pattern_length(sequencer.pattern, length);

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

    set_pattern_step(sequencer.pattern, step_index, &step);

    return true;
}

bool exec_patwipe(const char* command)
{
    if (strlen(command) != 7) {
        usb_puts(PSTR("Malformed command"));
        return false;
    }

    wipe_pattern(sequencer.pattern);

    return true;
}

bool exec_pgm(const char* command)
{
    // Abort if the command is malformed
    if (strlen(command) < 5 || command[3] != ' ') {
        usb_puts(PSTR("Malformed command"));
        return false;
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
    usb_printf(PSTR("Setting waveform speed to %u" USB_NEWLINE), speed);
    // TODO Adjust pitch bend speed
    active_program.field.speed = speed;
    set_speed(&control_wave, speed);
    set_sequencer_speed(&sequencer, speed);
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

    active_program.field.pedal_mode = atoi(command+5);
    adjust_whammy_mode(-1);
    return true;
}

void execute_program_callback(const uint32_t program_data)
{
    // Update active controller program in RAM
    active_program.word = program_data;

    // Apply Whammy controller mode
    switch (active_program.field.ctrl_mode) {
        case WHAMMY_CTRL_MODE_WAVE:
            stop_sequencer(&sequencer);
            clear_leds();

            // Set up wave
            init_wave(&control_wave, active_program.field.waveform,
                      active_program.field.speed, active_program.field.amplitude, 0);
            configure_tap_tempo_wave(&control_wave);

            // Update sequencer speed for seamless speed adjustment via HMI
            set_sequencer_speed(&sequencer, active_program.field.speed);
            break;

        case WHAMMY_CTRL_MODE_PATTERN:
            // Set up sequencer according to the loaded program
            set_sequencer_pattern(&sequencer, active_program.field.waveform);
            set_sequencer_speed(&sequencer, active_program.field.speed);
            configure_tap_tempo_wave(&sequencer.wave);
            start_sequencer(&sequencer);
            break;

        case WHAMMY_CTRL_MODE_DETUNE:
            send_control_change(WHAMMY_MIDI_CC_NUMBER, active_program.field.amplitude);
            // intentional fall-through

        default:
            stop_sequencer(&sequencer);
            clear_leds();
            break;
    }

    // Update Whammy pedal mode
    if (active_program.field.ctrl_mode == WHAMMY_CTRL_MODE_BYPASS
    ||  active_program.field.ctrl_mode == WHAMMY_CTRL_MODE_MOMENTARY) {
        send_program_change(WHAMMY_MODE_OFF);
    }
    else {
        send_program_change(active_program.field.pedal_mode);
    }
}

void handle_midi_program_change(midi_value_t program)
{
    usb_printf(PSTR("Entering program #%u" USB_NEWLINE), program+1);
    enter_program(program);
}

void init_whammy_module(void)
{
    init_program_module(0x0000, &execute_program_callback);
    configure_sequencer_channel(SEQUENCER_CHANNEL_1, &sequencer);
    // speed: 80, amplitude: 127, offset: 0
    init_wave(&control_wave, WAVE_OFF, 80, 127, 0);
}

void save_current_program(void)
{
    usb_puts(PSTR("Storing current setup"));
    update_program(active_program.word);
}

void update_controller_value(void)
{
    switch (active_program.field.ctrl_mode) {
        case WHAMMY_CTRL_MODE_PATTERN:
            update_sequencer();
            break;
        case WHAMMY_CTRL_MODE_WAVE:
        {
            static uint8_t last_sent_value = 0;
            uint8_t value = update_wave(&control_wave);
            if (value != last_sent_value) {
                last_sent_value = value;
                send_control_change(WHAMMY_MIDI_CC_NUMBER, value);
            }
            break;
        }
        default:
            break;
    }
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
