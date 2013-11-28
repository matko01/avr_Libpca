#include <dev_hd44780.h>
#include <util/delay.h>

/* ================================================================================ */

static void _hd44780_data_latch(struct dev_hd44780_ctx *a_disp);

/* ================================================================================ */

void hd44780_init(struct dev_hd44780_ctx *a_disp) {
	
	uint8_t x = HD44780_DATALINES;

	GPIO_CONFIGURE_AS_OUTPUT(&a_disp->rs);
	GPIO_CONFIGURE_AS_OUTPUT(&a_disp->e);

#if HD44780_USE_RW_LINE == 1
	GPIO_CONFIGURE_AS_OUTPUT(&a_disp->rw);
	GPIO_SET_LOW(&a_disp->rw);
#endif

	// set control lines low
	GPIO_SET_LOW(&a_disp->rs);
	GPIO_SET_LOW(&a_disp->e);

	// set all data lines high
	while (x--) {
		GPIO_CONFIGURE_AS_OUTPUT(&a_disp->data[x]);
		GPIO_SET_HIGH(&a_disp->data[x]);
	}

	// wait for the display
	_delay_ms(HD44780_RESET_DELAY_MS);

#if HD44780_8BIT_MODE == 1
	GPIO_SET_LOW(&a_disp->data[7]);
	GPIO_SET_LOW(&a_disp->data[6]);
#else
	GPIO_SET_LOW(&a_disp->data[3]);
	GPIO_SET_LOW(&a_disp->data[2]);
#endif

	_hd44780_data_latch(a_disp);
	_delay_ms(5);

	x=4;
	while (--x) {
		_hd44780_data_latch(a_disp);
		_delay_us(200);
	}

#if HD44780_8BIT_MODE == 0
	GPIO_SET_LOW(&a_disp->data[0]);
#endif

	_hd44780_data_latch(a_disp);

	// wait for a while before sending the "real" commands
	_delay_us(10);

	// set font and display lines
#if HD44780_8BIT_MODE == 1
	hd44780_cmd(a_disp, HD44780_CMD_FUNCTION_SET(1, (a_disp->lines >= 2), a_disp->font));
#else
	hd44780_cmd(a_disp, HD44780_CMD_FUNCTION_SET(0, (a_disp->lines >= 2), a_disp->font));
#endif

	// display on, cursor off, blink off
	hd44780_cmd(a_disp, HD44780_CMD_DISPLAY_CONTROL(1, 0, 0));

	// clear the screen
	hd44780_clrscr(a_disp);

	// set entry mode
	hd44780_cmd(a_disp, HD44780_CMD_ENTRY_MODE(1, 0));
}


void hd44780_write(struct dev_hd44780_ctx *a_disp, uint8_t a_data, uint8_t a_rs) {	

	uint8_t x = HD44780_DATALINES;

#if HD44780_8BIT_MODE == 1
	uint8_t nibbles = 1;
#else
	uint8_t nibbles = 3;
#endif

	if (a_rs)
		GPIO_SET_HIGH(&a_disp->rs);
	else
		GPIO_SET_LOW(&a_disp->rs);

#if HD44780_8BIT_MODE == 0
	while (--nibbles) {
#endif

		x = HD44780_DATALINES;
		while (x--) {
			if (a_data & (0x01 << (x + 4*(nibbles-1))))
				GPIO_SET_HIGH(&a_disp->data[x]);
			else
				GPIO_SET_LOW(&a_disp->data[x]);
		}

		// latch the data
		_hd44780_data_latch(a_disp);

#if HD44780_8BIT_MODE == 0
	}
#endif
}


void hd44780_puts(struct dev_hd44780_ctx *a_disp, const char *a_str) {
	while (*a_str) hd44780_putc(a_disp, *a_str++);
}


#if HD44780_USE_RW_LINE == 1
uint8_t hd44780_read(struct dev_hd44780_ctx *a_disp, uint8_t a_rs) {

}
#endif

/* ================================================================================ */

static void _hd44780_data_latch(struct dev_hd44780_ctx *a_disp) {
		GPIO_SET_LOW(&a_disp->e);
		_delay_us(1);
		GPIO_SET_HIGH(&a_disp->e);
		_delay_us(1);
		GPIO_SET_LOW(&a_disp->e);
}

/* ================================================================================ */
