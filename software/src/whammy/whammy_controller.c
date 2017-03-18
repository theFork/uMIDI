/// \file
/// \brief      Whammy controller implementation

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

#include "lib/program.h"
#include "lib/sequencer.h"
#include "lib/usb.h"
#include "lib/wave.h"

#include "config.h"
#include "user_interface.h"
#include "whammy_controller.h"


////////////////////////////////////////////////////////////////
//               P R I V A T E   D E F I N E S                //
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
//              P R I V A T E   T Y P E D E F S               //
////////////////////////////////////////////////////////////////


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
            { .channel = WHAMMY_CTRL_MIDI_CHANNEL, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_UNISON, },
            { .channel = WHAMMY_CTRL_MIDI_CHANNEL, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_OCTAVE, },
            { .channel = WHAMMY_CTRL_MIDI_CHANNEL, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_UNISON, },
            { .channel = WHAMMY_CTRL_MIDI_CHANNEL, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_OCTAVE, },
            { .channel = WHAMMY_CTRL_MIDI_CHANNEL, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_UNISON, },
            { .channel = WHAMMY_CTRL_MIDI_CHANNEL, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_OCTAVE, },
            { .channel = WHAMMY_CTRL_MIDI_CHANNEL, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_UNISON, },
            { .channel = WHAMMY_CTRL_MIDI_CHANNEL, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_OCTAVE, },
        }
    },

    { // SEQUENCER_PATTERN_02
        .length     = 8,
        .steps      = {
            { .channel = WHAMMY_CTRL_MIDI_CHANNEL, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_UNISON, },
            { .channel = WHAMMY_CTRL_MIDI_CHANNEL, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_1ST_PERFECT_FIFTH, },
            { .channel = WHAMMY_CTRL_MIDI_CHANNEL, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_1ST_OCTAVE, },
            { .channel = WHAMMY_CTRL_MIDI_CHANNEL, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_2ND_PERFECT_FIFTH, },
            { .channel = WHAMMY_CTRL_MIDI_CHANNEL, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_2ND_OCTAVE, },
            { .channel = WHAMMY_CTRL_MIDI_CHANNEL, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_2ND_PERFECT_FIFTH, },
            { .channel = WHAMMY_CTRL_MIDI_CHANNEL, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_1ST_OCTAVE, },
            { .channel = WHAMMY_CTRL_MIDI_CHANNEL, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_1ST_PERFECT_FIFTH, },
        }
    },
    { // SEQUENCER_PATTERN_03
        .length     = 8,
        .steps      = {
            { .channel = WHAMMY_CTRL_MIDI_CHANNEL, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_UNISON, },
            { .channel = WHAMMY_CTRL_MIDI_CHANNEL, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_OCTAVE, },
            { .channel = WHAMMY_CTRL_MIDI_CHANNEL, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_OCTAVE, },
            { .channel = WHAMMY_CTRL_MIDI_CHANNEL, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_UNISON, },
            { .channel = WHAMMY_CTRL_MIDI_CHANNEL, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_OCTAVE, },
            { .channel = WHAMMY_CTRL_MIDI_CHANNEL, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_OCTAVE, },
            { .channel = WHAMMY_CTRL_MIDI_CHANNEL, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_UNISON, },
            { .channel = WHAMMY_CTRL_MIDI_CHANNEL, .type = MIDI_MSG_TYPE_CONTROL_CHANGE, .data0 = 11, .data1 = WHAMMY_NOTE_OCTAVE, },
        }
    },
};
static const uint8_t factory_patterns_size = sizeof(factory_patterns) / sizeof(struct sequencer_pattern);

static struct sequencer_channel sequencer = {
    .speed          = 80,
    .mode           = SEQUENCER_CHANNEL_MODE_CONTINUOUS,
    .tick_callback  = &sequencer_tick_handler,
};

// Initialized in init_whammy_controller()
static struct wave control_wave;

static union whammy_ctrl_program active_program = {
    .field.ctrl_mode = WHAMMY_CTRL_MODE_OFF,
};



