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
#include "string.h"

#include "serial.h"

/// serial rx ring buffer
static volatile t_buffer g_rx_buff;


/**
 * @brief receive USART interrupt
 *
 * @param USART_RX_vect
 */
ISR(USART_RX_vect) {
	
	// must read the data in order to clear the interrupt flag
	unsigned char data = UDR0;

	// no frame error
	if (bit_is_clear(UCSR0A, FE0)) {
		volatile unsigned char next =
		   	((g_rx_buff.head + 1) % SERIAL_RING_SIZE);

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
		/// increase the frame error counter
		g_rx_buff.stats.frame_error++;
	}

}

e_return serial_init(e_serial_speed a_speed) {
	
	power_usart0_enable();	

	switch (a_speed) { 
		case E_BAUD_4800:
			UBRR0H = 0;
			UBRR0L = 207;
			break;

		case E_BAUD_9600:
		default:
			UBRR0H = 0;
			UBRR0L = 103;
			break;
	} // switch

	// clear the ring
	memset((unsigned char *)&g_rx_buff, 0x00, sizeof(g_rx_buff));

	// asynchronous, 8N1 mode
	UCSR0C |= 0x06;
	
	// rx/tx enable
	UCSR0B |= _BV(RXEN0);
	UCSR0B |= _BV(TXEN0);

	serial_flush();

	// enable receive interrupt
	UCSR0B |= _BV(RXCIE0);

	return RET_OK;
}

inline unsigned char serial_available() {
	return (SERIAL_RING_SIZE + g_rx_buff.head - g_rx_buff.tail) % SERIAL_RING_SIZE;
}

unsigned char serial_peek(void *a_data, unsigned char a_size) {
	unsigned char read = serial_available();
	if (read > a_size)
		read = a_size;

	for (unsigned char i = 0; i<read; i++) {
		((unsigned char *)a_data)[i] =
			g_rx_buff.ring[ (g_rx_buff.tail + i) % SERIAL_RING_SIZE ];
	}

	return read;
}

unsigned char serial_getc(unsigned char *a_data) {
	if (g_rx_buff.head == g_rx_buff.tail)
		return 0;

	*a_data = g_rx_buff.ring[g_rx_buff.tail];
	g_rx_buff.tail = (g_rx_buff.tail + 1) % SERIAL_RING_SIZE;
	return 1;
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
			g_rx_buff.tail = (g_rx_buff.tail + 1) % SERIAL_RING_SIZE;
			read++;
		}

		if (!a_waitall)
			break;
	}
	
	return read;
}

void serial_flush() {
	unsigned char dummy __attribute__((unused)) = UDR0;
	g_rx_buff.tail = g_rx_buff.head = 0x00;
}

unsigned int serial_send(void *data, unsigned int a_size) {
	unsigned int i = 0x00;
	while (i < a_size) {
		// wait until usart buffer is empty
		while ( bit_is_clear(UCSR0A, UDRE0) );

		// send the data
		UDR0 = ((unsigned char *)data)[i++];
	}

	return i;
}

unsigned int serial_sendc(unsigned char a_char) {
	return serial_send((void *)&a_char, 1);
}

volatile t_buffer* serial_get_state() {
	return &g_rx_buff;
}
