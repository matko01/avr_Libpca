#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <util/delay.h>
#include <math.h>

#include "main.h"
#include "pca.h"



int main(void)
{
	struct dev_pcd8544_ctx lcd;
	struct bus_t spi_bus;

	// display buffer
	uint8_t buffer[PCD8544_W * (PCD8544_H/8)] = {0x00};

	spi_bus = spi_hw_poll_bus_get();
	lcd.bus = &spi_bus;

	lcd.sce.port = &PORTB;
	lcd.sce.pin = PORTB0;

	lcd.dc.port = &PORTB;
	lcd.dc.pin = PORTB1;

	lcd.res.port = &PORTB;
	lcd.res.pin = PORTB2;

	spi_hw_poll_init(E_SPI_MODE_MASTER, E_SPI_SPEED_F2);
	pcd8544_init(&lcd);
	pcd8544_clrscr(&lcd);
	pcd8544_install_stdout(&lcd);

	// using the stdout
	printf("Hello world");

	/* example 2: writing graphics data to the display */

	pcd8544_gotoxy(&lcd, 0, 0);
	uint8_t buf = 0;

	for (uint8_t x = 0; x<PCD8544_W; x++) {
		for (uint8_t y = 0; y<PCD8544_H; y++) {
			// this is actually a clever way to generate a sierpinski triangle
			if (!(x&y)) {
				PCD8544_SET_BIT_HIGH(buffer, x,y);
			}
		}
	}

	// copy the display buffer to the display itself
	pcd8544_blit(&lcd, buffer);

	for (;;);
	return 0;
}
