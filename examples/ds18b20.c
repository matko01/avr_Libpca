#include "pca.h"
#include <string.h>
#include <util/delay.h>

#define OW_PORT PORTB
#define OW_PIN 0

void main(void) {
	// one wire bus
	struct soft_ow sow_ctx;

	// temperature reading
	uint16_t temperature = 0x00;
	
	// configure the bus
	sow_ctx.conf = SOFT_OW_CONF_MAKE(OW_POWER_PARASITE, OW_SINGLEDROP);
	sow_ctx.bus.port = &OW_PORT;
	sow_ctx.bus.pin = OW_PIN;

	// initialize one-wire
	sow_init(&sow_ctx);

	// perform HW init 12 bit resolution
	ds18b20_write_rom(&sow_ctx, NULL, 0x00, 0x00, 0x03, 0x00);

	// execution loop
	for (;;) {

		// start conversion
		ds18b20_start_conversion(&sow_ctx, NULL);

		// wait for the conversion to finnish
		_delay_ms(800);

		// read the temperature
		ds18b20_read_scratchpad(&sow_ctx, 
			NULL, 
			(uint8_t *)&temperature, 2);

	}
}
