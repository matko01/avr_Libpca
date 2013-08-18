#include "beeper.h"
#include <avr/io.h>


void beeper_init(e_timer a_timer) {
	tdelay_init(a_timer);

	switch (a_timer) {

#if TDELAY_IMPLEMENT_T0_INT == 1
		case E_TIMER0:
			DDRD |= _BV(PORTD6);
			TCCR0A |= 0x40;
			break;
#endif

#if TDELAY_IMPLEMENT_T1_INT == 1
		case E_TIMER1:
			DDRB |= _BV(PORTB1);
			TCCR1A |= 0x40;
			break;
#endif

#if TDELAY_IMPLEMENT_T2_INT == 1
		case E_TIMER2:
			DDRB |= _BV(PORTB3);
			TCCR2A |= 0x40;
			break;
#endif

		default:
			break;
	} // switch

	_tdc_set_cmp_pin(_tdc_get_tdelay_timer(a_timer), 0x00);
}


void beeper_beep(e_timer a_timer, 
	uint32_t freq, 
	uint32_t duration) {
	
	beeper_off(a_timer);
	_tdc_setup_delay(_tdc_get_tdelay_timer(a_timer), freq, duration);

	if (freq) {
		switch(a_timer) {

#if TDELAY_IMPLEMENT_T0_INT == 1
			case E_TIMER0:
				TCCR0A |= _BV(COM0A0);
				break;
#endif

#if TDELAY_IMPLEMENT_T1_INT == 1
			case E_TIMER1:
				TCCR1A |= _BV(COM1A0);
				break;
#endif

#if TDELAY_IMPLEMENT_T2_INT == 1
			case E_TIMER2:
				TCCR2A |= _BV(COM2A0);
				break;
#endif
			default:
				break;
		} // switch
	}	

	_timer_enable_interrupt(a_timer);
}

void beeper_off(e_timer a_timer) {
	switch (a_timer) {

#if TDELAY_IMPLEMENT_T0_INT == 1
		case E_TIMER0:
			_timer_disable_interrupt(a_timer);
			TCCR0B &= 0xf8;
			TCCR0A &= ~_BV(COM0A0);
			break;
#endif

#if TDELAY_IMPLEMENT_T1_INT == 1
		case E_TIMER1:
			_timer_disable_interrupt(a_timer);
			TCCR1B &= 0xf8;
			TCCR1A &= ~_BV(COM1A0);
			break;
#endif

#if TDELAY_IMPLEMENT_T2_INT == 1
		case E_TIMER2:
			_timer_disable_interrupt(a_timer);
			TCCR2B &= 0xf8;
			TCCR2A &= ~_BV(COM2A0);
			break;
#endif

		default:
			break;
	} // switch
	_tdc_set_duration(_tdc_get_tdelay_timer(a_timer), 0x00);
}

inline void beeper_block(e_timer a_timer) {
	_tdc_block(_tdc_get_tdelay_timer(a_timer));
}
