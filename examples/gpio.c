#include "pca.h"
#include <util/delay.h>


void blink(gpio_pin *p) {
	while (1) {
		GPIO_TOGGLE(p);
		_delay_ms(500);
	}
}


int main(int argc, char *argv[])
{
	gpio_pin a;
	uint8_t value = 0;

	// setup the gpio
	a.port = &PORTB;
	a.pin = 0;

	// configure as input
	GPIO_CONFIGURE_AS_INPUT(&a);

	// get the value
	value = GPIO_GET(&a);

	// configure as output
	GPIO_CONFIGURE_AS_OUTPUT(&a);

	// manipulating the state
	GPIO_SET_LOW(&a);
	GPIO_SET_HIGH(&a);

	// passing gpio to a function
	blink(a);

	return 0;
}
