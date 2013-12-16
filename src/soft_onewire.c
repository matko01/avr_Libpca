#include <soft_onewire.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include <util/atomic.h>

/* ================================================================================ */

void sow_init(volatile struct soft_ow *a_bus) {

	// disable pull-ups
	// MCUCR |= _BV(PUD);

	// configure as input, (state high because of pull-up)
	GPIO_CONFIGURE_AS_INPUT(&a_bus->bus);

	// sleep for a while
	_delay_us(500);
}


uint8_t sow_reset(volatile struct soft_ow *a_bus) {

	uint8_t presence = 0;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		GPIO_CONFIGURE_AS_OUTPUT(&a_bus->bus);
		GPIO_SET_LOW(&a_bus->bus);
		_delay_us(500);

		GPIO_CONFIGURE_AS_INPUT(&a_bus->bus);
		_delay_us(50); // 15 - 60

		if (!GPIO_GET(&a_bus->bus)) {
			// after 60 - 240
			_delay_us(300); 
			presence = (GPIO_GET(&a_bus->bus) ? 0x01 : 0x00);
		} // bit is clear
	} // ATOMIC_BLOCK
	
	return presence;
}


void sow_write_byte(volatile struct soft_ow *a_bus, uint8_t a_byte) {
	uint8_t n = 0;

	for (; n<8; n++) {
		_sow_write_bit(a_bus, ((a_byte >> n) & 0x01));
	}
	
}


uint8_t sow_read_byte(volatile struct soft_ow *a_bus) {
	uint8_t n = 0;
	uint8_t data = 0x00;

	for (; n<8; n++) {
		data |= (_sow_read_bit(a_bus) << n);
	}
	
	return data;
}


void sow_write_data(volatile struct soft_ow *a_bus, uint8_t *a_data, uint8_t a_len) {

	while (a_len--) {
		sow_write_byte(a_bus, (*a_data++));
	}

}


void sow_read_data(volatile struct soft_ow *a_bus, uint8_t *a_data, uint8_t a_maxlen) {

	while (a_maxlen--) {
		(*a_data++) = sow_read_byte(a_bus);
	}

}


/* ================================================================================ */


void _sow_write_bit(volatile struct soft_ow *a_bus, uint8_t a_bit) {
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {

		GPIO_CONFIGURE_AS_OUTPUT(&a_bus->bus);
		GPIO_SET_LOW(&a_bus->bus);

		_delay_us(5); // max 15
		
		if (a_bit & 0x01)
			GPIO_CONFIGURE_AS_INPUT(&a_bus->bus);

		_delay_us(80);
		GPIO_CONFIGURE_AS_INPUT(&a_bus->bus);
	}
}


uint8_t _sow_read_bit(volatile struct soft_ow *a_bus) {
	uint8_t bit = 0;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		GPIO_CONFIGURE_AS_OUTPUT(&a_bus->bus);
		GPIO_SET_LOW(&a_bus->bus);

		_delay_us(1); // >= 1
		GPIO_CONFIGURE_AS_INPUT(&a_bus->bus);

		_delay_us(12); // sample within 15u slot
		bit = (GPIO_GET(&a_bus->bus) ? 0x01 : 0x00);

		// time slot duration + recovery time
		_delay_us(60 + 1);
	}
	return bit;
}

/* ================================================================================ */
