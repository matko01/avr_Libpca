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
 * @param e_timer timer
 * @param a_delay delay in miliseconds
 */
void tdelay_ms(e_timer, uint32_t a_delay);

/**
 * @brief delay execution using a given timer
 *
 * @param e_timer timer
 * @param a_delay delay in microseconds
 */
void tdelay_us(e_timer, uint32_t a_delay);

#endif /* end of include guard: TDELAY_H_IPOQKAD0 */
