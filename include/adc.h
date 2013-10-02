#ifndef ADC_H_W6GMC45G
#define ADC_H_W6GMC45G


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
 * @file adc.h 
 *
 * @brief API for Atmega328p ADC setup
 *
 * This file provides a basic API to use the Atmega328p analog to digital converter
 *
 */


void adc_init();
void adc_select_channel(uint8_t a_channel);


#endif /* end of include guard: ADC_H_W6GMC45G */

