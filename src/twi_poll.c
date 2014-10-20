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


#include "twi_poll.h"
#include <avr/power.h>
#include <util/twi.h>


#define _twi_poll_twint_wait() while (!(TWCR & _BV(TWINT)))


void twi_minit(volatile struct twi_ctx *a_ctx, uint8_t a_freq) {
	uint8_t s = sizeof(struct twi_ctx);
	common_zero_mem(a_ctx, s);

	power_twi_enable();
	_twi_common_frequency_setup(a_freq);

	// enable interrupt, twi interface and acknowledge bit
	TWCR = (_BV(TWEN) | _BV(TWEA));
}


void twi_poll_sync() {
	_twi_poll_twint_wait();
}


#if TWI_MASTER_TRANSMITTER == 1
void twi_poll_mtx(volatile struct twi_ctx *a_ctx, 
		uint8_t a_address, 
		uint8_t *a_data, 
		uint16_t a_len, 
		uint8_t a_flag) {
	a_ctx->xdata = a_data;
	a_ctx->len = a_len;
	a_ctx->slarw = a_address;
	a_ctx->status |= a_flag;

	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
	_twi_poll_twint_wait();

}
#endif


#if TWI_MASTER_RECEIVER == 1
void twi_poll_mrx(volatile struct twi_ctx *a_ctx, 
		uint8_t a_address, 
		uint8_t *a_data, 
		uint16_t a_len, 
		uint8_t a_flag) {
}
#endif


uint8_t twi_poll_status(volatile struct twi_ctx *a_ctx) {
}


#if TWI_SUPPORT_BUS_STATUS == 1
uint8_t twi_poll_search_devices(volatile struct twi_ctx *a_ctx, uint8_t *a_dev) {
}
#endif


