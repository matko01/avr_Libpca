#ifndef DEV_DS18B20_H_1QHHIDKK
#define DEV_DS18B20_H_1QHHIDKK

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

#include <config.h>
#include <common.h>
#include <soft_onewire.h>
#include <soft_onewire_network.h>


/**
 * @file dev_ds18b20.h 
 *
 * @brief Basic API for the ds18b20 digital one wire temperature sensor.
 *
 * This header contains the detailed definitions of addresses of registers and the data layout in them for
 * the ds1307 RTC device. Have a look on provided example in order to familiarize yourself with the usage.
 *
 * @example ds18b20.c
 *
 * Using the ds18b20 API
 */


#warning SINGLEDROP support implemented only at the moment

// ds18b20 specific commands
#define DS18B20_CMD_ALARM_SEARCH 0xec
#define DS18B20_CMD_CONVERT_T 0x44
#define DS18B20_CMD_WRITE_SP 0x4e
#define DS18B20_CMD_READ_SP 0xbe
#define DS18B20_CMD_COPY_SP 0x48
#define DS18B20_CMD_RECALL_EE 0xb8
#define DS18B20_CMD_READ_POWER 0xb4


/**
 * @brief triggers start conversion of a given sensor
 *
 * @param a_bus software one wire bus context
 * @param a_romcode device romcode (or NULL if only one device on the bus)
 */
void ds18b20_start_conversion(volatile struct soft_ow *a_bus, ow_romcode_t *a_romcode);


/**
 * @brief Read the contents of the device's scratchpad
 *
 * @param a_bus software one wire bus context
 * @param a_romcode device romcode (or NULL if only one device on the bus)
 * @param a_data pointer for the destination buffer (to which the data will be written)
 * @param len maximum length of the data to read
 */
void ds18b20_read_scratchpad(volatile struct soft_ow *a_bus, ow_romcode_t *a_romcode, volatile uint8_t *a_data, uint8_t len);


/**
 * @brief This function updates the persistent sensor configuration
 *
 * It updates the contents of the RAM scratchpad and copies them explicitly to the internal ROM. By default (force = 0) it won't rewrite ROM
 * when the given settings are identical with the one already in the sensor
 *
 * @param a_bus software one wire bus context
 * @param a_romcode device romcode (or NULL if only one device on the bus)
 * @param a_th high temperature alarm value
 * @param a_tl low temperature alarm value
 * @param a_res sampling resolution
 * @param a_force force update
 */
void ds18b20_write_rom(volatile struct soft_ow *a_bus, ow_romcode_t *a_romcode, uint8_t a_th, uint8_t a_tl, uint8_t a_res, uint8_t a_force);


#endif /* end of include guard: DEV_DS18B20_H_1QHHIDKK */

