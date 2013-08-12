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

#include <avr/io.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>

#include "serial.h"


/* ================================================================================ */

/// serial rx ring buffer
static volatile t_buffer g_rx_buff;

/// serial tx ring buffer
static volatile t_buffer g_tx_buff;

/* ================================================================================ */


/**
 * @brief receive USART interrupt
 *
 * @param USART_RX_vect
 */
ISR(USART_RX_vect, ISR_BLOCK) {
	// no frame error
	// UCSR0A must be read before UDR0 !!!
	if (bit_is_clear(UCSR0A, FE0)) {
		// must read the data in order to clear the interrupt flag
		volatile unsigned char data = UDR0;
		volatile unsigned char next =
		   	((g_rx_buff.head + 1) % SERIAL_RX_RING_SIZE);

		// do not overflow the buffer
		if (next != g_rx_buff.tail) {
			g_rx_buff.ring[g_rx_buff.head] = data;
			g_rx_buff.head = next;
			g_rx_buff.stats.ok++;
		}
		else {
			/// increase the dropped counter
			g_rx_buff.stats.dropped++;
		}
	}
	else {
		// must read the data in order to clear the interrupt flag
		volatile unsigned char data __attribute__((unused)) = UDR0;

		/// increase the frame error counter
		g_rx_buff.stats.frame_error++;
	}
}

/* ================================================================================ */

static void _serial_putc(char c, FILE *stream) {
	if ('\n' == c) {
		_serial_putc('\r', stream);
	}

	serial_poll_sendc(c);
}

static char _serial_getc(FILE *stream) {
	unsigned char c = 0x00;
	while (!serial_getc(&c));
	return (char)c;
}

/* ================================================================================ */


e_return serial_init(uint32_t a_speed) {
	
	// baud value
	uint16_t baud_value = 0x00;

	// enable power
	power_usart0_enable();	

	// double mode disabled
	UCSR0A = 0x00;

	// choose predefined value or calculate
	switch (a_speed) { 
		case E_BAUD_2400:
			UCSR0A |= _BV(U2X0);
			baud_value = 832;
			break;

		case E_BAUD_4800:
			baud_value = 207;
			break;

		case E_BAUD_9600:
			baud_value = 103;
			break;

		case E_BAUD_14400:
			baud_value = 68;
			break;

		case E_BAUD_19200:
			baud_value = 51;
			break;

		case E_BAUD_28800:
			baud_value = 34;
			break;

		case E_BAUD_38400:
			baud_value = 25;
			break;

		case E_BAUD_57600:
			UCSR0A |= _BV(U2X0);
			baud_value = 34;
			break;

		case E_BAUD_76800:
			baud_value = 12;
			break;

		case E_BAUD_115200:
			baud_value = 8;
			break;

		default:
			// manual calculation
			{
				int8_t use_double = 1;

				do {
					if (use_double) {
						UCSR0A |= _BV(U2X0);
						baud_value = (F_CPU / 4 / a_speed - 1) / 2;
					} else {
						UCSR0A = 0x00;
						baud_value = (F_CPU / 8 / a_speed - 1) / 2;
					}

				} while (baud_value > 4095 && use_double--);
			}
			break;
	} // switch

	UBRR0H = (baud_value >> 8) & 0xff;
	UBRR0L = baud_value & 0xff;

	// clear the ring
	memset((unsigned char *)&g_rx_buff, 0x00, sizeof(g_rx_buff));

	// asynchronous, 8N1 mode
	UCSR0C |= 0x06;
	
	// rx/tx enable
	UCSR0B |= _BV(RXEN0);
	UCSR0B |= _BV(TXEN0);

	serial_flush();
	return RET_OK;
}


void serial_install_interrupts(unsigned char a_flags) {

	if (a_flags & SERIAL_RX_INTERRUPT) {
		// enable receive interrupt
		UCSR0B |= _BV(RXCIE0);
		UCSR0B &= ~_BV(UDRIE0);
	}

	if (a_flags & SERIAL_TX_INTERRUPT) {
		// TODO implement me
	}

	// enable global interrupts
	sei();
}


void serial_install_stdio() {
	static FILE uart_stdout = FDEV_SETUP_STREAM(_serial_putc, NULL, _FDEV_SETUP_WRITE);
	static FILE uart_stdin = FDEV_SETUP_STREAM(NULL, _serial_getc, _FDEV_SETUP_READ);

	stdout = &uart_stdout;
	stdin = &uart_stdin;
}


inline unsigned char serial_available() {
	return (SERIAL_RX_RING_SIZE + g_rx_buff.head - g_rx_buff.tail) % SERIAL_RX_RING_SIZE;
}


unsigned char serial_peek(void *a_data, unsigned char a_size) {
	unsigned char read = serial_available();
	if (read > a_size)
		read = a_size;

	for (unsigned char i = 0; i<read; i++) {
		((unsigned char *)a_data)[i] =
			g_rx_buff.ring[ (g_rx_buff.tail + i) % SERIAL_RX_RING_SIZE ];
	}

	return read;
}


unsigned int serial_recv(void *a_data, unsigned int a_size, unsigned char a_waitall) {
	unsigned int read = 0x00;

	if (!a_waitall && g_rx_buff.head == g_rx_buff.tail) {
		return 0;
	}

	while ( read < a_size ) {

		while (serial_available() && (read < a_size)) 
		{
			((unsigned char *)a_data)[read] = g_rx_buff.ring[ g_rx_buff.tail ];
			g_rx_buff.tail = (g_rx_buff.tail + 1) % SERIAL_RX_RING_SIZE;
			read++;
		}

		if (!a_waitall)
			break;
	}
	
	return read;
}


unsigned char serial_getc(unsigned char *a_data) {
	
	if (g_rx_buff.head == g_rx_buff.tail)
		return 0;

	*a_data = g_rx_buff.ring[g_rx_buff.tail];
	g_rx_buff.tail = (g_rx_buff.tail + 1) % SERIAL_RX_RING_SIZE;
	return 1;
}


unsigned char serial_poll_recv(unsigned char *a_data, unsigned int a_size) {
	unsigned int cnt = 0x00;

	while (cnt < a_size) {
		/* Wait for data to be received */
		while ( !(UCSR0A & (1<<RXC0)) );

		/* Get and return received data from buffer */
		a_data[cnt++] = UDR0;
	}

	return cnt;
}


unsigned char serial_poll_getc(unsigned char *a_data) {
	return serial_poll_recv(a_data, 1);
}


unsigned char serial_send(void *a_data, unsigned int a_size, unsigned char a_waitall) {
// TODO implement me
	return 0;
}


unsigned char serial_sendc(unsigned char a_data) {
// TODO implement me
	return 0;
}


unsigned int serial_poll_send(void *data, unsigned int a_size) {
	unsigned int i = 0x00;
	while (i < a_size) {
		// wait until usart buffer is empty
		while ( bit_is_clear(UCSR0A, UDRE0) );

		// send the data
		UDR0 = ((unsigned char *)data)[i++];
	}

	return i;
}


unsigned int serial_poll_sendc(unsigned char a_char) {
	return serial_poll_send((void *)&a_char, 1);
}


void serial_flush() {
	unsigned char dummy __attribute__((unused)) = 0x00;
	g_rx_buff.tail = g_rx_buff.head = 0x00;

	// flush the input fifo
	while ( UCSR0A & (1<<RXC0) ) dummy = UDR0;
}


volatile t_buffer* serial_get_rx_state() {
	return &g_rx_buff;
}


volatile t_buffer* serial_get_tx_state() {
	return &g_tx_buff;
}
