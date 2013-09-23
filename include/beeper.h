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


/**
 * @file beeper.h 
 *
 * @brief API for generating tones using timer(s).
 *
 * This file provides a basic API to use the Atmega328p timers as tone generators.
 *
 * In order to use this API an interrupt implementation must be enabled in config.h for a requested timer (TDELAY_IMPLEMENT_T0_INT, TDELAY_IMPLEMENT_T1_INT, TDELAY_IMPLEMENT_T2_INT).
 * For details please refer to the documentation of config.h options
 *
 * @example beeper_01.c
 *
 * General beeper API usage example
 *
 * @example beeper_02.c
 *
 * Using beeper_block() and beeper_off() functions
 */

#include "config.h"
#include "tdelay.h"


/**
 * @brief block execution until the tone fades out
 *
 * @param a_timer timer used for tone generation
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
 *  (may be 0 - will allow to use bepper_block for an amount of timer specified in duration argument)
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

