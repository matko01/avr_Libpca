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


/**
 * @file serial.h 
 *
 * @brief API over UART configured as RS232. Contains all the necessary routines in order to provide basic means
 *  of communication over serial port
 *
 * Using macro definitions here you can enable/disable some of the library implementation features, aw well
 *  as change the default values for some of the important configuration constants.
 *
 * @example serial_01.c
 *
 * Basic Serial port usage example
 *
 * @example serial_02.c
 *
 * Using serial port used in polled fashion
 *
 * @example serial_03.c
 *
 * Using serial port as STDOUT
 *
 * @example serial_04.c
 *
 * Using serial port along with SLIP protocol for binary data IO synchronization
 */

#include <config.h>
#include <common.h>
#include <ring.h>


/**
 * @brief module version definition (just for internal use, to determine API incompatibilities)
 */
#define SERIAL_VERSION "0.010"


/**
 * @brief available flags used by the serial API functions
 */
typedef enum _e_serial_flags {
	E_FLAGS_SERIAL_RX_INTERRUPT = 0x01,
	E_FLAGS_SERIAL_TX_INTERRUPT,
} e_serial_flags;


/**
 * @brief serial pre calculated supported USART baud rates
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
	E_BAUD_115200 = 115200,

#if SERIAL_SUPPORT_HIGH_SPEEDS == 1
	// bellow speed definitions may be not supported by all hardware
	E_BAUD_230400 = 230400,
	E_BAUD_250000 = 250000,
	E_BAUD_500000 = 500000,
	E_BAUD_1000000 = 1000000,
#endif

	E_BAUD_LAST
} e_serial_speed;


#if SERIAL_COLLECT_STATS == 1
/**
 * @brief IO buffer statistics type  declaration
 */
typedef struct _t_stats {

	/// number of a valid rx/tx bytes
	volatile uint32_t ok;

	/// number of dropped data bytes due to buffer being full
	volatile uint32_t dropped;

	/// number of bytes received with frame error indicator
	volatile uint32_t frame_error;
} t_stats;
#endif


/**
 * @brief TX/RX ring buffer declaration
 */
typedef struct _t_buffer {

	/// data storage space for the ring buffer
	union {
		volatile uint8_t raw[SERIAL_RX_RING_SIZE + RING_SIZE];
		volatile ring_buffer r;
	} u;

#if SERIAL_COLLECT_STATS == 1

	/// statistics for the buffer
	volatile t_stats stats;
#endif

} t_buffer;


/**
 * @brief initialize USART as RS232 port
 *
 * @param a_speed speed (e_serial_speed enumeration can be used for standard BAUD rates)
 *
 * @return always success
 */
e_return serial_init(uint32_t a_speed);


/**
 * @brief install USART interrupts
 *
 * @param a_flags bitfield defining interrupt which user want to install (accepted values: E_FLAGS_SERIAL_RX_INTERRUPT, E_FLAGS_SERIAL_TX_INTERRUPT)
 */
void serial_install_interrupts(e_serial_flags a_flags);


/**
 * @brief install serial handlers so we can use printf-like functions with serial console
 */
void serial_install_stdio();


#if SERIAL_IMPLEMENT_RX_INT == 1
/**
 * @brief check how many bytes are available, pending to be read 
 * (this only works if RX interrupts are installed)
 *
 * @return number of bytes received and waiting
 */
unsigned char serial_available();


/**
 * @brief check if there is any data available pending 
 * (this only works if RX interrupts are installed)
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


#if SERIAL_IMPLEMENT_TX_INT == 1
/**
 * @brief send data using interrupts
 *
 * @param a_data data to be send
 * @param a_size size of data
 * @param a_waitall block until everything is sent
 *
 * @return number of characters send
 */
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


#if SERIAL_IMPLEMENT_RX_INT == 1
/**
 * @brief return serial buffer context for RX (information and statistics about serial port)
 *
 * @return serial buffer info
 */
volatile t_buffer* serial_get_rx_state();
#endif


#if SERIAL_IMPLEMENT_TX_INT == 1
/**
 * @brief return serial buffer context for TX (information and statistics about serial port)
 *
 * @return serial buffer info
 */
volatile t_buffer* serial_get_tx_state();
#endif


/**
 * @brief flush data & rx fifo
 */
void serial_flush();


#endif /* __SERIAL_H__ */
