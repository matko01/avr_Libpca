#ifndef TWI_H_V8WDZFBC
#define TWI_H_V8WDZFBC

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
 * @file twi.h 
 *
 * @brief API for configure Atmega's TWI peripheral
 *
 * @example twi_01.c
 *
 * Using the twi interface to communicate with i2c device
 */

#include "config.h"
#include "common.h"
#include "twi_common.h"


/**
 * @brief general TWI interface initialization
 *
 * @param a_freq SCL frequency
 *
 */
void twi_minit(volatile struct twi_ctx *a_ctx, uint8_t a_freq);

/**
 * @brief initialize the bus as a slave device
 *
 * @param a_ctx bus context
 * @param a_addr slave device address
 */
void twi_sinit(volatile struct twi_ctx *a_ctx, uint8_t a_addr);

/**
 * @brief wait until a data transfer has been completed
 *
 * @param twi_m_ctx TWI master context 
 */
void twi_sync();


#if TWI_MASTER_TRANSMITTER == 1
/**
 * @brief sends data in master transmitter mode
 *
 * @param a_address I2C address of the slave
 * @param a_data data buffer
 * @param a_len length of the data
 */
void twi_mtx(volatile struct twi_ctx *a_ctx, uint8_t a_address, uint8_t *a_data, uint16_t a_len, uint8_t a_flag);
#endif


#if TWI_MASTER_RECEIVER == 1
/**
 * @brief receives data in master receiver mode
 *
 * @param a_address I2C address
 * @param a_data buffer for the data
 * @param a_maxlen maximum data length
 *
 */
void twi_mrx(volatile struct twi_ctx *a_ctx, uint8_t a_address, uint8_t *a_data, uint16_t a_len, uint8_t a_flag);
#endif


/**
 * @brief get the bus status
 *
 * @param a_ctx bus context
 *
 * @return bus status
 */
uint8_t twi_status(volatile struct twi_ctx *a_ctx);


#if TWI_SUPPORT_BUS_STATUS == 1
/**
 * @brief search for all the devices connected to the bus
 *
 * @param a_dev a table to hold each 7 bit device address
 *
 * @return number of devices found
 */
uint8_t twi_search_devices(volatile struct twi_ctx *a_ctx, uint8_t *a_dev);
#endif

#endif /* end of include guard: TWI_H_V8WDZFBC */

