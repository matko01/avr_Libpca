#ifndef DEV_PCD8544_H_PN3AJ51E
#define DEV_PCD8544_H_PN3AJ51E

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
 * @file dev_pcd8544.h 
 *
 * @brief Display driver for the nokia based pcd8544 displays
 *
 * This module implements a driver for the standard pcd8544 nokia display. This driver uses the pca bus abstraction
 * layer and works on top of any provided bus_t compatible SPI abstraction layer. Have a look on the example for details of using the driver
 *
 * @example pcd8544.c
 *
 * Basic usage of the display api
 */


#include <config.h>
#include <common.h>
#include <bus.h>
#include <gpio.h>


/**
 * @brief enumerates all the possible display modes
 */
typedef enum _pcd8544_mode {
	E_DISPLAY_BLANK = 0,
	E_DISPLAY_NORMAL = 0x04,
	E_DISPLAY_ALLON = 0x01,
	E_DISPLAY_INVERSE = 0x05,
} pcd8544_mode;


#define PCD8544_CMD 0x00
#define PCD8544_DATA 0x01

#define PCD8544_W 84
#define PCD8544_H 48

#define PCD8544_CMD_FUNCTION_SET(__pd, __v, __h) \
	(0x20 | (__pd << 2) | (__v << 1) | (__h))

#define PCD8544_CMD_SET_X(__x) \
	(0x80 | __x)

#define PCD8544_CMD_SET_Y(__y) \
	(0x40 | (__y & 0x07))

#define PCD8544_CMD_DISPLAY_CTRL(__d) \
	(0x08 | (__d & 0x07))

#define PCD8544_CMD_TMP_CTRL(__tc) \
	(0x04 | (__tc & 0x03))

#define PCD8544_CMD_BIAS(__bias) \
	(0x10 | (__bias & 0x07))

#define PCD8544_CMD_SET_VOP(__vop) \
	(0x80 | (__vop))

#define PCD8544_SET_BIT_HIGH(__mem, __x, __y) \
	__mem[x + (y >> 3)*PCD8544_W] |= _BV( y - ((y >> 3) << 3) )

#define PCD8544_SET_BIT_LOW(__mem, __x, __y) \
	__mem[x + (y >> 3)*PCD8544_W] &= ~_BV( y - ((y >> 3) << 3) )



/**
 * @brief display context
 */
struct dev_pcd8544_ctx {
	struct bus_t *bus;

	/// serial chip enable
	gpio_pin sce;

	/// data/control 1/0
	gpio_pin dc;

	/// reset - active low
	gpio_pin res;

	/// helper variables store current x,y display pointer
	/// it's more convenient to track them manually
	/// since it's impossible to read it out of the display itself
	uint8_t x,y;
};


/**
 * @brief initializes the display
 *
 * @param a_disp display context (must contain all the gpio's defined and a pointer to working SPI bus)
 */
void pcd8544_init(struct dev_pcd8544_ctx *a_disp);


/**
 * @brief clear the display
 *
 * @param a_disp display context
 */
void pcd8544_clrscr(struct dev_pcd8544_ctx *a_disp);


/**
 * @brief move the internal display pointer to given coordinates
 *
 * @param a_disp display context
 * @param x 0 <= x < 84
 * @param y 0 <= y < 6 (8 rows per byte)
 */
void pcd8544_gotoxy(struct dev_pcd8544_ctx *a_disp, uint8_t x, uint8_t y);


/**
 * @brief set the specified display as the stdout 
 *
 * After that you can use functions like printf to write to the display directly

 * @param a_disp display context
 */
void pcd8544_install_stdout(struct dev_pcd8544_ctx *a_disp);


/**
 * @brief write data byte to the display
 *
 * @param a_disp display context 
 * @param mode data/control byte (PCD8544_DATA/PCD8544_CMD)
 * @param data data
 */
void pcd8544_write(struct dev_pcd8544_ctx *a_disp, uint8_t mode, uint8_t data);


/**
 * @brief wrapper for pcd8544_write
 *
 * writes a single character to the display using standard font
 *
 * @param a_disp display context
 * @param c ASCII character to be written
 */
void pcd8544_putc(struct dev_pcd8544_ctx *a_disp, char c);


/**
 * @brief wrapper for pcd8544_write
 *
 * write a string to the display at current position (using standard font)
 *
 * @param a_disp display context
 * @param str string to be written
 */
void pcd8544_str(struct dev_pcd8544_ctx *a_disp, char *str);


/**
 * @brief write a single display data block to the display
 *
 * @param a_disp display context
 * @param x 0 <= x < 84
 * @param y 0 <= y < 6 (8 rows per byte)
 * @param c data
 */
void pcd8544_putblock(struct dev_pcd8544_ctx *a_disp, uint8_t x, uint8_t y, uint8_t c);


/**
 * @brief blit the enire display buffer (84*6) to the display
 *
 * @param a_disp display context
 * @param data data buffer (must be of size at least 84*6)
 */
void pcd8544_blit(struct dev_pcd8544_ctx *a_disp, uint8_t *data);


/**
 * @brief fill the whole display with the pattern specified in the byte
 *
 * @param a_disp display context
 * @param data pattern
 */
void pcd8544_fill(struct dev_pcd8544_ctx *a_disp, uint8_t data);


#endif /* end of include guard: DEV_PCD8544_H_PN3AJ51E */

