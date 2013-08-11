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
#define SERIAL_VERSION "0.004"

/**
 * @brief ring size
 */
#define SERIAL_RING_SIZE 32

/**
 * @brief serial speeds available
 */
typedef enum _e_serial_speed {
	E_BAUD_2400 = 2400,
	E_BAUD_4800 = 4800,
	E_BAUD_9600 = 9600,
	E_BAUD_14400 = 14400,
	E_BAUD_19200 = 19200,
	E_BAUD_28800 = 28800,
	E_BAUD_38400 = 38400,
	E_BAUD_57600 = 57600,
	E_BAUD_76800 = 76800,
	E_BAUD_115200 = 115200
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
e_return serial_init(uint32_t a_speed);

/**
 * @brief install USART interrupts
 */
void serial_install_interrupts();

/**
 * @brief install serial handlers so we can use printf-like functions with serial console
 */
void serial_install_stdio();

/**
 * @brief check how many bytes are available to read
 *
 * @return number of bytes received and waiting
 */
unsigned char serial_available();

/**
 * @brief check if there is any data available pending
 *
 * @param a_data buffer to place any data in
 * @param a_size size of the buffer
 *
 * @return number of bytes available
 */
unsigned char serial_peek(void *a_data, unsigned char a_size);

/**
 * @brief receive data
 *
 * @param a_data buffer for the data
 * @param a_size buffer size
 * @param a_flag flags
 *
 * @return number of bytes read
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
 * @brief get a character through polling. It will block until there is a byte available
 *
 * @param a_data data holder
 *
 * @return always 1
 */
unsigned char serial_poll_getc(unsigned char *a_data);

/**
 * @brief transmit a single character (this is a wrapper for serial_poll_send)
 *
 * @param a_char character to send
 *
 * @return 1 if success, 0 if failure
 */
unsigned int serial_poll_sendc(unsigned char a_char);

/**
 * @brief send data using serial port TX status polling (it will block until data is fully transmitted)
 *
 * @param data data to send
 * @param a_size size of the buffer
 *
 * @return number of bytes sent
 */
unsigned int serial_poll_send(void *data, unsigned int a_size);

/**
 * @brief return serial buffer context (information and statistics about serial port)
 *
 * @return serial buffer info
 */
volatile t_buffer* serial_get_state();

/**
 * @brief flush data & rx fifo
 */
void serial_flush();


#endif /* __SERIAL_H__ */
