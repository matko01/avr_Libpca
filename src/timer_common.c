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
 * @file timer_common.c
 *
 * @brief Timer common routines implementation
 *
 */

#include <timer_common.h>
#include <avr/interrupt.h>
#include <avr/power.h>

void _timer_en_compa_int(e_timer a_timer) {
	// which pin
	switch(a_timer) {
		case E_TIMER0:
			TIMSK0 |= _BV(OCIE0A);
			break;

		case E_TIMER1:
			TIMSK1 |= _BV(OCIE1A);
			break;

		case E_TIMER2:
			TIMSK2 |= _BV(OCIE2A);
			break;

		default:
			break;
	} // switch
}


void _timer_dis_compa_int(e_timer a_timer) {
	// which pin
	switch(a_timer) {
		case E_TIMER0:
			TIMSK0 &= ~_BV(OCIE0A);
			break;

		case E_TIMER1:
			TIMSK1 &= ~_BV(OCIE1A);
			break;

		case E_TIMER2:
			TIMSK2 &= ~_BV(OCIE2A);
			break;

		default:
			break;
	} // switch
}

// they are outside in order not to declare them every time when they function
// is called very often
static const uint8_t _g_prescalers01[] = { 0x00, 0x03, 0x06, 0x08, 0x0a, 0x00 };
static const uint8_t _g_prescalers2[] = { 0x00, 0x03, 0x05, 0x06, 0x07, 0x08, 0x0a, 0x00};


uint32_t _timer_freq_prescale(e_timer a_timer, uint32_t a_freq, uint16_t a_criterion) {

	/**
	 * combine both prescaler and ocr value in one 32bit value 
	 */
	uint32_t retval = 0x00;
	uint16_t *ocr = (uint16_t *)&retval;
	uint8_t *presc = ((uint8_t *)&retval) + 3;

	uint32_t ocr2 = 0;
	const uint8_t *prescalers = 
		(a_timer == E_TIMER2 ? _g_prescalers2 : _g_prescalers01);

	do {
		ocr2 = (uint32_t)(F_CPU / ((a_freq << 1) * (0x01 << prescalers[*presc])));
		if (ocr2) --ocr2;
		++*presc;		
	} while ((ocr2 > a_criterion) && (prescalers[*presc]));

	if (ocr2 > a_criterion) ocr2 = a_criterion;
	*ocr = ocr2 & 0xffff;
	return retval;
}


void _timer_init_fpwm(e_timer a_timer) {

	switch(a_timer) {
		case E_TIMER0:
			power_timer0_enable();
			TCCR0A = 0x03;
			TCCR0B = 0x00;
			OCR0A = 0x00;
			break;

		case E_TIMER1:
			power_timer1_enable();
			TCCR1A = 0x02;
			TCCR1B = 0x18;
			OCR1AH = OCR1AL = 0x00;
			break;

		case E_TIMER2:
			power_timer2_enable();
			ASSR &= ~(_BV(EXCLK) | _BV(AS2));
			TCCR2A = 0x03;
			TCCR2B = 0x00;
			OCR2A = 0x00;
			break;

		default:
			break;
	} // switch
}


void _timer_init_pwm(e_timer a_timer) {

	switch(a_timer) {
		case E_TIMER0:
			power_timer0_enable();
			TCCR0A = 0x01;
			TCCR0B = 0x00;
			OCR0A = 0x00;
			break;

		case E_TIMER1:
			power_timer1_enable();
			TCCR1A = 0x00;
			TCCR1B = 0x10;
			OCR1AH = OCR1AL = 0x00;
			break;

		case E_TIMER2:
			power_timer2_enable();
			ASSR &= ~(_BV(EXCLK) | _BV(AS2));
			TCCR2A = 0x01;
			TCCR2B = 0x00;
			OCR2A = 0x00;
			break;

		default:
			break;
	} // switch
}


void _timer_init_ctc(e_timer a_timer) {

	switch (a_timer) {
		case E_TIMER0:
			power_timer0_enable();
			// clock disabled
			TCCR0A = 0x02;
			TCCR0B = 0x00;
			TCNT0 = 0x00;
			OCR0A = 0x00;
			break;

		case E_TIMER1:
			power_timer1_enable();
			// clock disabled, CTC mode
			TCCR1A = 0x00;
			TCCR1B = 0x08;
			TCNT1H = 0x00;
			TCNT1L = 0x00;
			OCR1AH = 0x00;
			OCR1AL = 0x00;
			break;

		case E_TIMER2:
			power_timer2_enable();
			TCCR2A = 0x02;
			TCCR2B = 0x00;
			TCNT2 = 0x00;
			OCR2A = 0x00;
			break;

		default:
			break;
	} // switch
}


void _timer_setup_ctc(e_timer a_timer, uint32_t a_pocr) {

	switch(a_timer) {
		case E_TIMER0:
			TCCR0B &= 0xf8;
			TCCR0B |= (a_pocr >> 24) & 0x07;			
			OCR0A = a_pocr & 0xff;
			TCNT0 = 0x00;
			break;

		case E_TIMER1:
			TCCR1B &= 0xf8;
			TCCR1B |= ((a_pocr >> 24) & 0x07);
			OCR1A = (a_pocr & 0xffff);
			TCNT1H = TCNT1L = 0x00;
			break;

		case E_TIMER2:
			TCCR2B &= 0xf8;
			TCCR2B |= (a_pocr >> 24) & 0x07;			
			OCR2A = a_pocr & 0xff;
			TCNT2 = 0x00;
			break;

		default:
			break;
	} // switch
}


void _timer_en_oca(e_timer a_timer) {
	switch (a_timer) {
		case E_TIMER0:
			DDRD |= _BV(PORTD6);
			break;

		case E_TIMER1:
			DDRB |= _BV(PORTB1);
			break;

		case E_TIMER2:
			DDRB |= _BV(PORTB3);
			break;

		default:
			break;
	} // switch
}


void _timer_en_ocb(e_timer a_timer) {
	switch (a_timer) {
		case E_TIMER0:
			DDRD |= _BV(PORTD5);
			break;

		case E_TIMER1:
			DDRB |= _BV(PORTB2);
			break;

		case E_TIMER2:
			DDRD |= _BV(PORTD3);
			break;

		default:
			break;
	} // switch
}


