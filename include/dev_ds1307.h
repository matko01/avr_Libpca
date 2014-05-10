#ifndef DEV_DS1307_H_GIQURGE1
#define DEV_DS1307_H_GIQURGE1

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



/**
 * @file dev_ds1307.h 
 *
 * @brief Register addresses and definitions of the ds1307 I2c RTC device
 *
 * This header contains the detailed definitions of addresses of registers and the data layout in them for
 * the ds1307 RTC device. Have a look on provided example in order to familiarize yourself with the usage.
 *
 * @example ds1307.c
 *
 * Using using the ds1307_time_t structure.
 */



// time date elements addresses
#define DS1307_SECONDS_ADDR 0x00
#define DS1307_MINUTES_ADDR 0x01
#define DS1307_HOURS_ADDR 0x02
#define DS1307_DOW_ADDR 0x03
#define DS1307_DOM_ADDR 0x04
#define DS1307_MONTHS_ADDR 0x05
#define DS1307_YEAR_ADDR 0x06


// control register address
#define DS1307_CONTROL_ADDR 0x07


// NVRAM address
#define DS1307_NVRAM_ADDR 0x08
#define DS1307_NVRAM_SIZE 0x38


// control register bits
#define DS1307_CONTROL_BIT_OUT 0x07
#define DS1307_CONTROL_BIT_SQW 0x04
#define DS1307_CONTROL_BIT_RS1 0x01
#define DS1307_CONTROL_BIT_RS0 0x00


// possible SQW out frequencies and setting of the RS bits
#define DS1307_SQW_FREQ_1HZ 0x00
#define DS1307_SQW_FREQ_4096K 0x01
#define DS1307_SQW_FREQ_8192K 0x02
#define DS1307_SQW_FREQ_32768K 0x03


/**
 * @brief time structure
 */
typedef struct _ds1307_time_t {
	// bit 7 - clock halt
	uint8_t ch_sec;	
	uint8_t min;
	// 7 - 0
	// 6 - 1 = 12, 0 = 24 hour mode
	// 5 - 1 = pm, 0 = am or second digit in 24 hour mode
	uint8_t mode_ampm_hour;
	uint8_t dow; // day of week
	uint8_t dom; // day of month
	uint8_t month;
	uint8_t year;
} ds1307_time_t;




#endif /* end of include guard: DEV_DS1307_H_GIQURGE1 */