///////////////////////////////////////////////////////////////////
// S T A T I C   F U N C T I O N S   A N D   P R O C E D U R E S //
///////////////////////////////////////////////////////////////////

static void execute_program_callback(const uint32_t program_data)
{
    // Update active controller program in RAM
    active_program.word = program_data;

    // Apply Whammy controller mode
    switch (active_program.field.ctrl_mode) {
        case WHAMMY_CTRL_MODE_PATTERN:
            // Set up sequencer according to the loaded program
            set_sequencer_pattern(&sequencer, active_program.field.waveform);
            set_sequencer_speed(&sequencer, active_program.field.speed);
            configure_tap_tempo_wave(&sequencer.wave);
            start_sequencer(&sequencer);
            break;

        case WHAMMY_CTRL_MODE_WAVE:
            // Set up wave
            init_wave(&control_wave, active_program.field.waveform,
                      active_program.field.speed, active_program.field.range, 0);
            configure_tap_tempo_wave(&control_wave);

            // Update sequencer speed for seamless speed adjustment via HMI
            set_sequencer_speed(&sequencer, active_program.field.speed);
            goto cleanup;

        case WHAMMY_CTRL_MODE_DETUNE:
            send_control_change(WHAMMY_MIDI_CC_NUMBER, active_program.field.range);
            // intentional fall-through

        default:
cleanup:
            stop_sequencer(&sequencer);
            clear_value_display();
            break;
    }

    // Update Whammy pedal mode
    if (active_program.field.ctrl_mode == WHAMMY_CTRL_MODE_OFF
    ||  active_program.field.ctrl_mode == WHAMMY_CTRL_MODE_MOMENTARY) {
        send_program_change(WHAMMY_MODE_OFF);
    }
    else {
        send_program_change(active_program.field.pedal_mode);
    }
}

/// \brief      Updates the HMI led bar to reflect the current sequencer pattern step index
static void sequencer_tick_handler(void)
{
    visualize_sequencer(sequencer.step_index);
}

static void stop_modulation(void)
{
    stop_sequencer(&sequencer);
    set_waveform(&control_wave, WAVE_OFF);
    clear_value_display();
}

/// \brief      Updates the HMI led bar to visualize the current waveform
static void update_control_wave(void)
{
    static uint8_t last_sent_value = 0;
    uint8_t value = update_wave(&control_wave);
    if (value != last_sent_value) {
        last_sent_value = value;
        send_control_change(WHAMMY_MIDI_CC_NUMBER, value);
        visualize_wave(value);
    }
}



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

/// \brief      Adjusts the amplitude of the control wave / the pitch bend target
/// \param      delta
///                 difference from current amplitude
uint8_t adjust_amplitude(int8_t delta)
{
    active_program.field.range += delta;
    active_program.field.range %= MIDI_MAX_VALUE+1;
    control_wave.settings.amplitude = active_program.field.range;
    if (active_program.field.ctrl_mode == WHAMMY_CTRL_MODE_DETUNE) {
        send_control_change(WHAMMY_MIDI_CC_NUMBER, active_program.field.range);
    }
    usb_printf(PSTR("Set amplitude to %u" USB_NEWLINE), active_program.field.range);
    // TODO Adjust pitch bend note
    return active_program.field.range;
}


/// \brief      Adjusts the speed of all control modes
/// \param      delta
///                 difference from current speed
uint8_t adjust_speed(int8_t delta)
{
    uint8_t speed = adjust_sequencer_speed(&sequencer, delta);
    // TODO Adjust pitch bend speed
    active_program.field.speed = speed;
    set_speed(&control_wave, speed);
    set_sequencer_speed(&sequencer, speed);
    usb_printf(PSTR("Set speed to %u" USB_NEWLINE), speed);
    return active_program.field.speed;
}

/// \brief      Adjusts the whammy pedal mode
/// \param      delta
///                 difference from current mode
enum whammy_mode adjust_whammy_mode(int8_t delta)
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
    return active_program.field.pedal_mode + 1;
}

