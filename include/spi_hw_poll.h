#ifndef SPI_HW_POLL_H_JM4LGPKV
#define SPI_HW_POLL_H_JM4LGPKV

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


#include <config.h>
#include <common.h>

#include <bus.h>
#include <spi_common.h>


typedef enum _spi_speed {
	E_SPI_SPEED_F4 = 0,
	E_SPI_SPEED_F16,
	E_SPI_SPEED_F64,
	E_SPI_SPEED_F128,
	E_SPI_SPEED_F2,
	E_SPI_SPEED_F8,
	E_SPI_SPEED_F32,
} spi_speed;


void spi_hw_poll_init(spi_mode mode, spi_speed);
void spi_hw_poll_clock_mode_set(spi_clock_mode mode);

uint8_t spi_hw_poll_getc(uint8_t *a_data);
uint8_t spi_hw_poll_sendc(uint8_t *a_data);
uint8_t spi_hw_poll_recv(void *a_data, uint8_t a_len, uint8_t a_waitall);
uint8_t spi_hw_poll_send(void *a_data, uint8_t a_len, uint8_t a_waitall);
uint8_t spi_hw_poll_available(void);

struct bus_t spi_hw_poll_bus_get();


#endif /* end of include guard: SPI_HW_POLL_H_JM4LGPKV */


