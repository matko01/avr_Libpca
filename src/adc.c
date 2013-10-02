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


void adc_init() {

	power_adc_enable();

}



void adc_channel_set(uint8_t a_channel) {

}


void adc_reference_set(e_adc_ref a_ref) {
	
}
