#ifndef RING_H_FDIQOKDH
#define RING_H_FDIQOKDH

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

