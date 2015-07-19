/*
 * Copyright 2014,2015 Sebastian Neuser
 *
 * This file is part of the uMIDI firmware.
 *
 * The MIDI volume controller firmware is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The MIDI volume controller firmware is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the uMIDI firmware.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Header for the ADC module of the uMIDI firmware
*/

#ifndef _ADC_H
#define _ADC_H


//---------------- includes ----------------//
#include <stdint.h>
#include <avr/io.h>


//---------------- data types ----------------//
typedef uint16_t adc_accumulator;

enum adc_conversion_mode {
    ADC_MODE_UNSIGNED   = 0x0,
    ADC_MODE_SIGNED     = 0x1
};

enum adc_channel {
    ADC_CHANNEL_0   = 0,
    ADC_CHANNEL_1   = 1,
};

enum adc_input {
    ADC_INPUT_0   = ADC_CH_MUXPOS_PIN0_gc,
    ADC_INPUT_1   = ADC_CH_MUXPOS_PIN1_gc,
    ADC_INPUT_2   = ADC_CH_MUXPOS_PIN2_gc,
    ADC_INPUT_3   = ADC_CH_MUXPOS_PIN3_gc,
    ADC_INPUT_4   = ADC_CH_MUXPOS_PIN4_gc,
    ADC_INPUT_5   = ADC_CH_MUXPOS_PIN5_gc,
    ADC_INPUT_6   = ADC_CH_MUXPOS_PIN6_gc,
    ADC_INPUT_7   = ADC_CH_MUXPOS_PIN7_gc,
};

struct adc_conversion_config {
    enum adc_channel    channel;
    enum adc_input      input;
    void (*callback)(uint8_t);
};

struct adc_config {
    enum adc_conversion_mode    mode;
    enum ADC_PRESCALER_enum     prescaler;
};


//---------------- constants ----------------//
#define     ADC_RESOLUTION                  12
#define     ADC_SAMPLE_BUFFER_SIZE          (1 << (8*sizeof(adc_accumulator) - ADC_RESOLUTION))


//---------------- functions and procedures ----------------//
void calibrate_adc_offset(enum adc_channel channel);
void enable_adc_interrupt(enum adc_channel channel);
void disable_adc_interrupt(enum adc_channel channel);
void initialize_adc_conversion(const struct adc_conversion_config* config);
void initialize_adc_module(const struct adc_config* config, const struct adc_conversion_config* primary_conversion);
void trigger_adc(enum adc_channel channel);


//---------------- EOF ----------------//
#endif // ADC_H

