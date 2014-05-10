#include "pca.h"
#include <string.h>


void main(void) {

	// TWI interface
	volatile struct twi_ctx *twi_ctx = NULL;

	// time
	ds1307_time_t tm; 
	memset((void *)&tm, 0x00, sizeof(tm));

	// setup bus
	twi_ctx = twi_init(E_TWI_SCL_100K);

	// enable interrupts
	sei();

	// execution loop
	for (;;) {

		uint8_t ptr = 0x00;
		twi_mtx(TWI_RTC_ADDR, &ptr, 0x01, 0x00);

		// wait for the bus
		while (twi_ctx->status & E_TWI_BIT_BUSY);

		// read the RTC data
		twi_mrx(TWI_RTC_ADDR, (uint8_t *)&tm, 
					sizeof(tm),
					E_TWI_BIT_SEND_STOP);

		// wait for the bus
		while (twi_ctx->status & E_TWI_BIT_BUSY);

		/* at this stage the tm structure contains all the relevant RTC specifc data */
		/* read from the ds1307 device */

		// poll every 100 ms 
		_delay_ms(100);
	} // for
}
