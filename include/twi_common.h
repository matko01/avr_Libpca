#ifndef TWI_COMMON_H_2M5RXJHG
#define TWI_COMMON_H_2M5RXJHG

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


// #define TWI_DEBUG 1
#undef TWI_DEBUG


/**
 * @brief if set indicates that the TWI HW is in the middle of operation
 */
#define	E_TWI_BIT_BUSY 0x10


/** 
 * @brief internal repeated start bit definition 
 *
 * When set it means that the system already 
 * sent a repeated start bit
 */
#define E_TWI_BIT_REPEATED_START 0x20


/** 
 * @brief defines if the transaction should end with sending the STOP bit
 */
#define E_TWI_BIT_SEND_STOP 0x40


/**
 * @brief standard i2c frequencies enumeration
 */
typedef enum _e_twi_scl_freq {
	E_TWI_SCL_100K = 0,
	E_TWI_SCL_250K,
	E_TWI_SCL_400K
} e_twi_scl_freq;


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
 *  +-------------- unused
 *  |	+---------- user_send_stop
 *  |	|   +------ repeated_start status
 *  |   |   |   +-- state (IDLE/BUSY)
 *  |   |   |   |
 *  v | v | v | v | error
 * ---+---+---+---+--------
 *  7 | 6 | 5 | 4 | 3 - 0
 */
typedef volatile uint8_t twi_status_t;


// bus status manipulation
#define _twi_common_set_idle(__status) __status &= ~(E_TWI_BIT_BUSY)
#define _twi_common_set_busy(__status) __status |= E_TWI_BIT_BUSY


// set clear the repeated start bit
#define _twi_common_set_repeated_start(__status) __status |= E_TWI_BIT_REPEATED_START
#define _twi_common_clear_repeated_start(__status) __status &= ~(E_TWI_BIT_REPEATED_START)



/**
 * @brief debug function prototype
 */
typedef void (*twi_debug_hook_t)(void);


/**
 * @brief TWI bus context structure
 */
struct twi_ctx {
	/// slave R/W address - will be changed by SW during master transactions
	volatile uint8_t slarw;

	/// status
	twi_status_t status; 

#ifdef TWI_DEBUG
	/// pointer to the debugging hook
	twi_debug_hook_t debug_hook;
#endif

	/// bus transmission speed
	uint8_t freq;

	/// data length
	volatile uint16_t len;

	/// pointer to the data buffer
	volatile uint8_t *xdata;
};


/**
 * @brief setup frequency prescaler and divider
 *
 * @param a_freq requested frequency enumeration
 */
void _twi_common_frequency_setup(uint8_t a_freq);


#if TWI_SLAVE_TRANSMITTER == 1 || TWI_SLAVE_RECEIVER == 1
#error TWI SLAVE MODE NOT SUPPORTED AT THE MOMENT
/**
 * @brief initialize MCU as a TWI slave with given address
 *
 * @param a_address
 */
void twi_common_setup_slave(uint8_t a_address, uint8_t a_mask);
#endif


#ifdef TWI_DEBUG
/**
 * @brief install debug hook which will be called in the interrupt
 */
void twi_common_debug_hook_install(volatile twi_ctx *a_ctx, twi_debug_hook_t a_dbg);
#endif


#endif /* end of include guard: TWI_COMMON_H_2M5RXJHG */
