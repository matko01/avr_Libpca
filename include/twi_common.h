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
 * sent a repeated start bin
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



#endif /* end of include guard: TWI_COMMON_H_2M5RXJHG */
