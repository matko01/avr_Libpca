#include "soft_onewire.h"
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include <util/atomic.h>


#define SOW_OUTPUT(__ddr, __pin) \
	__ddr |= _BV(__pin)


#define SOW_INPUT(__ddr, __pin) \
	__ddr &= ~_BV(__pin)


#define SOW_HIGH(__port, __pin) \
	__port |= _BV(__pin)


#define SOW_LOW(__port, __pin) \
	__port &= ~_BV(__pin)



void sow_init(struct soft_ow *a_bus) {

	// disable pull-ups
	MCUCR |= _BV(PUD);

	// configure as input, (state high because of pull-up)
	SOW_INPUT(a_bus->ddr, a_bus->pin);

	// sleep for a while
	_delay_us(500);
}


void sow_string_pullup(struct soft_ow *a_bus, uint8_t a_enable) {
	if (a_enable) {
		SOW_OUTPUT(a_bus->ddr, a_bus->pin);
		SOW_HIGH(a_bus->outp, a_bus->pin);
		return;
	}

	SOW_INPUT(a_bus->ddr, a_bus->pin);
}



