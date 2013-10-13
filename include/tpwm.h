#ifndef TPWM_H_IUK1KDC0
#define TPWM_H_IUK1KDC0

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
 * @file tpwm.h 
 *
 * @brief Common API for configuring timers as PWM generators
 *
 * This file provides a common API for timer to provide a PWM functionality.
 * At the moment this API is incomplete although functional
 * 
 * @example timer_02.c
 *
 * Configure timer as PWM generator
 */


#include <config.h>
#include <common.h>
#include <timer_common.h>


/**
 * @brief frequencies allowed to be configured
 */
typedef enum _e_pwm_freq {
	E_PWM_FREQ_4M = 2,	// 2 
	E_PWM_FREQ_2M,		// 3
	E_PWM_FREQ_1M,		// 4
	E_PWM_FREQ_500K,	// 5
	E_PWM_FREQ_250K,	// 6
	E_PWM_FREQ_125K,	// 7
	E_PWM_FREQ_62500,	// 8
	E_PWM_FREQ_31250,	// 9
	E_PWM_FREQ_15625,	// 10
	E_PWM_FREQ_7812,	// 11
	E_PWM_FREQ_3906,	// 12
	E_PWM_FREQ_1953,	// 13
	E_PWM_FREQ_976,		// 14
	E_PWM_FREQ_488,		// 15
	E_PWM_FREQ_244,		// 16
	E_PWM_FREQ_61		// 8 bit timers
} e_pwm_freq;


/**
 * @brief Configure selected timer to single slope fast PWM mode
 *
 * @param a_timer timer to be configured
 */
void tpwm_fpwm_init(e_timer a_timer);


/**
 * @brief Configure selected timer to double slope Phase Correct PWM mode
 *
 * @param a_timer timer to be configured
 */
void tpwm_pwm_init(e_timer a_timer);


/**
 * @brief setup timer, to provide a requested PWM resolution (frequency will be chosen automatically)
 *
 * @param a_timer timer to be configured (must be initialized first)
 * @param a_bitres requested bit resolution
 *
 * @return frequency configured
 */
uint32_t tpwm_setup_for_bitres(e_timer a_timer, uint8_t a_bitres);


/**
 * @brief setup timer, to provide requested PWM frequency (bit resolution will be chosen automatically)
 *
 * @param a_timer timer to be configured (must be initialized first)
 * @param a_freq requested frequency
 *
 * @return bit resolution configured
 */
uint8_t tpwm_setup_for_freq(e_timer a_timer, e_pwm_freq a_freq);

#endif /* TPWM_H_IUK1KDC0 */

