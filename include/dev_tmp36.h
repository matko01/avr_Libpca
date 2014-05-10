#ifndef DEV_TMP36_H_RSHVECXP
#define DEV_TMP36_H_RSHVECXP

/* Copyright (C) 
 * 2014 - Tomasz Wisniewski
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

/**
 * @brief utility function to convert the tmp36 ADC reading to the Celsius temperature
 *
 */
#define tmp36_raw_to_celsius(__reading, __adc_max, __adc_vref) \
	(((double)(__reading)/(__adc_max - 1))*__adc_vref - 500.0f)/10

#endif /* DEV_TMP36_H_RSHVECXP */