void copy_whammy_ctrl_pattern(enum sequencer_pattern_number destination)
{
    copy_pattern(sequencer.pattern, destination);
}

/// \brief      Prints the selected pattern to the terminal
void dump_current_pattern(void)
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
void dump_current_program(void)
{
    usb_puts(PSTR("Mode Wham Spee Ampl Wave"));

    // Prepare pretty output strings
    char* mode_string = "";
    switch (active_program.field.ctrl_mode) {
        case WHAMMY_CTRL_MODE_OFF:
            mode_string = "OFF";
            break;
        case WHAMMY_CTRL_MODE_DETUNE:
            mode_string = "DET";
            break;
        case WHAMMY_CTRL_MODE_NORMAL:
            mode_string = "NRM";
            break;
        case WHAMMY_CTRL_MODE_LIMIT:
            mode_string = "LIM";
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
               active_program.field.range, active_program.field.waveform);
}

/// \brief      Enters bypass (off) mode
void enter_bypass_mode(void)
{
    stop_modulation();
    usb_puts(PSTR("Enabling bypass"));
    active_program.field.ctrl_mode = WHAMMY_CTRL_MODE_OFF;
    send_program_change(WHAMMY_MODE_OFF);
}

/// \brief      Enters static detune mode
void enter_detune_mode(void)
{
    stop_modulation();
    usb_puts(PSTR("Entering detune mode"));
    active_program.field.ctrl_mode = WHAMMY_CTRL_MODE_DETUNE;
    send_control_change(WHAMMY_MIDI_CC_NUMBER, active_program.field.range);
}

/// \brief      Enters limit (CC scaling) mode
void enter_limit_mode(void)
{
    stop_modulation();
    usb_puts(PSTR("Entering limit mode"));
    active_program.field.ctrl_mode = WHAMMY_CTRL_MODE_LIMIT;
}

/// \brief      Enters momentary (pitch bend) mode
void enter_momentary_mode(void)
{
    stop_modulation();
    usb_puts(PSTR("Entering momentary mode"));
    active_program.field.ctrl_mode = WHAMMY_CTRL_MODE_MOMENTARY;
    send_program_change(WHAMMY_MODE_OFF);
}

/// \brief      Enters normal (CC thru) mode
void enter_normal_mode(void)
{
    stop_modulation();
    usb_puts(PSTR("Entering normal mode"));
    active_program.field.ctrl_mode = WHAMMY_CTRL_MODE_NORMAL;
}

/// \brief      Enters pattern mode
void enter_pattern_mode(const enum sequencer_pattern_number pattern)
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
void enter_wave_mode(const enum waveform waveform)
{
    stop_sequencer(&sequencer);

    usb_puts(PSTR("Entering wave mode"));
    active_program.field.ctrl_mode = WHAMMY_CTRL_MODE_WAVE;
    active_program.field.waveform = waveform;
    set_waveform(&control_wave, waveform);
    usb_printf(PSTR("Waveform %d" USB_NEWLINE), waveform);
}

uint8_t get_current_amplitude(void)
{
    return active_program.field.range;
}

enum ui_ctrl_mode get_current_ctrl_mode(void)
{
    switch(active_program.field.ctrl_mode) {
        case WHAMMY_CTRL_MODE_DETUNE:
            return UI_CTRL_MODE_DETUNE;

        case WHAMMY_CTRL_MODE_LIMIT:
            return UI_CTRL_MODE_LIMIT;

        case WHAMMY_CTRL_MODE_MOMENTARY:
            return UI_CTRL_MODE_MOMENTARY;

        case WHAMMY_CTRL_MODE_NORMAL:
            return UI_CTRL_MODE_NORMAL;

        case WHAMMY_CTRL_MODE_WAVE:
            return UI_CTRL_MODE_WAVE_SINE-WAVE_SINE+active_program.field.waveform;

        case WHAMMY_CTRL_MODE_PATTERN:
            return UI_CTRL_MODE_PATTERN_01+sequencer.pattern;

        default:
            return UI_CTRL_MODE_OFF;
    }
}

