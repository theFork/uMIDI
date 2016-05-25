/// \file
/// \brief  ADC configuration and service functions

/*
 * Copyright 2014, 2015 Sebastian Neuser
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

#ifndef _ADC_H
#define _ADC_H


//---------------- includes ----------------//
#include <stdint.h>
#include <avr/io.h>


//---------------- data types ----------------//

/// \brief      Data type for ADC mean value computations
typedef uint16_t adc_accumulator;

/// \brief      ADC conversion mode
enum adc_conversion_mode
{
    ADC_MODE_UNSIGNED   = 0x0,                  ///< ADC conversion yields unsigned values
    ADC_MODE_SIGNED     = 0x1                   ///< ADC conversion yields signed values
};

/// \brief      ADC channel
/// \details    Channel 0 is reserved for MIDI values
/// \see        ISR(ADCA_CH0_vect)
enum adc_channel
{
    ADC_CHANNEL_0   = 0,                        ///< ADC channel 0
    ADC_CHANNEL_1   = 1,                        ///< ADC channel 1
    ADC_CHANNEL_2   = 2,                        ///< ADC channel 2
    ADC_CHANNEL_3   = 3                         ///< ADC channel 3
};

/// \brief      ADC input pin
enum adc_input
{
    ADC_INPUT_0   = ADC_CH_MUXPOS_PIN0_gc,      ///< Use ADC input pin 0
    ADC_INPUT_1   = ADC_CH_MUXPOS_PIN1_gc,      ///< Use ADC input pin 1
    ADC_INPUT_2   = ADC_CH_MUXPOS_PIN2_gc,      ///< Use ADC input pin 2
    ADC_INPUT_3   = ADC_CH_MUXPOS_PIN3_gc,      ///< Use ADC input pin 3
    ADC_INPUT_4   = ADC_CH_MUXPOS_PIN4_gc,      ///< Use ADC input pin 4
    ADC_INPUT_5   = ADC_CH_MUXPOS_PIN5_gc,      ///< Use ADC input pin 5
    ADC_INPUT_6   = ADC_CH_MUXPOS_PIN6_gc,      ///< Use ADC input pin 6
    ADC_INPUT_7   = ADC_CH_MUXPOS_PIN7_gc,      ///< Use ADC input pin 7
};

/// \brief      ADC conversion configuration
/// \see        adc_channel
/// \see        adc_input
struct adc_conversion_config
{
    enum adc_channel    channel;                ///< ADC channel
    enum adc_input      input;                  ///< ADC input pin
    void (*callback_signed)(int16_t value);     ///< Conversion callback for signed values
                                                /// \param  value
                                                ///             The conversion result
    void (*callback_unsigned)(uint16_t value);  ///< Conversion callback for unsigned values
                                                /// \param  value
                                                ///             The conversion result
};

/// \brief      ADC configuration common to all conversions
/// \see        adc_conversion_mode
/// \see        iox128a4u.h
struct adc_config
{
    enum adc_conversion_mode    mode;           ///< ADC conversion mode
    enum ADC_PRESCALER_enum     prescaler;      ///< ADC clock prescaler as defined in the AVR gcc headers
};


//---------------- constants ----------------//

/// \brief      ADC resolution in bits
#define     ADC_RESOLUTION                  12

/// \brief      Size of the sample buffer for mean value computations
#define     ADC_SAMPLE_BUFFER_SIZE          (1 << (8*sizeof(adc_accumulator) - ADC_RESOLUTION))


//---------------- functions and procedures ----------------//
/// \brief      Calibrates the ADC offset using the given channel
/// \param      channel
///                 the ADC channel
/// \returns    the measured offset
uint16_t calibrate_adc_offset(enum adc_channel channel);

/// \brief      Disables the interrupt for an ADC channel
/// \param      channel
///                 the ADC channel
void disable_adc_interrupt(enum adc_channel channel);

/// \brief      Enables the interrupt for an ADC channel
/// \param      channel
///                 the ADC channel
void enable_adc_interrupt(enum adc_channel channel);

/// \brief      Sets up an ADC conversion
/// \details    Registers the callbacks, configures the input and performs a dummy-conversion.
///             The GPIO pin is *not* configured here -- this must be done through the GPIO module.
/// \param      config
///                 the ADC conversion configuration
/// \see        init_gpio_module
void init_adc_conversion(const struct adc_conversion_config* config);

/// \brief      Initializes and calibrates the ADC
/// \details    Selects the voltage reference, sets the clock prescaler and conversion mode and
///             calibrates the ADC
/// \param      config
///                 the shared ADC configuration
void init_adc_module(const struct adc_config* config);

/// \brief      Sets the ADC offset
/// \param      offset
///                 the offset
void set_adc_offset(uint16_t offset);

/// \brief      Triggers a conversion for the given channel
/// \param      channel
///                 the ADC channel
void trigger_adc(enum adc_channel channel);


//---------------- EOF ----------------//
#endif // ADC_H

