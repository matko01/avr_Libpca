#ifndef TDELAY_COMMON_H_NXGPZAZA
#define TDELAY_COMMON_H_NXGPZAZA

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

#include "config.h"
#include "common.h"
#include "timer_common.h"


/**
 * @brief Since we do not have to use all timers for delays, there is
 *  no need to implement and declare interrupts and data structures for
 * 	every timer. That's why there is a need for tdelay specific denomination
 */
typedef enum _e_tdelay_timer {

#if TDELAY_IMPLEMENT_T0_INT == 1
	E_TDELAY_TIMER0,
#endif

#if TDELAY_IMPLEMENT_T1_INT == 1	
	E_TDELAY_TIMER1,
#endif

#if TDELAY_IMPLEMENT_T2_INT == 1	
	E_TDELAY_TIMER2,
#endif

	E_TDELAY_TIMER_LAST
} e_tdelay_timer;


/**
 * @brief convert an e_timer to e_tdelay_timer
 *
 * @param a_timer e_timer
 *
 * @return e_tdelay_timer
 */
e_tdelay_timer _tdc_get_tdelay_timer(e_timer a_timer);

/**
 * @brief convert e_tdelay_timer to e_timer denominator
 *
 * @param a_timer e_tdelay_timer
 *
 * @return e_timer
 */
e_timer _tdc_get_timer(e_tdelay_timer a_timer);

/**
 * @brief configure the duration for a given timer
 *
 * @param a_tim timer
 * @param a_dur duration value (in cycles)
 */
void _tdc_set_duration(e_tdelay_timer a_tim, uint32_t a_dur);

/**
 * @brief set to 1 if you want to toggle OCMPXA PIN down, once timer expires
 *
 * @param a_tim timer
 * @param a_pin 1 - pull down, 0 - do nothing
 */
void _tdc_set_cmp_pin(e_tdelay_timer a_tim, uint8_t a_pin);

/**
 * @brief configure timer for a specified frequency and duration
 *
 * @param a_timer timer
 * @param a_freq freq
 * @param a_delay duration
 */
void _tdc_setup_delay(e_tdelay_timer a_timer, uint32_t a_freq, uint32_t a_delay);

/**
 * @brief block until timer expires (must be configured before)
 *
 * @param a_timer time
 */
void _tdc_block(e_tdelay_timer a_timer);

#endif /* end of include guard: TDELAY_COMMON_H_NXGPZAZA */

