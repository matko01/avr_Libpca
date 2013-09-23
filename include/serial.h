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

#include "config.h"
#include "common.h"

/**
 * @brief module version definition
 */
#define SERIAL_VERSION "0.010"

/**
 * @brief interrupt flags
 */
#define SERIAL_RX_INTERRUPT 0x01
#define SERIAL_TX_INTERRUPT 0x02

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

#if SERIAL_COLLECT_STATS == 1
/**
 * @brief buffer statistics
 */
typedef struct _t_stats {
	volatile uint32_t ok;
	volatile uint32_t dropped;
	volatile uint32_t frame_error;
} t_stats;
#endif

/**
 * @brief ring buffer
 */
typedef struct _t_buffer {
	volatile unsigned char ring[SERIAL_RX_RING_SIZE];
	volatile unsigned char head;
	volatile unsigned char tail;
#if SERIAL_COLLECT_STATS == 1
	volatile t_stats stats;
#endif
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
void serial_install_interrupts(unsigned char a_flags);

/**
 * @brief install serial handlers so we can use printf-like functions with serial console
 */
void serial_install_stdio();

/**
 * @brief check how many bytes are available to read
 *
 * @return number of bytes received and waiting
 */
#if SERIAL_IMPLEMENT_RX_INT == 1
unsigned char serial_available();

/**
 * @brief check if there is any data available pending 
 * (this only works if interrupts are installed)
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
 * @param a_waitall 1 - block until a data block of requested size is assembled
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
#endif

/**
 * @brief receive data (this function is blocking)
 *
 * @param a_data buffer for the data
 * @param a_size buffer size
 *
 * @return number of bytes read
 */
unsigned char serial_poll_recv(unsigned char *a_data, unsigned int a_size);

/**
 * @brief get a character through polling. It will block until there is a byte available
 *
 * @param a_data data holder
 *
 * @return always 1
 */
unsigned char serial_poll_getc(unsigned char *a_data);

/**
 * @brief send data using interrupts
 *
 * @param a_data data to be send
 * @param a_size size of data
 * @param a_waitall block until everything is sent
 *
 * @return number of characters send
 */
#if SERIAL_IMPLEMENT_TX_INT == 1
unsigned char serial_send(void *a_data, unsigned int a_size, unsigned char a_waitall);

/**
 * @brief send a character using interrupts
 *
 * @param a_data character to be sent
 *
 * @return 1 if sent
 */
unsigned char serial_sendc(unsigned char a_data);
#endif

/**
 * @brief send data using serial port TX status polling (it will block until data is fully transmitted)
 *
 * @param a_data data to be sent
 * @param a_size size of the buffer
 *
 * @return number of bytes sent
 */
unsigned int serial_poll_send(void *a_data, unsigned int a_size);

/**
 * @brief transmit a single character (this is a wrapper for serial_poll_send)
 *
 * @param a_char character to send
 *
 * @return 1 if success, 0 if failure
 */
unsigned int serial_poll_sendc(unsigned char a_char);

/**
 * @brief return serial buffer context for RX (information and statistics about serial port)
 *
 * @return serial buffer info
 */
#if SERIAL_IMPLEMENT_RX_INT == 1
volatile t_buffer* serial_get_rx_state();
#endif

/**
 * @brief return serial buffer context for TX (information and statistics about serial port)
 *
 * @return serial buffer info
 */
#if SERIAL_IMPLEMENT_TX_INT == 1
volatile t_buffer* serial_get_tx_state();
#endif

/**
 * @brief flush data & rx fifo
 */
void serial_flush();


#endif /* __SERIAL_H__ */
