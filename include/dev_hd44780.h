#ifndef DEV_HD44780_H_7E8DMXS5
#define DEV_HD44780_H_7E8DMXS5

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

#define HD44780_CMD_ENTRY_MODE(__inc, __shift) \
	(0x04 | (__inc << 1) | __shift)

#define HD44780_CMD_DISPLAY_CONTROL(__onoff, __cursor, __blink) \
	(0x08 | (__onoff << 2 ) | (__cursor << 1) | __blink)

#define HD44780_CMD_CD_SHIFT(__moveshift, __rightleft) \
	(0x10 | (__moveshift << 3) | (__rightleft << 2))

#define HD44780_CMD_FUNCTION_SET(__dl, __lines, __font) \
	(0x20 | (__dl << 4) | (__lines << 3) | (__font << 2))

#define HD44780_CMD_SET_CGRAM(__acg) \
	(0x40 | __acg)

#define HD44780_CMD_SET_DDRAM(__add) \
	(0x80 | __add)

// ================================================================================

/**
 * @brief display context
 */
struct dev_hd44780_ctx {
	gpio_pin rs;
	gpio_pin e;

#if HD44780_USE_RW_LINE == 1
	gpio_pin rw;
#endif

	gpio_pin data[HD44780_DATALINES];

	// number of display lines
	uint8_t lines;

	// font setup
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
uint8_t hd44780_is_busy(struct dev_hd44780_ctx *a_disp);
uint8_t hd44780_read(struct dev_hd44780_ctx *a_disp, uint8_t a_rs);
#endif


#endif /* end of include guard: DEV_HD44780_H_7E8DMXS5 */

