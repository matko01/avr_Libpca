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


/**
 * @brief standard i2c frequencies enumeration
 */
typedef enum _e_twi_scl_freq {
	E_TWI_SCL_100K = 0,
	E_TWI_SCL_250K,
	E_TWI_SCL_400K
} e_twi_scl_freq;


typedef enum _e_twi_status {
	E_TWI_STATUS_IDLE = 0x00,
	
	E_TWI_STATUS_LAST
} e_twi_status;



/**
 * @brief general TWI interface initialization
 */
void twi_init();

#if TWI_MASTER_TRANSMITTER == 1 || TWI_MASTER_RECEIVER == 1
/**
 * @brief initialize the MCU as TWI master
 *
 * @param a_freq SCL frequency
 */
void twi_setup_master(e_twi_scl_freq a_freq);
#endif

#if TWI_SLAVE_TRANSMITTER == 1 || TWI_SLAVE_RECEIVER == 1
/**
 * @brief initialize MCU as a TWI slave with given address
 *
 * @param a_address
 */
void twi_setup_slave(uint8_t a_address, uint8_t a_mask);
#endif

#if TWI_MASTER_TRANSMITTER == 1

/**
 * @brief sends data in master transmitter mode
 *
 * @param a_address I2C address of the slave
 * @param a_data data buffer
 * @param a_len length of the data
 */
void twi_mtx(uint8_t a_address, uint8_t *a_data, uint16_t a_len);
#endif

#if TWI_MASTER_RECEIVER == 1

/**
 * @brief receives data in master receiver mode
 *
 * @param a_address I2C address
 * @param a_data buffer for the data
 * @param a_maxlen maximum data length
 *
 * @return 
 */
uint8_t twi_mrx(uint8_t a_address, uint8_t *a_data, uint16_t a_len);
#endif

/**
 * @brief checks if the TWI hardware is busy
 *
 * @return 1 if busy
 */
#define twi_busy() (!(TWCR & _BV(TWINT)))

#endif /* end of include guard: TWI_H_V8WDZFBC */

