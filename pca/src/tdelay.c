#include "tdelay.h"

#include <avr/power.h>
#include <avr/interrupt.h>

/* ================================================================================ */

static void _tdelay_setup_ms(e_timer a_timer, uint32_t a_delay) {
	_tdc_setup_delay(_tdc_get_tdelay_timer(a_timer), 1000, a_delay);
}


static void _tdelay_setup_us(e_timer a_timer, uint32_t a_delay) {
	_tdc_setup_delay(_tdc_get_tdelay_timer(a_timer), 1000000, a_delay);
}

/* ================================================================================ */

void tdelay_init(e_timer a_timer) {

	// enable interrupts
	sei();
	switch (a_timer) {

		case E_TIMER0:
			power_timer0_enable();
			// clock disabled
			TCCR0A = 0x02;
			TCCR0B = 0x00;
			TCNT0 = 0x00;
			OCR0A = 0x00;
			break;

		case E_TIMER1:
			power_timer1_enable();
			// clock disabled, CTC mode
			TCCR1A = 0x00;
			TCCR1B = 0x08;
			TCNT1H = 0x00;
			TCNT1L = 0x00;
			OCR1AH = 0x00;
			OCR1AL = 0x00;
			break;

		case E_TIMER2:
			power_timer2_enable();
			TCCR2A = 0x02;
			TCCR2B = 0x00;
			TCNT2 = 0x00;
			OCR2A = 0x00;
			break;

		default:
			break;
	} // switch

	_tdc_set_cmp_pin(_tdc_get_tdelay_timer(a_timer), 0x01);
}


void tdelay_ms(e_timer a_timer, uint32_t a_delay) {
	_tdelay_setup_ms(a_timer, a_delay);
	_timer_enable_interrupt(a_timer);
	_tdc_block(_tdc_get_tdelay_timer(a_timer));
}


void tdelay_us(e_timer a_timer, uint32_t a_delay) {
	_tdelay_setup_us(a_timer, a_delay);
	_timer_enable_interrupt(a_timer);
	_tdc_block(_tdc_get_tdelay_timer(a_timer));
}

