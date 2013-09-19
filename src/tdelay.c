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
	_timer_init_ctc(a_timer);
	_tdc_set_cmp_pin(_tdc_get_tdelay_timer(a_timer), 0x01);

	// enable interrupts
	sei();
}


void tdelay_ms(e_timer a_timer, uint32_t a_delay) {
	_tdelay_setup_ms(a_timer, a_delay);
	_timer_en_compa_int(a_timer);
	_tdc_block(_tdc_get_tdelay_timer(a_timer));
}


void tdelay_us(e_timer a_timer, uint32_t a_delay) {
	_tdelay_setup_us(a_timer, a_delay);
	_timer_en_compa_int(a_timer);
	_tdc_block(_tdc_get_tdelay_timer(a_timer));
}

