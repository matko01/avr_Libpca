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


// general
#define TWI_STATUS_START 				0x08
#define TWI_STATUS_REPEATED_START 		0x10
#define TWI_STATUS_ARBITRATION_LOST		0x38

// master transmitter mode
#define TWI_STATUS_MT_SLA_ACK 			0x18
#define TWI_STATUS_MT_SLA_NACK 			0x20
#define TWI_STATUS_MT_DATA_ACK 			0x28
#define TWI_STATUS_MT_DATA_NACK 		0x30

// master receiver mode
#define TWI_STATUS_MR_SLA_ACK 			0x40
#define TWI_STATUS_MR_SLA_NACK 			0x48
#define TWI_STATUS_MR_DATA_ACK 			0x50
#define TWI_STATUS_MR_DATA_NACK 		0x58

// slave receiver mode
#define TWI_STATUS_SR_SLA_ACK			0x60
#define TWI_STATUS_SR_GENC_ACK			0x70
#define TWI_STATUS_SR_SLA_DATA_ACK		0x80
#define TWI_STATUS_SR_SLA_DATA_NACK		0x88
#define TWI_STATUS_SR_GENC_DATA_ACK 	0x90
#define TWI_STATUS_SR_GENC_DATA_NACK	0x98
#define TWI_STATUS_SR_STOP				0xa0

// slave transmitter mode

#endif /* end of include guard: TWI_H_V8WDZFBC */

