#ifndef CONFIG_H_VPGOK90D
#define CONFIG_H_VPGOK90D

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
 * @file config.h 
 *
 * @brief This file stores all significant compilation configuration flags which decide upon the implementation
 *  inside of the library.
 *
 * Using macro definitions here you can enable/disable some of the library implementation features, aw well
 * as change the default values for some of the important configuration constants
 *
 */

// ============================ SERIAL PORT =======================================

/**
 * @brief change to 0 and rebuild if you do not need stats information
 */
#define SERIAL_COLLECT_STATS 0

/**
 * @brief change to 0 and rebuild if you do not wish to include serial RX interrupt
 * some functionality will be missing.
 *
 * Serial reception will be only possible using polling, blocking mechanisms
 */
#define SERIAL_IMPLEMENT_RX_INT 1

/**
 * @brief change to 0 and rebuild if you do not with to include serial tx interrupt
 * some functionality will be missing
 *
 * Serial transmission will be only possible character by character without any buffering,
 * using only polling mechanisms
 */
#define SERIAL_IMPLEMENT_TX_INT 0

/**
 * @brief if set to "1" the high speed pre-calculated baud rate settings will be included
 */
#define SERIAL_SUPPORT_HIGH_SPEEDS 1

/**
 * @brief 1 - STDIN data will be received by polling, 0 - interrupts
 */
#define SERIAL_STDIN_POLL 0

/**
 * @brief 1 - STDOUT data will be sent by polling, 0 - interrupts
 */
#define SERIAL_STDOUT_POLL 1

/**
 * @brief RX ring buffer size if SERIAL_IMPLEMENT_RX_INT == 1
 *
 * For high speeds and very busy applications one may consider increasing the RX ring buffer size
 * This variable has any useful meaning only if SERIAL_IMPLEMENT_RX_INT == 1
 */
#define SERIAL_RX_RING_SIZE 64

/**
 * @brief TX ring buffer size if SERIAL_IMPLEMENT_TX_INT == 1
 *
 * For high speeds and very busy applications one may consider increasing the TX ring buffer size
 * This variable has any useful meaning only if SERIAL_IMPLEMENT_TX_INT == 1
 */
#define SERIAL_TX_RING_SIZE 64

// ============================ TIMERS/BEEPER =====================================

/**
 * @brief change to 0 if to disable timer 0 delay and beeper interrupts and routines
 *
 * In case user wants to define his own ISR for TIMER0_COMPA_vect, he should disable the library implementation
 */
#define TDELAY_IMPLEMENT_T0_INT 1

/**
 * @brief change to 0 if to disable timer 1 delay and beeper interrupts and routines
 *
 * In case user wants to define his own ISR for TIMER1_COMPA_vect, he should disable the library implementation
 */
#define TDELAY_IMPLEMENT_T1_INT 0

/**
 * @brief change to 0 if to disable timer 2 delay and beeper interrupts and routines
 *
 * In case user wants to define his own ISR for TIMER2_COMPA_vect, he should disable the library implementation
 */
#define TDELAY_IMPLEMENT_T2_INT 0

// =========================== SLIP IMPLEMENTATION ================================

/**
 * @brief change to 0 if to disable the crc16 SLIP implementation
 */
#define SLIP_IMPLEMENT_CRC16 1

// =================================== TWI ========================================

/**
 * @brief change to 0 to disable master transmitter support
 */
#define TWI_MASTER_TRANSMITTER 1

/**
 * @brief change to 0 to disable master receiver support
 */
#define TWI_MASTER_RECEIVER 1

/**
 * @brief change to 0 to disable slave transmitter support
 */
#define TWI_SLAVE_TRANSMITTER 0

/**
 * @brief change to 0 to disable slave receiver support
 */
#define TWI_SLAVE_RECEIVER 0

/**
 * @brief change to 0 to disable bus status collection after each transaction
 */
#define TWI_SUPPORT_BUS_STATUS 1

/**
 * @brief change to 0 to disable repeated start support (good for single master buses)
 */
#define TWI_SUPPORT_REPEATED_START 1

// ================================================================================

/**
 * @brief change to 1 to use the lcd in 8 bit data bus mode
 */
#define HD44780_8BIT_MODE 0

/**
 * @brief change to 1 to use the lcd rw line and enable support for read operation
 */
#define HD44780_USE_RW_LINE 0 

// ================================================================================


#endif /* end of include guard: CONFIG_H_VPGOK90D */