enum sequencer_pattern_number get_current_pattern_number(void)
{
    return sequencer.pattern;
}

uint8_t get_current_speed(void)
{
    return active_program.field.speed;
}

enum whammy_mode get_current_whammy_mode(void)
{
    return active_program.field.pedal_mode + 1;
}

void handle_midi_control_change(const midi_value_t controller, const midi_value_t orig_value)
{
    uint16_t value = orig_value;
    switch (active_program.field.ctrl_mode) {
        case WHAMMY_CTRL_MODE_LIMIT:
            value *= active_program.field.range;
            value /= MIDI_MAX_VALUE;
        case WHAMMY_CTRL_MODE_NORMAL:
            break;
        default:
            return;
    }
    send_control_change(WHAMMY_MIDI_CC_NUMBER, value);
}

void handle_midi_note_off(midi_value_t note, midi_value_t velocity)
{
    if (active_program.field.ctrl_mode != WHAMMY_CTRL_MODE_MOMENTARY) {
        return;
    }

    // Release pitch bend
    send_control_change(WHAMMY_MIDI_CC_NUMBER, 0);

    // Turn off Whammy pedal
    send_program_change(WHAMMY_MODE_OFF);
}

void handle_midi_note_on(midi_value_t note, midi_value_t velocity)
{
    if (active_program.field.ctrl_mode != WHAMMY_CTRL_MODE_MOMENTARY) {
        return;
    }

    // Enable Whammy pedal
    send_program_change(active_program.field.pedal_mode);

    // Pitch bend
    send_control_change(WHAMMY_MIDI_CC_NUMBER, active_program.field.range);
}

void handle_midi_program_change(midi_value_t program)
{
    usb_printf(PSTR("Entering program #%u" USB_NEWLINE), program+1);
    enter_program(program);
}

void init_whammy_controller(void)
{
    init_program_module(0x0000, &execute_program_callback);
    configure_sequencer_channel(SEQUENCER_CHANNEL_1, &sequencer);
    // speed: 80, amplitude: 127, offset: 0
    init_wave(&control_wave, WAVE_OFF, 80, 127, 0);
}

void reset_whammy_patterns(void)
{
    init_sequencer_patterns(factory_patterns, factory_patterns_size);
}

void save_current_program(void)
{
    update_program(active_program.word);
}

enum ui_ctrl_mode select_next_ctrl_mode(void)
{
    switch(active_program.field.ctrl_mode) {
        case WHAMMY_CTRL_MODE_OFF:
            enter_detune_mode();
            return UI_CTRL_MODE_DETUNE;

        case WHAMMY_CTRL_MODE_DETUNE:
            enter_normal_mode();
            return UI_CTRL_MODE_NORMAL;

        case WHAMMY_CTRL_MODE_NORMAL:
            enter_limit_mode();
            return UI_CTRL_MODE_LIMIT;

        case WHAMMY_CTRL_MODE_LIMIT:
            enter_momentary_mode();
            return UI_CTRL_MODE_MOMENTARY;

        case WHAMMY_CTRL_MODE_MOMENTARY:
            enter_wave_mode(WAVE_SINE);
            return UI_CTRL_MODE_WAVE_SINE;

        case WHAMMY_CTRL_MODE_WAVE:
            ++active_program.field.waveform;
            if (active_program.field.waveform > WAVE_RANDOM) {
                enter_pattern_mode(SEQUENCER_PATTERN_01);
                return UI_CTRL_MODE_PATTERN_01;
            }
            set_waveform(&control_wave, active_program.field.waveform);
            usb_printf(PSTR("Waveform %d" USB_NEWLINE), active_program.field.waveform);
            return UI_CTRL_MODE_WAVE_SINE-WAVE_SINE+active_program.field.waveform;

        case WHAMMY_CTRL_MODE_PATTERN:
            adjust_sequencer_pattern(&sequencer, 1);
            active_program.field.waveform = sequencer.pattern;
            if (sequencer.pattern != SEQUENCER_PATTERN_01) {
                usb_printf(PSTR("Pattern %d" USB_NEWLINE), sequencer.pattern+1);
                return UI_CTRL_MODE_PATTERN_01+sequencer.pattern;
            }
            // else fall through

        default:
            enter_bypass_mode();
            return UI_CTRL_MODE_OFF;
    }
}

