#ifndef ADC_H_W6GMC45G
#define ADC_H_W6GMC45G


/* Copyright (C) 
 * 2013 - Tomasz Wisniewski
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 */


/**
 * @file adc.h 
 *
 * @brief API for Atmega328p ADC setup
 *
 * This file provides a basic API to use the Atmega328p analog to digital converter
 *
 * @example adc_01.c
 *
 */


#include "common.h"
#include <avr/io.h>
#include <stdio.h>

/**
 * @brief assemble the measurements results in the ADC registers
 * into a single 16 bit value
 *
 * @param __data measurement result
 *
 */
#define _adc_result_assemble(__data) \
	__data = ADCL; \
	__data |= (ADCH << 8)




/**
 * @brief enumeration defining possible analog reference voltage sources
 */
typedef enum _e_adc_ref {
	// voltage provided to external AREF pin
	E_ADC_EXTERNAL_AREF = 0x00,

	// use analog voltage Vcc as reference (5V)
	E_ADC_EXTERNAL_AVCC,
	
	/// internal 1.1 analog reference will be used
	E_ADC_REF_INTERNAL_11 = 0x03
} e_adc_ref;


/**
 * @brief possible ADC operational modes
 */
typedef enum _e_adc_mode {
	/// single shot mode, ADC won't be re-triggered
	E_SINGLE_SHOT = 0,

	/// free running mode. ADC will be re-triggered constantly after every conversion
	E_AT_FREERUN,

	/// analog comparator re-trigger
	E_AT_AC,

	/// external interrupt re-trigger
	E_AT_EINT0,

	/// Timer0 Compare A re-trigger
	E_AT_TIMER0_COMPA,

	/// Timer0 Overflow re-trigger
	E_AT_TIMER0_OVFL,

	/// Timer1 Compare B re-trigger
	E_AT_TIMER1_COMPB,

	/// Timer1 Overflow re-trigger
	E_AT_TIMER1_OVFL,

	/// Timer1 Capture event re-trigger
	E_AT_TIMER1_CAPTURE_EVENT,

	/// enumeration counter
	E_AT_MODE_LAST
} e_adc_mode;


/**
 * @brief initialize ADC to most generic settings
 *
 * Enables power to ADC peripheral, selects channel 0,
 * disables ADC interrupt and selects the F_CPU/128 prescaler
 *
 * @param a_mode mode to which ADC will be configured
 */
void adc_init(e_adc_mode a_mode);


/**
 * @brief set ADC multiplexer to a specified channel
 *
 * @param a_channel channel (0-8)
 */
#define adc_channel_set(__channel) \
	ADMUX &= 0xf0; \
	ADMUX |= (__channel & 0x0f)


/**
 * @brief set ADC analog reference voltage source
 *
 * @param a_ref reference voltage source
 */
#define adc_reference_set(__ref) \
	ADMUX &= 0x3f; \
	ADMUX |= ((__ref & 0x03) << 6)


/**
 * @brief configure ADC to take measurements out of internal
 *  ADC temperature sensor
 */
void adc_temperature_sensor_enable();


/**
 * @brief enable digital input buffer on the ADC port
 *
 * @param __input port number
 *
 */
#define adc_di_enable(__input) \
	DIDR0 &= ~_BV(__input)


/**
 * @brief disable digital input buffer on the ADC port
 *
 * @param __input input port number
 *
 */
#define adc_di_disable(__input) \
	DIDR0 |= _BV(__input)


/**
 * @brief Configure the ADCs prescaler
 *
 * @param __prescaler prescaler
 *
 */
#define adc_prescaler_set(__prescaler) \
	ADCSRA &= 0xf8; \
	ADCSRA |= (__prescaler & 0x07)


/**
 * @brief Enable ADC interrupt 
 */
#define adc_interrupt_enable() \
	ADCSRA |= _BV(ADIE)
	

/**
 * @brief Disable ADC interrupt
 */
#define adc_interrupt_disable() \
	ADCSRA &= ~_BV(ADIE)


/**
 * @brief trigger the conversion in manual mode or trigger the first conversion when
 *  free running mode is enabled
 */
#define adc_conversion_trigger() \
	ADCSRA |= _BV(ADSC)


/**
 * @brief read the convertion result from ADC. Wait until ADSC flag becomes 0
 *
 * @return ADC conversion result
 */
#define adc_result_poll_get(__result) \
	while ((ADCSRA & _BV(ADSC))); \
	_adc_result_assemble(__result)


/**
 * @brief read the convertion result from ADC
 *
 * @return ADC conversion result
 */
#define adc_result_get(__result) \
	_adc_result_assemble(__result)



#endif /* end of include guard: ADC_H_W6GMC45G */

