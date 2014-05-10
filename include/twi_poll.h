#ifndef __TWI_POLL_H__
#define __TWI_POLL_H__

/* Copyright (C) 
 * 2014 - Tomasz Wisniewski
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
#include "twi_common.h"

#warning NOT IMPLEMENTED YET

/**
 * @brief TWI bus context structure
 */
struct twi_poll_ctx {
	/// status
	twi_status_t status; 

#ifdef TWI_DEBUG
	twi_debug_hook_t debug_hook;
#endif
};


#ifdef TWI_DEBUG
/**
 * @brief install debug hook which will be called in the interrupt
 *
 * @param a_dbg debug hook to be called
 */
void twi_common_debug_hook_install(twi_debug_hook_t);
#endif


void twi_poll_init(uint8_t a_freq);
// twi_poll_mtx()
// twi_poll_mrx()
// twi_poll_search_devices()


#endif /* __TWI_POLL_H__ */
