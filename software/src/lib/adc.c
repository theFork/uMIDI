/// \file
/// \brief  ADC configuration and service functions

/*
 * Copyright 2014, 2015 Sebastian Neuser
 *
 * This file is part of the uMIDI firmware.
 *
 * the uMIDI firmware is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * the uMIDI firmware is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the uMIDI firmware.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stddef.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "adc.h"
#include "math.h"
#include "midi.h"
#include "leds.h"

#include "usb.h"



////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

/// \brief      ADC channel configuration registers
struct adc_channel_regs {
    volatile uint8_t*   control_register;
    volatile uint8_t*   mux_register;
    volatile uint8_t*   interrupt_register;
    volatile uint8_t*   interrupt_flag;
    volatile uint16_t*  result_register;
};

/// \brief      Lookup table for ADC channel configuration registers
static const struct adc_channel_regs adc_regs[] = {
    { // Channel 0
        .control_register   = &ADCA_CH0_CTRL,
        .mux_register       = &ADCA_CH0_MUXCTRL,
        .interrupt_register = &ADCA_CH0_INTCTRL,
        .interrupt_flag     = &ADCA_CH0_INTFLAGS,
        .result_register    = &ADCA.CH0RES
    },
    { // Channel 1
        .control_register   = &ADCA_CH1_CTRL,
        .mux_register       = &ADCA_CH1_MUXCTRL,
        .interrupt_register = &ADCA_CH1_INTCTRL,
        .interrupt_flag     = &ADCA_CH1_INTFLAGS,
        .result_register    = &ADCA.CH1RES
    },
    { // Channel 2
        .control_register   = &ADCA_CH2_CTRL,
        .mux_register       = &ADCA_CH2_MUXCTRL,
        .interrupt_register = &ADCA_CH2_INTCTRL,
        .interrupt_flag     = &ADCA_CH2_INTFLAGS,
        .result_register    = &ADCA.CH2RES
    },
    { // Channel 3
        .control_register   = &ADCA_CH3_CTRL,
        .mux_register       = &ADCA_CH3_MUXCTRL,
        .interrupt_register = &ADCA_CH3_INTCTRL,
        .interrupt_flag     = &ADCA_CH3_INTFLAGS,
        .result_register    = &ADCA.CH3RES
    },
};


/// \brief      ADC offset
/// \details    Set during offset calibration
static uint16_t offset;

/// \brief      Buffer for ADC conversion results of channel 0
/// \see        ISR(ADCA_CH0_vect)
static uint16_t sample_buffer[ADC_SAMPLE_BUFFER_SIZE];

/// \brief      Conversion callbacks for signed values
/// \details    Used in the interrupt service routines
static void (*callbacks_signed[sizeof(adc_regs)/sizeof(struct adc_channel_regs)])(int16_t);

/// \brief      Conversion callbacks for unsigned values
/// \details    Used in the interrupt service routines
static void (*callbacks_unsigned[sizeof(adc_regs)/sizeof(struct adc_channel_regs)])(uint16_t);

/// \brief      Conversion callbacks for unsigned values
/// \details    Used in the interrupt service routines
static struct linear_range* adc_input_range;



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

static uint16_t compensate_offset(uint16_t sample)
{
    // Compensate offset and prevent integer overflow
    sample -= offset;
    if (sample >= 1<<ADC_RESOLUTION) {
        sample = 0;
    }
    return sample;
}

static uint16_t perform_filtered_measurement(enum adc_channel channel)
{
    adc_accumulator accumulator = 0;
    cli();
    for (uint8_t i=0; i<ADC_SAMPLE_BUFFER_SIZE; i++) {
        trigger_adc(channel);
        while (!*adc_regs[channel].interrupt_flag);
        *adc_regs[channel].interrupt_flag = true;
        accumulator += *adc_regs[channel].result_register;
    }
    sei();
    return accumulator / ADC_SAMPLE_BUFFER_SIZE;
}

uint16_t calibrate_adc_offset(enum adc_channel channel)
{
    offset = perform_filtered_measurement(channel);
    return offset;
}

inline void disable_adc_interrupt(enum adc_channel channel)
{
    *adc_regs[channel].interrupt_register &=~ ADC_CH_INTLVL_LO_gc;
}

inline void enable_adc_interrupt(enum adc_channel channel)
{
    *adc_regs[channel].interrupt_register |= ADC_CH_INTLVL_LO_gc;
}

void init_adc_module(const struct adc_config* const config)
{
    // Select voltage reference
    ADCA.REFCTRL = ADC_REFSEL_INTVCC_gc;

    // Prescale the ADC clock
    ADCA.PRESCALER = config->prescaler;

    // Save pointer to input range and initialize it
    adc_input_range = (struct linear_range*) &config->input_range;
    init_linear_to_midi(adc_input_range);

    // Set conversion mode
    ADCA.CTRLB = config->mode << ADC_CONMODE_bp;

    // Copy calibration values
    NVM.CMD = NVM_CMD_READ_CALIB_ROW_gc;
    ADCA.CALL = pgm_read_byte(offsetof(NVM_PROD_SIGNATURES_t, ADCACAL0));
    NVM.CMD = NVM_CMD_NO_OPERATION_gc;
    NVM.CMD = NVM_CMD_READ_CALIB_ROW_gc;
    ADCA.CALH = pgm_read_byte(offsetof(NVM_PROD_SIGNATURES_t, ADCACAL1));
    NVM.CMD = NVM_CMD_NO_OPERATION_gc;

    // Enable the ADC
    ADCA.CTRLA |= ADC_ENABLE_bm;
}

void init_adc_conversion(const struct adc_conversion_config* const config)
{
    // Save callbacks
    callbacks_signed[config->channel] = config->callback_signed;
    callbacks_unsigned[config->channel] = config->callback_unsigned;

    // Select channel input mode
    *adc_regs[config->channel].control_register = ADC_CH_INPUTMODE_SINGLEENDED_gc;

    // Select input source
    *adc_regs[config->channel].mux_register = config->input;

    // Start a dummy conversion
    trigger_adc(config->channel);
    while (!*adc_regs[config->channel].interrupt_flag);
    *adc_regs[config->channel].interrupt_flag = true;
}

void set_adc_channel0_max_value(void)
{
    adc_input_range->to = compensate_offset(perform_filtered_measurement(ADC_CHANNEL_0));
    init_linear_to_midi(adc_input_range);
}

void set_adc_channel0_min_value(void)
{
    adc_input_range->from = compensate_offset(perform_filtered_measurement(ADC_CHANNEL_0));
    init_linear_to_midi(adc_input_range);
}

void set_adc_offset(uint16_t new_offset)
{
    offset = new_offset;
}

void trigger_adc(enum adc_channel channel)
{
    *adc_regs[channel].mux_register = ADC_CH_MUXPOS_PIN4_gc;
    *adc_regs[channel].control_register |= ADC_CH_START_bm;
}



////////////////////////////////////////////////////////////////
//                    I N T E R R U P T S                     //
////////////////////////////////////////////////////////////////

/// \brief      Interrupt service routine for ADC channel 0
/// \details    This routine is different from the others: It computes a mean value of the last
///             ADC_SAMPLE_BUFFER_SIZE samples, converts the result to a valid MIDI value in the
///             range [0, 127] and passes it to the callback for unsigned values.
///             All this happens in an atomic block.
ISR(ADCA_CH0_vect)
{
    // Disable interrupts
    cli();

    // Copy sample value to buffer
    static uint8_t sample_buffer_index = 0;
    sample_buffer[sample_buffer_index++] = ADCA.CH0RES;
    sample_buffer_index %= ADC_SAMPLE_BUFFER_SIZE;

    // Compute mean value
    adc_accumulator accumulator = 0;
    uint8_t i;
    for (i=0; i < ADC_SAMPLE_BUFFER_SIZE; ++i) {
        accumulator += sample_buffer[i];
    }
    accumulator /= ADC_SAMPLE_BUFFER_SIZE;
    accumulator  = compensate_offset(accumulator);

    // Convert to MIDI value and invoke callback
    uint8_t midi_value = linear_to_midi(adc_input_range, accumulator);
    if (callbacks_unsigned[ADC_CHANNEL_0] != NULL) {
        callbacks_unsigned[ADC_CHANNEL_0](midi_value);
    }

    // Enable interrupts
    sei();
}

/// \brief      Invokes registered callbacks for the given channels
/// \param      channel
///                 the ADC channel whose callbacks should be invoked
/// \param      value
///                 the ADC conversion result
static void invoke_callbacks(enum adc_channel channel, uint16_t value)
{
    value = compensate_offset(value);
    if (callbacks_signed[channel] != NULL) {
        callbacks_signed[channel]((int16_t) value);
    }
    if (callbacks_unsigned[channel] != NULL) {
        callbacks_unsigned[channel](value);
    }
}

/// \brief      Interrupt service routine for ADC channel 1
/// \details    Calls the registered callback(s) for the channel in an atomic block.
ISR(ADCA_CH1_vect)
{
    invoke_callbacks(ADC_CHANNEL_1, ADCA.CH1RES);
}

/// \brief      Interrupt service routine for ADC channel 2
/// \details    Calls the registered callback(s) for the channel in an atomic block.
ISR(ADCA_CH2_vect)
{
    invoke_callbacks(ADC_CHANNEL_2, ADCA.CH2RES);
}

/// \brief      Interrupt service routine for ADC channel 3
/// \details    Calls the registered callback(s) for the channel in an atomic block.
ISR(ADCA_CH3_vect)
{
    invoke_callbacks(ADC_CHANNEL_3, ADCA.CH3RES);
}
