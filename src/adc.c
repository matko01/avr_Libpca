#include "adc.h"

#include <avr/power.h>


/**
 * @file adc.c 
 *
 * @brief implementation of ADC API
 *
 * This file provides an API implementation of the ADC routines
 *
 */


void adc_init(e_adc_mode a_mode) {

	// enable power to ADC
	power_adc_enable();

	// default prescaler = 128. The ADC clock = F_CPU/128 = 125 kHz
	ADCSRA = (_BV(ADEN) | 0x07);

	// configure mode
	if (E_SINGLE_SHOT == a_mode) {
		ADCSRB = 0x00;			
	}
	else {
		// set the appropriate mode
		ADCSRB = (a_mode - 1);
		// enable auto-trigger mode
		ADCSRA = _BV(ADATE);
	}

	// set data format, internal reference and channel zero by default
	ADMUX = 0x00;
	ADMUX &= ~_BV(ADLAR);
}


void adc_temperature_sensor_enable() {
	adc_channel_set(0x08);
	adc_reference_set(E_ADC_REF_INTERNAL_11);
}
