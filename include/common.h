#ifndef __COMMON_H__
#define __COMMON_H__

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

#include <stdint.h>

// ================================================================================


/**
 * volatile fixed size data types declaration
 */
typedef volatile int8_t vint8_t;
typedef volatile uint8_t vuint8_t;
typedef volatile int16_t vint16_t;
typedef volatile uint16_t vuint16_t;
typedef volatile int32_t vint32_t;
typedef volatile uint32_t vuint32_t;
typedef volatile int64_t vint64_t;
typedef volatile uint64_t vuint64_t;


/**
 * @brief return value type
 */
typedef enum _e_return {
	RET_OK = 0x00,
	RET_ERROR = 0x80,
	RET_ERROR_PARAMETER,
	RET_ERROR_RESOURCES
} e_return;

// ================================================================================

/**
 * @brief define NULL just for convenience
 */
#ifndef NULL
#define NULL (void *)0x00
#endif


// unused attribute - to make the compiler happy when needed
#define UNUSED __attribute__((unused))


/**
 * @brief macro to determine if the returned value is an error
 */
#define IS_ERROR(__x) \
	( (__x) >= RET_ERROR)


/**
 * @brief macro to determine if the returned value is a warning
 */
#define IS_WARNING(__x) \
	( ((__x) > RET_OK) && ((__x) < RET_ERROR) )


/**
 * @brief macro to determine if the returned value is a warning or a success
 */
#define IS_WARNING_OK(__x) \
	( ((__x) >= RET_OK) && ((__x) < RET_ERROR))


// ================================================================================

/**
 * @brief return maximum value out of two given
 *
 * @param __x first value
 * @param __y second value
 *
 * @return __x or __y depending on which is bigger
 */
#define MAX(__x, __y) \
	( (__x) > (__y) ? (__x) : (__y))


/**
 * @brief return minimum value out of two given
 *
 * @param __x first value
 * @param __y second value
 *
 * @return __x or __y depending on which is smaller
 */
#define MIN(__x, __y) \
	( (__x) < (__y) ? (__x) : (__y))

// ================================================================================


/**
 * @brief provide a brief calculation of the memory usage and report how much is left
 *
 * @return number of bytes available
 */
uint16_t common_memory_left();

#endif /* __COMMON_H__ */
