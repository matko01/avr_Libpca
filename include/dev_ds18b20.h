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


// ds18b20 specific commands
#define DS18B20_CMD_ALARM_SEARCH 0xec
#define DS18B20_CMD_CONVERT_T 0x44
#define DS18B20_CMD_WRITE_SP 0x4e
#define DS18B20_CMD_READ_SP 0xbe
#define DS18B20_CMD_COPY_SP 0x48
#define DS18B20_CMD_RECALL_EE 0xb8
#define DS18B20_CMD_READ_POWER 0xb4


void ds18b20_start_conversion(volatile struct soft_ow *a_bus, ow_romcode_t *a_romcode);
void ds18b20_read_scratchpad(volatile struct soft_ow *a_bus, ow_romcode_t *a_romcode, volatile uint8_t *a_data, uint8_t len);
void ds18b20_write_rom(volatile struct soft_ow *a_bus, ow_romcode_t *a_romcode, uint8_t a_th, uint8_t a_tl, uint8_t a_res, uint8_t a_force);


#endif /* end of include guard: DEV_DS18B20_H_1QHHIDKK */

