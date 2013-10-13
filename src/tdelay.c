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
 * @file tdelay.c
 *
 * @brief Timer Delay implementation
 *
 */

#include <tdelay.h>

#include <avr/power.h>
#include <avr/interrupt.h>

/* ================================================================================ */


/**
 * @brief Configure selected timer for a  specified delay with 1ms precision 
 *
 * @param a_timer timer
 * @param a_delay delay requested
 */
static void _tdelay_setup_ms(e_timer a_timer, uint32_t a_delay) {
	_tdc_setup_delay(_tdc_get_tdelay_timer(a_timer), 1000, a_delay);
}


/**
 * @brief Configure selected timer for a  specified delay with 1us precision 
 *
 * @param a_timer timer
 * @param a_delay delay requested
 */
static void _tdelay_setup_us(e_timer a_timer, uint32_t a_delay) {
	_tdc_setup_delay(_tdc_get_tdelay_timer(a_timer), 1000000, a_delay);
}

/* ================================================================================ */

void tdelay_init(e_timer a_timer) {
	_timer_init_ctc(a_timer);
	_tdc_set_cmp_pin(_tdc_get_tdelay_timer(a_timer), 0x01);

	// enable interrupts
	sei();
}


void tdelay_ms(e_timer a_timer, uint32_t a_delay) {
	_tdelay_setup_ms(a_timer, a_delay);
	_timer_en_compa_int(a_timer);
	_tdc_block(_tdc_get_tdelay_timer(a_timer));
}


void tdelay_us(e_timer a_timer, uint32_t a_delay) {
	_tdelay_setup_us(a_timer, a_delay);
	_timer_en_compa_int(a_timer);
	_tdc_block(_tdc_get_tdelay_timer(a_timer));
}

