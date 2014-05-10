#ifndef DEV_HD44780_H_7E8DMXS5
#define DEV_HD44780_H_7E8DMXS5

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
 * @file dev_hd44780.h 
 *
 * @brief Display driver for the hitachi hd44780 based displays
 *
 * This file provides a header for the implementation driving the hd44780 display.
 * Please, refer to the example and functions descriptions for details on using this module with your display
 *
 * The driver may handle multiple display at the same time (just declare multiple display context), It as well supports
 * the 4/8 bit data modes.
 *
 * @example hd44780.c
 *
 * Basic usage of the display api
 */

#include "config.h"
#include "common.h"
#include "gpio.h"

#include <util/delay.h>



#if HD44780_8BIT_MODE == 1
#define HD44780_DATALINES 8
#else
#define HD44780_DATALINES 4
#endif

// ================================================================================

/**
 * @brief waiting time for the display reset
 */
#define HD44780_RESET_DELAY_MS 50

// ================================================================================

/// available fonts
#define HD44780_FONT_5X8 0
#define HD44780_FONT_5X10 1

// ================================================================================

// commands
#define HD44780_CMD_CLRSCR() 0x01
#define HD44780_CMD_HOME() 0x02

/**
 * @brief constructs the ENTRY_MODE SET Command byte for the display
 *
 * Keep in mind that all the those macros only assemble a valid command byte, THEY DO NOT SEND THE ACTUAL BYTE TO THE DISPLAY - you must do it yourself
 * i.e. : hd44780_cmd(a_disp, HD44780_CMD_DISPLAY_CONTROL(1, 0, 0));
 *
 * @param __inc - cursor move direction 1 - increment, 0 - decrement
 * @param __shift - 1 - shift display with every new character
 */
#define HD44780_CMD_ENTRY_MODE(__inc, __shift) \
	(0x04 | (__inc << 1) | __shift)

/**
 * @brief construct the DISPLAY Control command byte
 *
 * @param __onoff - turn the display on/off
 * @param __cursos - turn the cursor on/off
 * @param __blink - tunr on/off cursor blinking
 */
#define HD44780_CMD_DISPLAY_CONTROL(__onoff, __cursor, __blink) \
	(0x08 | (__onoff << 2 ) | (__cursor << 1) | __blink)

/**
 * @brief Constructs the CURSOR OR DISPLAY SHIFT command byte
 *
 * @param __moveshift - 1 - will shift the display by one in the specified direction, 0 - will move cursor instead
 * @param __rightleft - 1 - move/shift right, 0 - move shift left
 */
#define HD44780_CMD_CD_SHIFT(__moveshift, __rightleft) \
	(0x10 | (__moveshift << 3) | (__rightleft << 2))

/**
 * @brief constructs the FUNCTION SET command byte
 *
 * @param __dl - sets interface data length 1 - 8 bit, 0 - 4 bit
 * @param __lines - number of display lines 1 - 2 lines , 0 - 1 line
 * @param __font - specifies display font 1 - 5x10, 0 - 5x8
 */
#define HD44780_CMD_FUNCTION_SET(__dl, __lines, __font) \
	(0x20 | (__dl << 4) | (__lines << 3) | (__font << 2))

/**
 * @brief Construct command byte which sets the address of int CGRAM
 *
 * @param __acg cgram address
 */
#define HD44780_CMD_SET_CGRAM(__acg) \
	(0x40 | __acg)

/**
 * @brief constructs the byte which sets the DDRAM address
 *
 * @param __add ddram address
 */
#define HD44780_CMD_SET_DDRAM(__add) \
	(0x80 | __add)

// ================================================================================

/**
 * @brief display context
 */
struct dev_hd44780_ctx {
	/// reset line
	gpio_pin rs;

	/// enable line
	gpio_pin e;

#if HD44780_USE_RW_LINE == 1
	/// R/W line
	gpio_pin rw;
#endif

	/// data gpio pins - must specify 4 or 8 - dependingly on the configuration
	gpio_pin data[HD44780_DATALINES];

	/// number of display lines (usualy two)
	uint8_t lines;

	/// font setup
	uint8_t font;
};


// ================================================================================


/**
 * @brief Send a command to the display
 *
 * @param __disp display context
 * @param __cmd a command byte
 */
#define hd44780_cmd(__disp, __cmd) \
	hd44780_write(__disp, __cmd, 0); \
	_delay_us(40)


/**
 * @brief Send a data (display RAM) to the display
 *
 * @param __disp display context
 * @param __data the data itself
 */
#define hd44780_data(__disp, __data) \
	hd44780_write(__disp, __data, 1); \
	_delay_us(40)


/**
 * @brief helper macro to send a clear-screen command
 *
 * @param Display context
 */
#define hd44780_clrscr(__disp) \
	hd44780_cmd(a_disp, HD44780_CMD_CLRSCR()); \
	_delay_us(1700)


/**
 * @brief Send a single character to the display
 *
 * @param __disp display context
 * @param __pos character
 */
#define hd44780_putc(__disp, __char) \
	hd44780_data(__disp, __char)


/**
 * @brief Place a cursor at specified position
 *
 * @param __disp display context
 * @param __pos position
 */
#define hd44780_goto(__disp, __pos) \
	hd44780_cmd(__disp, HD44780_CMD_SET_DDRAM(__pos))


/**
 * @brief initialize the display described by the given context
 *
 * The context should define which lines are used by the display
 *
 * @param a_disp display context
 */
void hd44780_init(struct dev_hd44780_ctx *a_disp);


/**
 * @brief Write data to the display
 *
 * @param a_disp display context
 * @param a_data data byte
 * @param a_rs register select value, 0 - instruction register, 1 - data register
 */
void hd44780_write(struct dev_hd44780_ctx *a_disp, uint8_t a_data, uint8_t a_rs);


/**
 * @brief put a string on the display
 *
 * @param a_disp display context
 * @param a_str string to display
 */
void hd44780_puts(struct dev_hd44780_ctx *a_disp, const char *a_str);

// ================================================================================

#if HD44780_USE_RW_LINE == 1

/**
 * @brief checks if the display is busy or not
 *
 * @param a_disp display context
 *
 * @return 1 if busy
 */
uint8_t hd44780_is_busy(struct dev_hd44780_ctx *a_disp);


/**
 * @brief reads a byte from the display (Requires RW line)
 *
 * @param a_disp display context
 * @param a_rs register select value, 0 - instruction register, 1 - data register
 *
 * @return read data byte
 */
uint8_t hd44780_read(struct dev_hd44780_ctx *a_disp, uint8_t a_rs);
#endif


#endif /* end of include guard: DEV_HD44780_H_7E8DMXS5 */

