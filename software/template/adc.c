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
#include "pwm.h"

#include <avr/interrupt.h>


////////////////////////////////////////////////////////////////
//                     V A R I A B L E S                      //
////////////////////////////////////////////////////////////////

uint16_t last_midi_value = 0;



////////////////////////////////////////////////////////////////
//      F U N C T I O N S   A N D   P R O C E D U R E S       //
////////////////////////////////////////////////////////////////

// initialization and endless loop
void configureADC(void)
{
    // select voltage reference
    ADMUX = _BV(REFS0);

    // select input channel
    ADMUX |= 0;

    // prescale the ADC clock
    ADCSRA = _BV(ADPS1) | _BV(ADPS0);

    // enable the ADC
    ADCSRA |= _BV(ADEN);

    // disable unused ADC input channels
    DIDR0 = _BV(ADC5D) | _BV(ADC4D) | _BV(ADC3D) | _BV(ADC2D);

    // start a dummy conversion
    ADCSRA |= _BV(ADSC);
    while (ADCSRA & _BV(ADSC));
    ADCSRA |= _BV(ADIF);

    // enable the ADC interrupt
    ADCSRA |= _BV(ADIE);
}

void triggerADC(void)
{
    ADCSRA |= _BV(ADSC);
}



////////////////////////////////////////////////////////////////
//                    I N T E R R U P T S                     //
////////////////////////////////////////////////////////////////

ISR(ADC_vect)
{
    // disable interrupts
    cli();

    // compute MIDI value
    uint32_t midiValue = ADCW;
    midiValue *= MIDI_MAX_VALUE;
    midiValue /= 1024;

    // send MIDI command if the value has changed
    if (midiValue != last_midi_value) {
        last_midi_value = midiValue;
        sendControlChange(CTRL_CUTOFF, midiValue);
    }

    // set PWM duty cycle
    applyDutyCycle(midiValue);

    // enable interrupts
    sei();
}
