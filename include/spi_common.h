#ifndef SPI_COMMON_H_RYRWDC2M
#define SPI_COMMON_H_RYRWDC2M

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


#include <common.h>


/**
 * @brief bus role
 */
typedef enum _spi_mode {
	E_SPI_MODE_SLAVE = 0,
	E_SPI_MODE_MASTER,
} spi_mode;


/**
 * @brief spi mode
 */
typedef enum _spi_clock_mode {
	E_CLOCK_MODE0 = 0,
	E_CLOCK_MODE1,
	E_CLOCK_MODE2,
	E_CLOCK_MODE3,
} spi_clock_mode;


#endif /* end of include guard: SPI_COMMON_H_RYRWDC2M */