enum ui_ctrl_mode select_previous_ctrl_mode(void)
{
    switch(active_program.field.ctrl_mode) {
        case WHAMMY_CTRL_MODE_OFF:
            enter_pattern_mode(SEQUENCER_PATTERN_20);
            return UI_CTRL_MODE_PATTERN_20;

        case WHAMMY_CTRL_MODE_DETUNE:
            enter_bypass_mode();
            return UI_CTRL_MODE_OFF;

        case WHAMMY_CTRL_MODE_NORMAL:
            enter_detune_mode();
            return UI_CTRL_MODE_DETUNE;

        case WHAMMY_CTRL_MODE_LIMIT:
            enter_normal_mode();
            return UI_CTRL_MODE_NORMAL;

        case WHAMMY_CTRL_MODE_MOMENTARY:
            enter_limit_mode();
            return UI_CTRL_MODE_LIMIT;

        case WHAMMY_CTRL_MODE_WAVE:
            --active_program.field.waveform;
            if (active_program.field.waveform == WAVE_OFF) {
                enter_momentary_mode();
                return UI_CTRL_MODE_MOMENTARY;
            }
            set_waveform(&control_wave, active_program.field.waveform);
            usb_printf(PSTR("Waveform %d" USB_NEWLINE), active_program.field.waveform);
            return UI_CTRL_MODE_WAVE_SINE-WAVE_SINE+active_program.field.waveform;

        case WHAMMY_CTRL_MODE_PATTERN:
            adjust_sequencer_pattern(&sequencer, -1);
            if (sequencer.pattern == SEQUENCER_PATTERN_20) {
                enter_wave_mode(WAVE_RANDOM);
                return UI_CTRL_MODE_WAVE_RANDOM;
            }
            usb_printf(PSTR("Pattern %d" USB_NEWLINE), sequencer.pattern+1);
            return UI_CTRL_MODE_PATTERN_01+sequencer.pattern;

        default:
            enter_bypass_mode();
            return UI_CTRL_MODE_OFF;
    }
}

void set_whammy_ctrl_amplitude(uint8_t amplitude)
{
    usb_printf(PSTR("Setting amplitude to %u" USB_NEWLINE), amplitude);
    // TODO Adjust pitch bend note
    active_program.field.range = amplitude;
    if (active_program.field.ctrl_mode == WHAMMY_CTRL_MODE_DETUNE) {
        send_control_change(WHAMMY_MIDI_CC_NUMBER, active_program.field.range);
    }
    control_wave.settings.amplitude = amplitude;
}

void set_whammy_ctrl_pattern_length(uint8_t length)
{
    set_pattern_length(sequencer.pattern, length);
}

void set_whammy_ctrl_pattern_step(uint8_t step_index, const struct sequencer_step * const step)
{
    set_pattern_step(sequencer.pattern, step_index, step);
}

void set_whammy_ctrl_speed(uint8_t speed)
{
    usb_printf(PSTR("Setting waveform speed to %u" USB_NEWLINE), speed);
    // TODO Adjust pitch bend speed
    active_program.field.speed = speed;
    set_speed(&control_wave, speed);
    set_sequencer_speed(&sequencer, speed);
}

void set_whammy_mode(enum whammy_mode mode)
{
    active_program.field.pedal_mode = mode;
    adjust_whammy_mode(-1);
}

void update_controller_value(void)
{
    switch (active_program.field.ctrl_mode) {
        case WHAMMY_CTRL_MODE_PATTERN:
            update_sequencer();
            break;
        case WHAMMY_CTRL_MODE_WAVE:
            update_control_wave();
            break;
        default:
            break;
    }
}

void wipe_whammy_ctrl_pattern(void)
{
    wipe_pattern(sequencer.pattern);
}
