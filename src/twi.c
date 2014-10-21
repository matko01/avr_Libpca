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
#include <util/twi.h>
#include <avr/interrupt.h>

/* ================================================================================ */

static void _twi_init(volatile struct twi_ctx *a_ctx, uint8_t a_freq);
static void _twi_mx(volatile struct twi_ctx *a_ctx, uint8_t a_address, uint8_t *a_data, uint16_t a_len, uint8_t a_flag);

/* ================================================================================ */

/**
 * @brief global bus context
 */
static volatile struct twi_ctx *g_bus_ctx = NULL;

/* ================================================================================ */

// send stop bit
#define _twi_stop() \
	TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTO); \
	_twi_common_set_idle(g_bus_ctx->status)

// disables the interrupt since it's asynchronous and we would land up in a loop
#define _twi_repeated_start() \
	TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWSTA)

// simply clear the interrupt flag and enable ACK
#define _twi_ack() \
	TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT)

// clear the interrupt flag and disable ACK
#define _twi_nack() \
	TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT)


/**
 * @brief send stop or repeated start
 */
#define _twi_stop_rs() \
	if (g_bus_ctx && g_bus_ctx->status & E_TWI_BIT_SEND_STOP) { \
		_twi_stop(); \
	} \
	else { \
		_twi_common_set_repeated_start(g_bus_ctx->status); \
		_twi_repeated_start(); \
	} \
	_twi_common_set_idle(g_bus_ctx->status)


/**
 * @brief TWI wire interface interrupt handler
 *
 * This vector is called every time hardware has done interaction and expects reaction
 * from the software side
 *
 * @param TWI_vect
 */
ISR(TWI_vect) {

	if (!g_bus_ctx) {
		// if context is NULL - then just acknowledge and do nothing
		_twi_ack();
	}
	else {

#ifdef TWI_DEBUG
		// debug hook
		if (g_bus_ctx->debug_hook) g_bus_ctx->debug_hook();
#endif

		// mask out the prescaler bits
		switch(TWSR & 0xf8) {

			case TW_START:
			case TW_REP_START:			
				TWDR = g_bus_ctx->slarw;
				// clear STA, clear interrupt flag and continue
				_twi_ack();
				break;

#if TWI_MASTER_TRANSMITTER == 1			
			case TW_MT_SLA_ACK: // 0x18
			case TW_MT_DATA_ACK: // 0x28
				if (g_bus_ctx->len--) {
					TWDR = *g_bus_ctx->xdata++;
					_twi_ack();
				}
				else {

#if TWI_SUPPORT_REPEATED_START == 1
					_twi_stop_rs();
#else
					_twi_stop();
#endif
				}
				break;

			case TW_MT_SLA_NACK: // 0x20			
			case TW_MT_DATA_NACK:
				// send stop or exit interrupt, next the repeated start will be sent
#if TWI_SUPPORT_BUS_STATUS == 1
				g_bus_ctx->status |= (TWSR >> 4);
#endif

#if TWI_SUPPORT_REPEATED_START == 1
				_twi_stop_rs();
#else
				_twi_stop();
#endif
				break;

			case TW_MT_ARB_LOST:
				// release the bus			
#if TWI_SUPPORT_BUS_STATUS == 1
				g_bus_ctx->status |= E_TWI_ERROR_ARB_LOST;
#endif
				_twi_stop();
				break;
#endif

#if TWI_MASTER_RECEIVER == 1			
			case TW_MR_DATA_ACK:
				(*g_bus_ctx->xdata++) = TWDR;
				// fall through deliberately

			case TW_MR_SLA_ACK:
				_twi_nack() | ((--g_bus_ctx->len) ? _BV(TWEA) : 0x00);
				break;

			case TW_MR_DATA_NACK:
				// fetch the final byte of data
				(*g_bus_ctx->xdata) = TWDR;
				// fall through deliberately

			case TW_MR_SLA_NACK:

#if TWI_SUPPORT_BUS_STATUS == 1
				g_bus_ctx->status |= (TWSR >> 4);
#endif

#if TWI_SUPPORT_REPEATED_START == 1
				// send stop or exit interrupt next the repeated start will be sent
				_twi_stop_rs();
#else
				_twi_stop();
#endif
				break;
#endif

#if TWI_SLAVE_TRANSMITTER == 1
			case TW_ST_SLA_ACK:
			case TW_ST_ARB_LOST_SLA_ACK:
				break;

			case TW_ST_DATA_ACK:
				break;

			case TW_ST_DATA_NACK:
			case TW_ST_LAST_DATA:
				break;
#endif

#if TWI_SLAVE_RECEIVER == 1
			case TW_SR_SLA_ACK:
			case TW_SR_GCALL_ACK:
			case TW_SR_ARB_LOST_SLA_ACK:
			case TW_SR_ARB_LOST_GCALL_ACK:
				break;

			case TW_SR_DATA_ACK:
			case TW_SR_GCALL_DATA_ACK:
				break;

			case TW_SR_DATA_NACK:
			case TW_SR_GCALL_DATA_NACK:
				break;

			case TW_SR_STOP:
				break;
#endif

			default:
				// clear the interrupt flag to avoid endless loop
				_twi_ack();
				break;
		} // switch(TWSR)
	} // if
} // ISR(TWI_vect)


