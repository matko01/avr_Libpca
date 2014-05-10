#ifndef RING_H_FDIQOKDH
#define RING_H_FDIQOKDH

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


#include <stdint.h>

typedef struct _ring_buffer {

	/// head index
	volatile unsigned char head;

	/// tail index
	volatile unsigned char tail;

	/// data storage space for the ring buffer
	volatile unsigned char ring[1];

} ring_buffer;

#define RING_SIZE (sizeof(ring_buffer) - 1)


#endif /* end of include guard: RING_H_FDIQOKDH */

