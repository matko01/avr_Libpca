#include <soft_onewire.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include <util/atomic.h>


#define SOW_OUTPUT(__ddr, __pin) \
	(*__ddr) |= _BV(__pin)


#define SOW_INPUT(__ddr, __pin) \
	(*__ddr) &= ~_BV(__pin)


#define SOW_HIGH(__port, __pin) \
	(*__port) |= _BV(__pin)


#define SOW_LOW(__port, __pin) \
	(*__port) &= ~_BV(__pin)


/* ================================================================================ */


void sow_init(struct soft_ow *a_bus) {

	// disable pull-ups
	MCUCR |= _BV(PUD);

	// configure as input, (state high because of pull-up)
	SOW_INPUT(a_bus->ddr, a_bus->pin);

	// sleep for a while
	_delay_us(500);
}


void sow_strong_pullup(struct soft_ow *a_bus, uint8_t a_enable) {
	if (a_enable) {
		SOW_OUTPUT(a_bus->ddr, a_bus->pin);
		SOW_HIGH(a_bus->outp, a_bus->pin);
		return;
	}

	SOW_INPUT(a_bus->ddr, a_bus->pin);
}


uint8_t sow_reset(struct soft_ow *a_bus) {

	uint8_t presence = 0;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		SOW_OUTPUT(a_bus->ddr, a_bus->pin);
		SOW_LOW(a_bus->outp, a_bus->pin);
		_delay_us(500);

		SOW_INPUT(a_bus->ddr, a_bus->pin);
		_delay_us(50); // 15 - 60

		if (!bit_is_clear(*(a_bus->inp), a_bus->pin)) {
			// after 60 - 240
			_delay_us(300); 
			presence = (bit_is_set(*(a_bus->inp), a_bus->pin) ? 0x01 : 0x00);
		} // bit is clear
	} // ATOMIC_BLOCK
	
	return presence;
}


uint8_t sow_write_byte(struct soft_ow *a_bus, uint8_t a_byte) {
	uint8_t n = 0;

	for (; n<8; n++) {
		_sow_write_bit(a_bus, ((a_byte >> n) & 0x01));
	}
	
	return 1;
}


uint8_t sow_read_byte(struct soft_ow *a_bus) {
	uint8_t n = 0;
	uint8_t data = 0x00;

	for (; n<8; n++) {
		data |= (_sow_read_bit(a_bus) << n);
	}
	
	return data;
}


uint8_t sow_write_data(struct soft_ow *a_bus, uint8_t *a_data, uint8_t a_len) {
	uint8_t n = a_len;

	while (a_len--) {
		sow_write_byte(a_bus, (*a_data++));
	}

	return n;
}


uint8_t sow_read_data(struct soft_ow *a_bus, uint8_t *a_data, uint8_t a_maxlen) {
	uint8_t n = a_maxlen;

	while (a_maxlen--) {
		(*a_data++) = sow_read_byte(a_bus);
	}

	return n;
}


/* ================================================================================ */


uint8_t _sow_write_bit(struct soft_ow *a_bus, uint8_t a_bit) {
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		SOW_OUTPUT(a_bus->ddr, a_bus->pin);
		SOW_LOW(a_bus->outp, a_bus->pin);

		_delay_us(5); // max 15
		
		if (a_bit & 0x01)
			SOW_INPUT(a_bus->ddr, a_bus->pin);

		_delay_us(80);
		SOW_INPUT(a_bus->ddr, a_bus->pin);
	}
	return 1;
}


uint8_t _sow_read_bit(struct soft_ow *a_bus) {
	uint8_t bit = 0;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		SOW_OUTPUT(a_bus->ddr, a_bus->pin);
		SOW_LOW(a_bus->outp, a_bus->pin);

		_delay_us(1); // >= 1
		SOW_INPUT(a_bus->ddr, a_bus->pin);

		_delay_us(12); // sample within 15u slot
		bit = (bit_is_set(*(a_bus->inp), a_bus->pin)) ? 0x01 : 0x00;

		// time slot duration + recovery time
		_delay_us(60 + 1);
	}
	return bit;
}

/* ================================================================================ */
