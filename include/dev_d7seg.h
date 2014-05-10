#ifndef DEV_D7SEG_H_SL6TXHVK
#define DEV_D7SEG_H_SL6TXHVK

#include "config.h"
#include "common.h"
#include "gpio.h"

#include <avr/pgmspace.h>


/**
 * @file dev_d7seg.h 
 *
 * @brief Simple implementation of the 7 segment display multiplexing technique
 *
 * This module provides an easy way to handle your 7 segment displays. It provides a general API and a basic font
 * for 7 segment display.
 *
 * This module defines practically 4 routines only to handle the display. The init routine initialize the display context
 * to a known state, the d7seg_chr_p and d7seg_chr_dot_p macros are used to place a digit into a given display - you can either
 * use a bare number (0-9) as an argument or any value defined in d7s_chars enumeration in order to use the standard built-in font.
 * You don't have to worry again about segment mapping to your font etc - the API takes care about everything for you.
 * The multiplexing routine switches the data on the display data lines and switches the selection line to the next one in order to activate 
 * another display segment - you can use it anywhere - in you standard execution loop as well as in interrupts - it doesn't matter.
 *
 * Since we are relying on static memory allocation you must define the maximum number of segments your display may have (D7SEG_MAX_DISPLAYS) in config.h
 * By default - in order to save memory this macro is configured to 4. Most common displays have only four segments.
 * This driver can support many displays at a time as well. Just define as many context as you want (can), and use the accordingly with the procedures provided.
 *
 * @example d7seg.c
 *
 * Using 7seg display API
 */


/**
 * @brief characters available 
 */
typedef enum _d7s_chars {
	CHAR_0 = 0,
	CHAR_1,
	CHAR_2,
	CHAR_3, 
	CHAR_4,
	CHAR_5,
	CHAR_6,
	CHAR_7,
	CHAR_8,
	CHAR_9,

	CHAR_A,
	CHAR_C,
	CHAR_E,
	CHAR_F,
	CHAR_H,
	CHAR_J,
	CHAR_L,
	CHAR_P,
	CHAR_U,

	CHAR_b,
	CHAR_c,
	CHAR_d,
	CHAR_h,
	CHAR_j,
	CHAR_n,
	CHAR_o,
	CHAR_t,
	CHAR_y,

	CHAR_RSP, // RIGHT SQUARE PARENTHESIS
	CHAR_MINUS,
	CHAR_DEG,
	CHAR_SPACE,

	CHAR_LAST
} d7s_chars;


/**
 * @brief OR your character byte with this definition
 * in order to light up the decimal dot on the display
 */
#define LED_SEG_DOT_MASK 0x80


/**
 * @brief this enum enumerates all the LED segments 
 * of the 7 segment display. It is used to define the GPIO
 * line for every segment
 */
typedef enum _d7s_segments {
	SEG_A = 0,
	SEG_B,
	SEG_C,
	SEG_D,
	SEG_E,
	SEG_F,
	SEG_G,
	SEG_DOT,
	SEG_LAST
} d7s_segments;


/**
 * @brief this structure defines the display context
 *
 * It aggregates all the GPIO lines used with the display
 * and holds current multiplexing state
 */
struct dev_d7seg_ctx {
	/// gpio lines to which particular segments are connected
	gpio_pin segments[SEG_LAST];

	/// gpio lines to which the display selection anodes/cathodes are connected
	gpio_pin sel[D7SEG_MAX_DISPLAYS];
	
	/// currently selected display
	volatile uint8_t _selected;

	/// data buffer for each display
	volatile uint8_t _data[D7SEG_MAX_DISPLAYS];
};



/**
 * @brief unfortunately this is needed for the macros to work
 */
extern const uint8_t g_d7seg_chars[CHAR_LAST] PROGMEM;


/**
 * @brief uses predefined character and places it in the given display's buffer
 * 
 * @param __display display context
 * @param __pos display number
 * @param __char character (as defined in d7s_chars)
 */ 
#define d7seg_chr_p(__display, __pos, __char) \
	(__display)->_data[__pos] = pgm_read_byte(&g_d7seg_chars[__char])


/**
 * @brief uses predefined character and places it in the given display's buffer.
 * Additionally masks the character with the decimal dot.
 * 
 * @param __display display context
 * @param __pos display number
 * @param __char character (as defined in d7s_chars)
 */ 
#define d7seg_chr_dot_p(__display, __pos, __char) \
	(__display)->_data[__pos] = (pgm_read_byte(&g_d7seg_chars[__char]) | LED_SEG_DOT_MASK)


/**
 * @brief initialize the display
 *
 * Configures all the defined GPIO lines as outputs. Zeroes the 
 * buffers.
 *
 * @param a_disp display context
 */
void d7seg_init(volatile struct dev_d7seg_ctx *a_disp);


/**
 * @brief performs display multiplexing
 *
 * Implements the 7 segment display multiplexing. Should be called either in the execution loop
 * or in the timer interrupt or any other (cyclicly occuring one)
 *
 * @param a_disp display context
 */
void d7seg_multiplex(volatile struct dev_d7seg_ctx *a_disp);



#endif /* end of include guard: DEV_D7SEG_H_SL6TXHVK */


