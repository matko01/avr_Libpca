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

// ================================================================================

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

#define IS_ERROR(__x) \
	( (__x) >= RET_ERROR)

#define IS_WARNING(__x) \
	( ((__x) > RET_OK) && ((__x) < RET_ERROR) )

#define IS_WARNING_OK(__x) \
	( ((__x) >= RET_OK) && ((__x) < RET_ERROR))

// ================================================================================

#define MAX(__x, __y) \
	( (__x) > (__y) ? (__x) : (__y))

#define MIN(__x, __y) \
	( (__x) < (__y) ? (__x) : (__y))

// ================================================================================


#endif /* __COMMON_H__ */
