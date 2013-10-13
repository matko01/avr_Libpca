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
 * @file tpwm.c
 *
 * @brief PWM routines implementation
 *
 */

#include <tpwm.h>

#include <avr/io.h>
#include <avr/power.h>
#include <avr/interrupt.h>


void tpwm_fpwm_init(e_timer a_timer) {
	_timer_init_fpwm(a_timer);

	switch(a_timer) {
		case E_TIMER0:
			TCCR0A = 0x80;
			OCR0A = 0x00;
			break;

		case E_TIMER1:
			power_timer1_enable();
			TCCR1A |= 0x80;
			OCR1AH = OCR1AL = 0x00;
			break;

		case E_TIMER2:
			TCCR2A |= 0x80;
			OCR2A = 0x00;
			break;

		default:
			break;

	} // switch


	_timer_en_oca(a_timer);
}


void tpwm_pwm_init(e_timer a_timer) {
	_timer_init_pwm(a_timer);
	
	switch(a_timer) {
		case E_TIMER0:
			TCCR0A |= 0x80;			
			OCR0A = 0x00;
			break;

		case E_TIMER1:
			TCCR1A |= 0x80;
			OCR1AH = OCR1AL = 0x00;
			break;

		case E_TIMER2:
			TCCR2A |= 0x80;
			OCR2A = 0x00;
			break;

		default:
			break;

	} // switch

	_timer_en_oca(a_timer);
}


uint32_t tpwm_setup_for_bitres(e_timer a_timer, uint8_t a_bitres) {
	if (E_TIMER1 == a_timer) {
		uint16_t presc = ((1 << a_bitres) - 1);

		// prescaler 1
		TCCR1B |= 0x01;	
		ICR1H = (presc >> 8) & 0xff;
		ICR1L = presc & 0xff;
		return (uint32_t)(F_CPU/(1 << a_bitres));
	}
	
	/* prescaler = 1 */
	if (E_TIMER0 == a_timer) {
		TCCR0B |= 0x01;
	}
	else {
		TCCR2B |= 0x01;
	}

	return F_CPU / 256;
}


uint8_t tpwm_setup_for_freq(e_timer a_timer, e_pwm_freq a_freq) {

	if (E_TIMER1 == a_timer) {
		if (E_PWM_FREQ_61 == a_freq)
			return 0;

		tpwm_setup_for_bitres(a_timer, a_freq);
		return a_freq;
	}

	uint8_t presc = 0x00;

	if (E_TIMER0 == a_timer) {
		switch(a_freq) {
			case E_PWM_FREQ_62500:				
				presc = 1;
				break;

			case E_PWM_FREQ_7812:
				presc = 2;
				break;

			case E_PWM_FREQ_976:
				presc = 3;
				break;

			case E_PWM_FREQ_244:
				presc = 4;
				break;

			case E_PWM_FREQ_61:
				presc = 5;
				break;

			default:
				return 0;
				break;
		}

		TCCR0B |= (presc & 0x07);
	}
	else {
		switch(a_freq) {
			case E_PWM_FREQ_62500:				
				presc = 1;
				break;

			case E_PWM_FREQ_7812:
				presc = 2;
				break;

			case E_PWM_FREQ_1953:
				presc = 3;
				break;

			case E_PWM_FREQ_976:
				presc = 4;
				break;

			case E_PWM_FREQ_488:
				presc = 5;
				break;

			case E_PWM_FREQ_244:
				presc = 6;
				break;

			case E_PWM_FREQ_61:
				presc = 7;
				break;

			default:
				return 0;
				break;
		}
		TCCR2B |= (presc & 0x07);
	}

	return 8;
}
