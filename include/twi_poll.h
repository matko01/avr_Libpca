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


void twi_minit(volatile struct twi_ctx *a_ctx, uint8_t a_freq);
void twi_poll_sync();


#if TWI_MASTER_TRANSMITTER == 1
void twi_poll_mtx(volatile struct twi_ctx *a_ctx, uint8_t a_address, uint8_t *a_data, uint16_t a_len, uint8_t a_flag);
#endif


#if TWI_MASTER_RECEIVER == 1
void twi_poll_mrx(volatile struct twi_ctx *a_ctx, uint8_t a_address, uint8_t *a_data, uint16_t a_len, uint8_t a_flag);
#endif


uint8_t twi_poll_status(volatile struct twi_ctx *a_ctx);


#if TWI_SUPPORT_BUS_STATUS == 1
uint8_t twi_poll_search_devices(volatile struct twi_ctx *a_ctx, uint8_t *a_dev);
#endif






#endif /* __TWI_POLL_H__ */
