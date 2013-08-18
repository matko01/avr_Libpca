#include "timer_common.h"
#include <avr/interrupt.h>

void _timer_enable_interrupt(e_timer a_timer) {
	// which pin
	switch(a_timer) {
		case E_TIMER0:
			TIMSK0 |= _BV(OCIE0A);
			break;

		case E_TIMER1:
			TIMSK1 |= _BV(OCIE1A);
			break;

		case E_TIMER2:
			TIMSK2 |= _BV(OCIE2A);
			break;

		default:
			break;
	} // switch
}


void _timer_disable_interrupt(e_timer a_timer) {
	// which pin
	switch(a_timer) {
		case E_TIMER0:
			TIMSK0 &= ~_BV(OCIE0A);
			break;

		case E_TIMER1:
			TIMSK1 &= ~_BV(OCIE1A);
			break;

		case E_TIMER2:
			TIMSK2 &= ~_BV(OCIE2A);
			break;

		default:
			break;
	} // switch
}


uint32_t _timer_freq_prescale(uint32_t a_freq, uint16_t a_criterion) {
	uint8_t prescalers[] = { 0x00, 0x03, 0x06, 0x08, 0x0a, 0x00 };

	/**
	 * combine both prescaler and ocr value in one 32bit value 
	 */
	uint32_t retval = 0x00;
	uint16_t *ocr = (uint16_t *)&retval;
	uint8_t *presc = ((uint8_t *)&retval) + 3;

	do {
		*ocr = F_CPU / ((a_freq << 1) * (0x01 << prescalers[*presc]));
		++*presc;		
	} while ((*ocr > a_criterion) && (prescalers[*presc]));

	/* --*ocr; */
	if (*ocr > a_criterion) *ocr = a_criterion;
	return retval;
}
