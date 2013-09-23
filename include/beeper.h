#ifndef BEEPER_H_URHFWD8M
#define BEEPER_H_URHFWD8M

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
#include "tdelay.h"

/**
 * @brief block execution until the tone fades out
 *
 * @param __a_timer timer used for tone generation
 *
 * @return void
 */
void beeper_block(e_timer a_timer);

/**
 * @brief initialize a given timer to act as a tone generator
 *
 * @param a_timer timer
 */
void beeper_init(e_timer a_timer);

/**
 * @brief generate a tone of specific frequency and duration using a timer
 *
 * @param a_timer timer 
 * @param freq frequency in Hz 
 * @param duration duration in milliseconds
 */
void beeper_beep(e_timer a_timer, uint32_t freq, uint32_t duration);

/**
 * @brief cancel tone generation 
 *
 * @param a_timer timer
 */
void beeper_off(e_timer a_timer);


#endif /* end of include guard: BEEPER_H_URHFWD8M */

