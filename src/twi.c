#include "twi.h"

#include <avr/power.h>
#include <avr/io.h>


#define _twi_start \
	TWCR = (_BV(TWSTA) | _BV(TWINT) | _BV(TWEN))


#define _twi_twint_wait \
	while (!(TWCR & _BV(TWINT)))


#define _twi_send \
	TWCR = (_BV(TWINT) | _BV(TWEN))


#define _twi_stop \
	TWCR = (_BV(TWSTO) | _BV(TWINT) | _BV(TWEN))


#define _twi_status_get \
	(TWSR & 0xf8)


