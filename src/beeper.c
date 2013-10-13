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
 * @file beeper.c 
 *
 * @brief Beeper - Tone Generation API implementation
 *
 */


#include <beeper.h>
#include <avr/io.h>


void beeper_init(e_timer a_timer) {

	tdelay_init(a_timer);
	_timer_en_oca(a_timer);

	switch (a_timer) {

#if TDELAY_IMPLEMENT_T0_INT == 1
		case E_TIMER0:
			TCCR0A |= 0x40;
			break;
#endif

#if TDELAY_IMPLEMENT_T1_INT == 1
		case E_TIMER1:
			TCCR1A |= 0x40;
			break;
#endif

#if TDELAY_IMPLEMENT_T2_INT == 1
		case E_TIMER2:
			TCCR2A |= 0x40;
			break;
#endif

		default:
			break;
	} // switch

	/**
	 * @brief inverted logic is used here, because checking for zero produces less code on AVR than for 1 
	 */
	_tdc_set_cmp_pin(_tdc_get_tdelay_timer(a_timer), 0x00);
}


void beeper_beep(e_timer a_timer, 
	uint32_t freq, 
	uint32_t duration) {
	
	beeper_off(a_timer);
	_tdc_setup_delay(_tdc_get_tdelay_timer(a_timer), freq, duration);

	if (freq) {
		switch(a_timer) {

#if TDELAY_IMPLEMENT_T0_INT == 1
			case E_TIMER0:
				TCCR0A |= _BV(COM0A0);
				break;
#endif

#if TDELAY_IMPLEMENT_T1_INT == 1
			case E_TIMER1:
				TCCR1A |= _BV(COM1A0);
				break;
#endif

#if TDELAY_IMPLEMENT_T2_INT == 1
			case E_TIMER2:
				TCCR2A |= _BV(COM2A0);
				break;
#endif
			default:
				break;
		} // switch
	}	

	_timer_en_compa_int(a_timer);
}


void beeper_off(e_timer a_timer) {
	switch (a_timer) {

#if TDELAY_IMPLEMENT_T0_INT == 1
		case E_TIMER0:
			_timer_dis_compa_int(a_timer);
			TCCR0B &= 0xf8;
			TCCR0A &= ~_BV(COM0A0);
			break;
#endif

#if TDELAY_IMPLEMENT_T1_INT == 1
		case E_TIMER1:
			_timer_dis_compa_int(a_timer);
			TCCR1B &= 0xf8;
			TCCR1A &= ~_BV(COM1A0);
			break;
#endif

#if TDELAY_IMPLEMENT_T2_INT == 1
		case E_TIMER2:
			_timer_dis_compa_int(a_timer);
			TCCR2B &= 0xf8;
			TCCR2A &= ~_BV(COM2A0);
			break;
#endif

		default:
			break;
	} // switch
	_tdc_set_duration(_tdc_get_tdelay_timer(a_timer), 0x00);
}


inline void beeper_block(e_timer a_timer) {
	_tdc_block(_tdc_get_tdelay_timer(a_timer));
}
