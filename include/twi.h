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
 * @brief internal repeated start bit definition 
 *
 * When set it means that the system already 
 * sent a repeated start bin
 */
#define E_TWI_BIT_REPEATED_START 0x20


/**
 * @brief possible bus errors
 */
#define E_TWI_ERROR_NO_ERROR 0x00
#define E_TWI_ERROR_ARB_LOST 0x01
#define E_TWI_ERROR_MT_SLA_NACK 0x02
#define E_TWI_ERROR_MT_DATA_NACK 0x03
#define E_TWI_ERROR_MR_SLA_NACK 0x04
#define E_TWI_ERROR_MR_DATA_NACK 0x05


/**
 * @brief TWI bus context structure
 */
struct twi_ctx {
	/// slave R/W address
	volatile uint8_t slarw;

	/// status
	twi_status_t status; 

	/// pointer to the data buffer
	volatile uint8_t *xdata;

	/// data length
	volatile uint16_t len;

#ifdef TWI_DEBUG
	twi_debug_hook_t debug_hook;
#endif

};


/**
 * @brief general TWI interface initialization
 *
 * @param a_freq SCL frequency
 *
 */
volatile struct twi_ctx* twi_init(uint8_t a_freq);


#if TWI_MASTER_TRANSMITTER == 1
/**
 * @brief sends data in master transmitter mode
 *
 * @param a_address I2C address of the slave
 * @param a_data data buffer
 * @param a_len length of the data
 */
void twi_mtx(uint8_t a_address, uint8_t *a_data, uint16_t a_len, uint8_t a_flag);
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
void twi_mrx(uint8_t a_address, uint8_t *a_data, uint16_t a_len, uint8_t a_flag);
#endif


#if TWI_SUPPORT_BUS_STATUS == 1
/**
 * @brief search for all the devices connected to the bus
 *
 * @param a_dev a table to hold each 7 bit device address
 *
 * @return number of devices found
 */
uint8_t twi_search_devices(uint8_t *a_dev);
#endif


#ifdef TWI_DEBUG
/**
 * @brief install debug hook which will be called in the interrupt
 *
 * @param a_dbg debug hook to be called
 */
void twi_debug_hook_install(twi_debug_hook_t);
#endif

#endif /* end of include guard: TWI_H_V8WDZFBC */

