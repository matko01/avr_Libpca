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

#include "twi.h"

#include <avr/power.h>
#include <avr/io.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#include <string.h>

/* ================================================================================ */

/**
 * @brief global transmission buffer
 */
volatile static struct {
	volatile uint8_t slarw;
	volatile uint8_t g_twi_status; 
	volatile uint8_t *xdata;
	volatile uint16_t len;
	volatile uint8_t twcr;
} g_bus_ctx;


/* ================================================================================ */

/**
 * @brief TWI wire interface interrupt handler
 *
 * This vector is called every time hardware has done interaction and expects reaction
 * from the software side
 *
 * @param TWI_vect
 */
ISR(TWI_vect) {
	switch(TWSR >> 3) {
		case TW_START:
		case TW_REP_START:
			TWDR = g_bus_ctx.slarw;
			
			// clear STA, clear interrupt flag
			TWCR = g_bus_ctx.twcr | _BV(TWINT);
			break;

		case TW_MT_SLA_ACK:
		case TW_MT_DATA_ACK:
			if (!g_bus_ctx.len) {
				TWCR = g_bus_ctx.twcr | _BV(TWINT) | _BV(TWSTO);
			}
			else {
				TWDR = *g_bus_ctx.xdata;
				g_bus_ctx.xdata++;
				g_bus_ctx.len--;
				TWCR = g_bus_ctx.twcr | _BV(TWINT);
			}
			break;


		default:
			break;
	} // switch(TWSR >> 3)
}

/* ================================================================================ */

void twi_init() {
	sei();
	power_twi_enable();		
	memset((void *)&g_bus_ctx, 0x00, sizeof(g_bus_ctx));

	// enable interrupt, twi interface and acknowledge bit
	g_bus_ctx.twcr = _BV(TWEN) | _BV(TWIE) | _BV(TWEA);
	TWCR = g_bus_ctx.twcr;
}


#if TWI_MASTER_TRANSMITTER == 1 || TWI_MASTER_RECEIVER == 1
void twi_setup_master(e_twi_scl_freq a_freq) {
	switch(a_freq) {
		case E_TWI_SCL_100K:
			// prescaler = 4
			TWSR = 0x01;
			TWBR = 0x12;
			break;

		case E_TWI_SCL_250K:
			// prescaler = 4
			TWSR = 0x01;
			TWBR = 0x06;
			break;

		case E_TWI_SCL_400K:
			// prescaler = 4
			TWSR = 0x01;
			TWBR = 0x03;
			break;

		default:
			break;
	}


}
#endif


#if TWI_SLAVE_TRANSMITTER == 1 || TWI_SLAVE_RECEIVER == 1

void twi_setup_slave(uint8_t a_address, uint8_t a_mask) {
	
	// respond to general query address
	TWAR = (a_address << 1) | 0x01;

	// set the slave address mask
	TWAMR = (a_mask << 1);
}

#endif

#if TWI_MASTER_TRANSMITTER == 1
void twi_mtx(uint8_t a_address, uint8_t *a_data, uint16_t a_len) {
	g_bus_ctx.xdata = a_data;
	g_bus_ctx.len = a_len;
	g_bus_ctx.slarw = (a_address << 1) & 0xfe;
	
	// start transmission
	TWCR = g_bus_ctx.twcr | (_BV(TWINT) | _BV(TWSTA));
}
#endif


#if TWI_MASTER_RECEIVER == 1
uint8_t twi_mrx(uint8_t a_address, uint8_t *a_data, uint16_t a_maxlen) {
	// TODO implement me
	return 0;
}
#endif


__inline__
uint8_t twi_busy() {
	return (!(TWCR & _BV(TWINT)));
}

/* ================================================================================ */
