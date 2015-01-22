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


//---------------- data types ----------------//
typedef uint16_t adc_accumulator;


//---------------- constants ----------------//
#define     ADC_INPUT_PIN                   4
#define     ADC_VREF_PIN                    0

#define     ADC_RESOLUTION                  12
#define     ADC_SAMPLE_BUFFER_SIZE          (1 << (8*sizeof(adc_accumulator) - ADC_RESOLUTION))


//---------------- functions and procedures ----------------//
void calibrate_adc_offset(void);
void enable_adc_interrupt(void);
void disable_adc_interrupt(void);
void initialize_adc_module(void);
void trigger_adc(void);


//---------------- EOF ----------------//
#endif // ADC_H

