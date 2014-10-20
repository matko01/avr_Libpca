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

#include "twi_common.h"
#include <util/twi.h>

void _twi_common_frequency_setup(uint8_t a_freq) {
	// setup frequency
	switch(a_freq) {
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
		case E_TWI_SCL_100K:
			// prescaler = 4
			TWSR = 0x01;
			TWBR = 0x12;
			break;
	}
}


#if TWI_SLAVE_TRANSMITTER == 1 || TWI_SLAVE_RECEIVER == 1
void twi_common_setup_slave(uint8_t a_address, uint8_t a_mask) {
	
	// respond to general query address
	TWAR = (a_address << 1) | 0x01;

	// set the slave address mask
	TWAMR = (a_mask << 1);
}
#endif


#ifdef TWI_DEBUG
/**
 * @brief install debug hook function
 *
 * @param a_dbg
 */
void twi_common_debug_hook_install(volatile twi_ctx *a_ctx, twi_debug_hook_t a_dbg) {
	a_ctx->debug_hook = a_dbg;
}
#endif


