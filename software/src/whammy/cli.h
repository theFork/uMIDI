/// \file
/// \brief      API for the command line interface of the whammy controller.

/*
 * Copyright 2015, 2016 Sebastian Neuser
 *
 * This file is part of the uMIDI firmware.
 *
 * The MIDI volume controller firmware is free software: you can redistribute it and/or modify
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

#ifndef _CMDLINE_INTERFACE_H
#define _CMDLINE_INTERFACE_H


//---------------- includes ----------------//


//---------------- constants ----------------//

static const char cmd_string_ampl[] PROGMEM = "ampl";
static const char help_string_ampl[] PROGMEM = "a\n"
    "Adjust the amplitude of the wave / pitch bend:\n"
    "<a> : amplitude [0, 127]\n";

static const char cmd_string_backup[] PROGMEM = "backup";
static const char help_string_backup[] PROGMEM = "Dumps all data stored in EEPROM";

static const char cmd_string_cpy[] PROGMEM = "cpy";
static const char help_string_cpy[] PROGMEM = "t i\n"
    "Copies the selected pattern or program to the specified slot:\n"
    "<t> : 'P' for pattern or 'p' for program\n"
    "<i> : target pattern index [0, 19] or program number [0, 119]\n";

static const char cmd_string_dump[] PROGMEM = "dump";
static const char help_string_dump[] PROGMEM = "t\n"
    "Shows information about the selected pattern or program\n"
    "<t> : 'P' for pattern or 'p' for program\n";

static const char cmd_string_reset[] PROGMEM = "reset";
static const char help_string_reset[] PROGMEM = "Restores all patterns to factory settings";

static const char cmd_string_mode[] PROGMEM = "mode";
static const char help_string_mode[] PROGMEM = "p\n"
    "Select control mode:\n"
    "<p> : mode\n"
    "      'N' = switch to next mode\n"
    "      'P' = switch to previous mode\n"
    "      'd'     = static detune mode\n"
    "      'l'     = limit (CC down-scaling) mode\n"
    "      'm'     = momentary pitch bend mode\n"
    "      'n'     = normal (CC forward) mode\n"
    "      'o'     = enable bypass (turn effect off)\n"
    "      'p' <n> = specified pattern n [1, 20]\n"
    "      'r'     = random mode\n";

static const char cmd_string_patlen[] PROGMEM = "patlen";
static const char help_string_patlen[] PROGMEM = "l\n"
    "Set the selected pattern's length:\n"
    "<l> : pattern length in steps\n";

static const char cmd_string_patmod[] PROGMEM = "patmod";
static const char help_string_patmod[] PROGMEM = "s c t d e\n"
    "Modify a step of the selected pattern:\n"
    "<s> : step index [1, length] (2 digits, zero-padded)\n"
    "<c> : MIDI channel [1, 16] (2 digits, zero-padded)\n"
    "<t> : MIDI message type\n"
    "      'c' control change\n"
    "      'f' note off\n"
    "      'n' note on\n"
    "      'p' program change\n"
    "<d> : MIDI data byte 0 (3 digits, zero-padded)\n"
    "<e> : MIDI data byte 1 (3 digits, zero-padded)\n";

static const char cmd_string_pgm[] PROGMEM = "pgm";
static const char help_string_pgm[] PROGMEM = "p\n"
    "Select or update program:\n"
    "<p> : program number [1, 120] or 'u' to overwrite the current program\n";

static const char cmd_string_speed[] PROGMEM = "speed";
static const char help_string_speed[] PROGMEM = "s\n"
    "Adjust the speed of the effect:\n"
    "<s> : bend/pattern/wave speed [0, 127]\n";

static const char cmd_string_store[] PROGMEM = "store";
static const char help_string_store[] PROGMEM = "t n d\n"
    "Stores given data in EEPROM:\n"
    "<t> : 'P' for pattern or 'p' for program\n"
    "<n> : pattern  or program number (counting from 1)\n"
    "<d> : data in hexadecimal format\n";

static const char cmd_string_wahfrq[] PROGMEM = "wahfrq";
static const char help_string_wahfrq[] PROGMEM = "<d>\n"
    "Sets the Wah filter frequency:\n"
    "<d> : PWM duty cycle as MIDI value {0, 1 ... 127}";

static const char cmd_string_wahon[] PROGMEM = "wahon";
static const char help_string_wahon[] PROGMEM = "<d>\n"
    "Enables the Wah device:\n"
    "<d> : Boolean {'t', 'f'}";

static const char cmd_string_wipe[] PROGMEM = "wipe";
static const char help_string_wipe[] PROGMEM = "t\n"
    "Wipes the selected pattern or program\n"
    "<t> : 'P' for pattern or 'p' for program\n";

static const char cmd_string_wham[] PROGMEM = "wham";
static const char help_string_wham[] PROGMEM = "m\n"
    "Select whammy pedal mode:\n"
    "<m> : mode {1..21, 43..63}\n";


//---------------- data types ----------------//


//---------------- functions and procedures ----------------//
bool exec_ampl(const char* command);
bool exec_backup(const char* command);
bool exec_cpy(const char* command);
bool exec_dump(const char* command);
bool exec_reset(const char* command);
bool exec_mode(const char* command);
bool exec_patlen(const char* command);
bool exec_patmod(const char* command);
bool exec_pgm(const char* command);
bool exec_speed(const char* command);
bool exec_store(const char* command);
bool exec_tap(const char* command);
bool exec_wahfrq(const char* command);
bool exec_wahon(const char* command);
bool exec_wipe(const char* command);
bool exec_wham(const char* command);

//---------------- EOF ----------------//
#endif // _CMDLINE_INTERFACE_H

