#include "dev_d7seg.h"


/**
 * @brief list of all character definitions
 *
 * This variable will be placed in flash
 *
 */
const uint8_t g_d7seg_chars[CHAR_LAST] PROGMEM = {
	0x3f, // 0
	0x06, // 1
	0x5b, // 2
	0x4f, // 3
	0x66, // 4
	0x6d, // 5
	0x7d, // 6
	0x27, // 7
	0x7f, // 8
	0x6f, // 9

	0x77, // A
	0x39, // C
	0x79, // E
	0x71, // F
	0x76, // H
	0x0e, // J
	0x38, // L
	0x73, // P
	0x3e, // U

	0x7c, // b
	0x58, // c
	0x5e, // d
	0x74, // h
	0x0c, // j
	0x54, // n
	0x5c, // o
	0x78, // t
	0x6e, // y

	0x0f, // ]
	0x40, // -
	0x63, // DEG
	0x00, // SPACE
};

/* ================================================================================ */

/**
 * @brief update the gpio lines with the data from currently selected
 * display's data buffer
 *
 * @param a_disp display context structure
 */
static void _d7seg_update(volatile struct dev_d7seg_ctx *a_disp);

/* ================================================================================ */

void d7seg_multiplex(volatile struct dev_d7seg_ctx *a_disp) {
	GPIO_SET_LOW(&a_disp->sel[a_disp->_selected]);

	a_disp->_selected--;
	if (a_disp->_selected > (D7SEG_MAX_DISPLAYS - 1)) {
		a_disp->_selected = D7SEG_MAX_DISPLAYS - 1;
	}	

	_d7seg_update(a_disp);
	GPIO_SET_HIGH(&a_disp->sel[a_disp->_selected]);
}


void d7seg_init(volatile struct dev_d7seg_ctx *a_disp) {
	uint8_t len = D7SEG_MAX_DISPLAYS;
	a_disp->_selected = (D7SEG_MAX_DISPLAYS - 1);

	common_zero_mem(a_disp->_data, len);

	for (uint8_t i = 0; i<SEG_LAST; i++) {
		GPIO_CONFIGURE_AS_OUTPUT(&a_disp->segments[i]);
	}

	for (uint8_t i = 0; i<D7SEG_MAX_DISPLAYS; i++) {
		GPIO_CONFIGURE_AS_OUTPUT(&a_disp->sel[i]);
	}

	_d7seg_update(a_disp);
}

/* ================================================================================ */

static void _d7seg_update(volatile struct dev_d7seg_ctx *a_disp) {

	for (uint8_t i = 0; i < SEG_LAST; i++) {
		if (a_disp->_data[a_disp->_selected] & (0x01 << i)) {
			GPIO_SET_HIGH(&a_disp->segments[i])
		}
		else {
			GPIO_SET_LOW(&a_disp->segments[i])
		}
	}
}

/* ================================================================================ */
