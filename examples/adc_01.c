#include "pca.h"
#include <util/delay.h>

int main(void)
{
	uint16_t result = 0x00;

	// initialize serial port
	serial_init(E_BAUD_9600);
	serial_install_interrupts(E_FLAGS_SERIAL_RX_INTERRUPT);
	serial_flush();
	serial_install_stdio();


	// initialize adc single shot
	adc_init(E_SINGLE_SHOT);

	// select internal temperature sensor
	adc_temperature_sensor_enable();

	while (1) {
		
		// trigger conversion
		adc_conversion_trigger();

		// get the result
		adc_result_poll_get(result);

		// print it to the serial line
		printf("Value: [%d]\n", result);
		_delay_ms(500);
	}


	return 0;
}