/* ================================================================================ */

void twi_minit(volatile struct twi_ctx *a_ctx, uint8_t a_freq) {
	_twi_init(a_ctx, a_freq);
}


#if TWI_SLAVE_TRANSMITTER == 1 || TWI_SLAVE_RECEIVER == 1
void twi_sinit(volatile struct twi_ctx *a_ctx, uint8_t a_addr) {
	_twi_init(a_ctx, 0);
	a_ctx->slarw = a_addr;
}
#endif


#if TWI_MASTER_TRANSMITTER == 1
void twi_mtx(volatile struct twi_ctx *a_ctx, uint8_t a_address, uint8_t *a_data, uint16_t a_len, uint8_t a_flag) {
	_twi_mx(a_ctx, (a_address << 1), a_data, a_len, a_flag);
}
#endif


#if TWI_MASTER_RECEIVER == 1
void twi_mrx(volatile struct twi_ctx *a_ctx, uint8_t a_address, uint8_t *a_data, uint16_t a_len, uint8_t a_flag) {
	_twi_mx(a_ctx, ((a_address << 1) | 0x01), a_data, a_len, a_flag);
}
#endif


uint8_t twi_status(volatile struct twi_ctx *a_ctx) {
	return (a_ctx->status & 0x0f);
}


void twi_sync() {
	while (g_bus_ctx && (g_bus_ctx->status & E_TWI_BIT_BUSY));
}


#if TWI_SUPPORT_BUS_STATUS == 1
uint8_t twi_search_devices(volatile struct twi_ctx *a_ctx, uint8_t *a_dev) {
	uint8_t i = 0x60;
	uint8_t cnt = 0x00;
	if (NULL == a_dev) return 0;

	for (; i < 0x70; i++) {
		twi_mtx(a_ctx, i, 0x00, 0x00, E_TWI_BIT_SEND_STOP);
		twi_sync();
		if (E_TWI_ERROR_NO_ERROR == twi_status(a_ctx)) {
			*a_dev++ = i;
			cnt++;
		}
	} // for

	return cnt;
}
#endif

/* ================================================================================ */

static void _twi_init(volatile struct twi_ctx *a_ctx, uint8_t a_freq) {
	uint8_t size = sizeof(struct twi_ctx);
	common_zero_mem(a_ctx, size);
	a_ctx->freq = a_freq;

	power_twi_enable();

	// enable interrupt, twi interface and acknowledge bit
	TWCR = (_BV(TWEN) | _BV(TWIE) | _BV(TWEA));

#if TWI_MASTER_TRANSMITTER == 1 || TWI_MASTER_RECEIVER == 1
	_twi_common_frequency_setup(a_freq);
#endif

	sei();
}


static void _twi_mx(volatile struct twi_ctx *a_ctx, 
		uint8_t a_address, 
		uint8_t *a_data, 
		uint16_t a_len, 
		uint8_t a_flag) {

	a_ctx->xdata = a_data;
	a_ctx->len = a_len;
	a_ctx->slarw = a_address;

	_twi_common_set_busy(a_ctx->status);
	a_ctx->status &= (E_TWI_BIT_REPEATED_START | E_TWI_BIT_BUSY);
	a_ctx->status |= a_flag;

	// set the global context to the current one
	if (g_bus_ctx != a_ctx) {
		if (g_bus_ctx && g_bus_ctx->freq != a_ctx->freq) {
			/// reconfigure the frequency
			_twi_common_frequency_setup(a_ctx->freq);
		}

		g_bus_ctx = a_ctx;
	} 
	
	// generate start or repeated start
#if TWI_SUPPORT_REPEATED_START == 1
	if (!(a_ctx->status & E_TWI_BIT_REPEATED_START)) 
	{
#endif
		TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWSTA);
#if TWI_SUPPORT_REPEATED_START == 1
	}
	else
   	{
		// repeated start has been send -> reenable int only
		_twi_common_clear_repeated_start(a_ctx->status);
		TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA);
	}
#endif
}

/* ================================================================================ */
