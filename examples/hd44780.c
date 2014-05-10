#include "pca.h"
#include <string.h>


void main(void) {

	// lcd display
	struct lcd_ctx lcd_ctx;

	memset((void *)&lcd_ctx, 0x00, sizeof(struct lcd_ctx));

	lcd_ctx.rs.port = &LCD_RS_PORT;
	lcd_ctx.rs.pin = LCD_RS_PIN;
	lcd_ctx.e.port = &LCD_E_PORT;
	lcd_ctx.e.pin = LCD_E_PIN;

	// setup data lines
	for (uint8_t x = 0; x<4; x++) {
		lcd_ctx.data[x].port = &LCD_DB_PORT;
		lcd_ctx.data[x].pin = LCD_DB_PIN_FIRST + x;
	}

	// display specifics
	lcd_ctx.lines = LCD_NUMBER_OF_LINES;
	lcd_ctx.font = HD44780_FONT_5X8;

	// initialize the device
	hd44780_init(&lcd_ctx);

	hd44780_goto(&lcd_ctx, 0x00);
	hd44780_puts(&lcd_ctx, "Hello World");

	// execution loop
	for (;;);
}
