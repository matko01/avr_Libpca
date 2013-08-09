#ifndef __SERIAL_H__
#define __SERIAL_H__

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

#include "common.h"

/**
 * @brief module version definition
 */
#define SERIAL_VERSION "0.002"

/**
 * @brief ring size
 */
#define SERIAL_RING_SIZE 32

/**
 * @brief serial speeds available
 */
typedef enum _e_serial_speed {
	E_BAUD_4800 = 0,
	E_BAUD_9600
} e_serial_speed;

/**
 * @brief buffer statistics
 */
typedef struct _t_stats {
	volatile unsigned int ok;
	volatile unsigned int dropped;
	volatile unsigned int frame_error;
} t_stats;

/**
 * @brief ring buffer
 */
typedef struct _t_buffer {
	volatile unsigned char ring[SERIAL_RING_SIZE];
	volatile unsigned char head;
	volatile unsigned char tail;
	volatile t_stats stats;
} t_buffer;

/**
 * @brief initialize USART as RS232 port
 *
 * @param a_speed speed
 *
 * @return success
 */
e_return serial_init(e_serial_speed a_speed);

/**
 * @brief check how many bytes are available to read
 *
 * @return number of bytes received and waiting
 */
unsigned char serial_available();

/**
 * @brief 
 *
 * @param a_data
 * @param a_size
 *
 * @return 
 */
unsigned char serial_peek(void *a_data, unsigned char a_size);

/**
 * @brief receive data
 *
 * @param a_data
 * @param a_size
 * @param a_flag
 *
 * @return 
 */
unsigned int serial_recv(void *a_data, unsigned int a_size, unsigned char a_waitall);

/**
 * @brief read a single char from the port
 *
 * @param a_data data read from the port
 *
 * @return 1 if character available, 0 if not
 */
unsigned char serial_getc(unsigned char *a_data);

/**
 * @brief transmit a single character (this is a wrapper for serial_send)
 *
 * @param a_char character to send
 *
 * @return 1 if success, 0 if failure
 */
unsigned int serial_sendc(unsigned char a_char);

/**
 * @brief send data
 *
 * @param data
 * @param a_size
 *
 * @return 
 */
unsigned int serial_send(void *data, unsigned int a_size);

/**
 * @brief return serial buffer context
 *
 * @return 
 */
volatile t_buffer* serial_get_state();

/**
 * @brief flush data
 */
void serial_flush();


#endif /* __SERIAL_H__ */
