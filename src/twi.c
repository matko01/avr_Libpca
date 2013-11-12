#include "twi.h"

#include <avr/power.h>
#include <avr/io.h>
#include <util/twi.h>


#define _twi_start \
	TWCR = (_BV(TWSTA) | _BV(TWINT) | _BV(TWEN))


#define _twi_twint_wait \
	while (!(TWCR & _BV(TWINT)))


#define _twi_send \
	TWCR = (_BV(TWINT) | _BV(TWEN))


#define _twi_stop \
	TWCR = (_BV(TWSTO) | _BV(TWINT) | _BV(TWEN))


#define _twi_slave_address_set(__address) \
	TWAR = __address << 1


