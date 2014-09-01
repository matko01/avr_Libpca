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
 * @file common.c 
 *
 * @brief General purpose API's implementation
 *
 */

#include <common.h>
#include <avr/pgmspace.h>


// 8x8 Bayer ordered dithering  
// pattern.  Each input pixel   
// is scaled to the 0..63 range 
// before looking in this table 
// to determine the action.     
static const uint8_t _bayer[8][8] PROGMEM = {
    { 0, 32,  8, 40,  2, 34, 10, 42},
    {48, 16, 56, 24, 50, 18, 58, 26},
    {12, 44,  4, 36, 14, 46,  6, 38},
    {60, 28, 52, 20, 62, 30, 54, 22},
    { 3, 35, 11, 43,  1, 33,  9, 41},
    {51, 19, 59, 27, 49, 17, 57, 25},
    {15, 47,  7, 39, 13, 45,  5, 37},
    {63, 31, 55, 23, 61, 29, 53, 21} 
};


uint16_t common_memory_left () {
  extern int __heap_start, *__brkval; 
  unsigned int v; 
  return (unsigned int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}


uint32_t common_abs(int32_t a_value) {
	int32_t temp = 0x00;

	temp = a_value >> 31;
	a_value ^= temp;
	a_value -= temp;
	
	return a_value;
}


uint32_t common_swap(uint32_t a_value) {
	uint8_t *p = (uint8_t *)&a_value;
	EXCHANGE(p[0], p[3]);
	EXCHANGE(p[1], p[2]);
	return a_value;
}


uint8_t common_ditherPoint(uint8_t c, uint8_t x, uint8_t y) {
	return (c > pgm_read_byte(&_bayer[ x & 0x07 ][ y & 0x07 ]));
}
