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
		ADCSRB = (a_mode - 1);
		// enable auto-trigger mode
		ADCSRA = _BV(ADATE);
	}

	// set data format, internal reference and channel zero by default
	ADMUX = 0x00;
	ADMUX &= ~_BV(ADLAR);
}


void adc_channel_set(uint8_t a_channel) {
	ADMUX = (a_channel & 0x0f) | (ADMUX & 0xf0);
}


void adc_reference_set(e_adc_ref a_ref) {
	ADMUX = (ADMUX & 0x3f) | a_ref;
}


uint16_t adc_result_get() {
	uint16_t res = 0x00;
	_adc_result_assemble(res);
	return res;	
}


void adc_conversion_trigger() {
	ADCSRA |= _BV(ADSC);
	while ((ADCSRA & _BV(ADSC)));
}


void adc_temperature_sensor_enable() {
	adc_channel_set(0x08);
	adc_reference_set(E_ADC_REF_INTERNAL_11);
}
