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

extern struct wave pwm_wave;

static uint16_t offset;
static uint16_t sample_buffer[ADC_SAMPLE_BUFFER_SIZE];



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

void calibrate_adc_offset(void)
{
    adc_accumulator accumulator = 0;
    uint8_t i;
    for (i=0; i<ADC_SAMPLE_BUFFER_SIZE; i++) {
        trigger_adc();
        while (!ADCA_CH0_INTFLAGS);
        ADCA_CH0_INTFLAGS = true;
        accumulator += ADCA.CH0RES;
        sample_buffer[i] = ADCA.CH0RES;
    }
    offset = accumulator / ADC_SAMPLE_BUFFER_SIZE;
}

inline void enable_adc_interrupt(void)
{
    ADCA_CH0_INTCTRL |= ADC_CH_INTLVL_LO_gc;
}

inline void disable_adc_interrupt(void)
{
    ADCA_CH0_INTCTRL &=~ ADC_CH_INTLVL_LO_gc;
}

void initialize_adc_module(void)
{
    // Configure input pin
    PORTA.DIRCLR = _BV(ADC_INPUT_PIN);
    PORTA.DIRCLR = _BV(ADC_VREF_PIN);

    // Select voltage reference
    ADCA.REFCTRL = ADC_REFSEL_INTVCC_gc;

    // Prescale the ADC clock
    ADCA.PRESCALER = ADC_PRESCALER_DIV512_gc;

    // Copy calibration values
    NVM.CMD = NVM_CMD_READ_CALIB_ROW_gc;
    ADCA.CALL = pgm_read_byte(offsetof(NVM_PROD_SIGNATURES_t, ADCACAL0));
    NVM.CMD = NVM_CMD_NO_OPERATION_gc;
    NVM.CMD = NVM_CMD_READ_CALIB_ROW_gc;
    ADCA.CALH = pgm_read_byte(offsetof(NVM_PROD_SIGNATURES_t, ADCACAL1));
    NVM.CMD = NVM_CMD_NO_OPERATION_gc;

    // Enable the ADC
    ADCA.CTRLA |= ADC_ENABLE_bm;

    // Select channel input mode
    ADCA_CH0_CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc;

    // Select input source
    ADCA_CH0_MUXCTRL = ADC_CH_MUXPOS_PIN4_gc;

    // Start a dummy conversion
    trigger_adc();
    while (!ADCA_CH0_INTFLAGS);
    ADCA_CH0_INTFLAGS = true;

    // Measure offset and initialize sample buffer
    calibrate_adc_offset();
}

void trigger_adc(void)
{
    ADCA_CH0_CTRL |= ADC_CH_START_bm;
}



////////////////////////////////////////////////////////////////
//                    I N T E R R U P T S                     //
////////////////////////////////////////////////////////////////

static void update_expression_value(uint8_t new_value) {
    static uint8_t old_value = 0;
    if (new_value != old_value) {
        old_value = new_value;
        flash_led(LED_RED);
        send_control_change(69, new_value);
    }
}

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
    update_expression_value(midi_value);

    // Enable interrupts
    sei();
}
