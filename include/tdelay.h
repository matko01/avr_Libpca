#ifndef TDELAY_H_IPOQKAD0
#define TDELAY_H_IPOQKAD0

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
 * @file tdelay.h 
 *
 * @brief API for timer delays
 *
 * This file provides a basic API for timer based delays.
 *
 * In order to use this API an interrupt implementation must be enabled in config.h for a requested timer (TDELAY_IMPLEMENT_T0_INT, TDELAY_IMPLEMENT_T1_INT, TDELAY_IMPLEMENT_T2_INT).
 * For details please refer to the documentation of config.h options
 *
 * This API works in general the same as beeper API. The only difference between those two is that beeper API in addition toggles the OCXA pin of the associated timer in order to
 * generate the requested frequency on the pin. tdelay API does not configure the OCXA pin as output. For all timers only TIMERX_COMPA_vect interrupt is implemented. This implementation
 * does not take care about COMPB interrupts at all.
 *
 * @example tdelay_01.c
 *
 * General tdelay API usage example
 *
 * @example tdelay_02.c
 *
 * Using tdelay API in conjunction with beeper API
 */

#include "config.h"
#include "common.h"
#include "tdelay_common.h"


/**
 * @brief initialize a selected timer to act as a delay timer
 *
 * @param a_timer timer
 */
void tdelay_init(e_timer a_timer);


/**
 * @brief delay execution using a given timer
 *
 * @param a_timer timer
 * @param a_delay delay in miliseconds
 */
void tdelay_ms(e_timer a_timer, uint32_t a_delay);

/**
 * @brief delay execution using a given timer
 *
 * @param a_timer timer
 * @param a_delay delay in microseconds
 */
void tdelay_us(e_timer a_timer, uint32_t a_delay);

#endif /* end of include guard: TDELAY_H_IPOQKAD0 */
