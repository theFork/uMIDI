/*
 * Copyright 2014,2015 Sebastian Neuser
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

/*
 * ADC module of the uMIDI firmware
*/

#include "adc.h"
#include "midi.h"
#include "leds.h"

#include <stddef.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

struct adc_channel_regs {
    volatile uint8_t*   control_register;
    volatile uint8_t*   mux_register;
    volatile uint8_t*   interrupt_register;
    volatile uint8_t*   interrupt_flag;
    volatile uint16_t*  result_register;
};

static const struct adc_channel_regs adc_regs[] = {
    {
        .control_register   = &ADCA_CH0_CTRL,
        .mux_register       = &ADCA_CH0_MUXCTRL,
        .interrupt_register = &ADCA_CH0_INTCTRL,
        .interrupt_flag     = &ADCA_CH0_INTFLAGS,
        .result_register    = &ADCA.CH0RES
    },
    {
        .control_register   = &ADCA_CH1_CTRL,
        .mux_register       = &ADCA_CH1_MUXCTRL,
        .interrupt_register = &ADCA_CH1_INTCTRL,
        .interrupt_flag     = &ADCA_CH1_INTFLAGS,
        .result_register    = &ADCA.CH1RES
    },
};

static uint16_t offset;
static uint16_t sample_buffer[ADC_SAMPLE_BUFFER_SIZE];
static void (*callbacks[sizeof(adc_regs)/sizeof(struct adc_channel_regs)])(uint8_t);



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void calibrate_adc_offset(enum adc_channel channel)
{
    adc_accumulator accumulator = 0;
    uint8_t i;
    for (i=0; i<ADC_SAMPLE_BUFFER_SIZE; i++) {
        trigger_adc(channel);
        while (!*adc_regs[channel].interrupt_flag);
        *adc_regs[channel].interrupt_flag = true;
        accumulator += *adc_regs[channel].result_register;
        sample_buffer[i] = *adc_regs[channel].result_register;
    }
    offset = accumulator / ADC_SAMPLE_BUFFER_SIZE;
}

inline void enable_adc_interrupt(enum adc_channel channel)
{
    *adc_regs[channel].interrupt_register |= ADC_CH_INTLVL_LO_gc;
}

inline void disable_adc_interrupt(enum adc_channel channel)
{
    *adc_regs[channel].interrupt_register &=~ ADC_CH_INTLVL_LO_gc;
}

void initialize_adc_module(const struct adc_config* config, const struct adc_conversion_config* primary_conversion)
{
    // Select voltage reference
    ADCA.REFCTRL = ADC_REFSEL_INTVCC_gc;

    // Prescale the ADC clock
    ADCA.PRESCALER = config->prescaler;

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

    initialize_adc_conversion(primary_conversion);

    // Measure offset and initialize sample buffer
    calibrate_adc_offset(primary_conversion->channel);
}

void initialize_adc_conversion(const struct adc_conversion_config* config)
{
    // Save callback
    callbacks[config->channel] = config->callback;

    // Configure input pin
    PORTA.DIRCLR = _BV(config->channel);

    // Select channel input mode
    *adc_regs[config->channel].control_register = ADC_CH_INPUTMODE_SINGLEENDED_gc;

    // Select input source
    *adc_regs[config->channel].mux_register = config->input;

    // Start a dummy conversion
    trigger_adc(config->channel);
    while (!*adc_regs[config->channel].interrupt_flag);
    *adc_regs[config->channel].interrupt_flag = true;
}

void trigger_adc(enum adc_channel channel)
{
    *adc_regs[channel].mux_register = ADC_CH_MUXPOS_PIN4_gc;
    ADCA_CH0_CTRL |= ADC_CH_START_bm;
}



////////////////////////////////////////////////////////////////
//                    I N T E R R U P T S                     //
////////////////////////////////////////////////////////////////

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

    // Compensate offset and prevent integer overflow
    accumulator -= offset;
    if (accumulator >= 1<<ADC_RESOLUTION) {
        accumulator = 0;
    }

    // Shift to MIDI value and update
    uint8_t midi_value = accumulator >> (ADC_RESOLUTION-7);
    callbacks[0](midi_value);

    // Enable interrupts
    sei();
}
