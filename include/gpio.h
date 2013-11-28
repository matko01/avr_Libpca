#ifndef GPIO_H_TUGJ3L7E
#define GPIO_H_TUGJ3L7E

#include "config.h"
#include "common.h"

#include <avr/io.h>


/**
 * @brief structure describing a single io line
 */
typedef struct _gpio_pin {
	volatile uint8_t *port;
	uint8_t pin;
} gpio_pin;


#define GPIO_CONFIGURE_AS_OUTPUT(__gpio) \
	*(GET_DDRX_FROM_PORTX((__gpio)->port)) |= _BV((__gpio)->pin)
	

#define GPIO_CONFIGURE_AS_INPUT(__gpio) \
	*(GET_DDRX_FROM_PORTX((__gpio)->port)) &= ~_BV((__gpio)->pin)


#define GPIO_SET_LOW(__gpio) \
	*(__gpio)->port &= ~_BV((__gpio)->pin)


#define GPIO_SET_HIGH(__gpio) \
	*(__gpio)->port |= _BV((__gpio)->pin)



#endif /* end of include guard: GPIO_H_TUGJ3L7E */

