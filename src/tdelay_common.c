#include "tdelay_common.h"

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/* ================================================================================ */

#if TDELAY_IMPLEMENT_T0_INT == 1 || TDELAY_IMPLEMENT_T1_INT == 1 || TDELAY_IMPLEMENT_T2_INT == 1
static volatile struct {
	volatile uint32_t duration;
	volatile uint8_t reset_cmpa_pin;
} g_tc[E_TDELAY_TIMER_LAST];


/**
 * @brief interrupt context structure
 */
struct regs {
	volatile e_timer timer;
	volatile uint8_t *timsk;
	volatile uint8_t *tccrb;
	volatile uint8_t *port;
	volatile uint8_t pin;
};

/* ================================================================================ */

static void _isr_tdelay_handler(volatile struct regs *sregs) {

	if (!g_tc[sregs->timer].duration) {
		// MASK the interrupt + disable the clock;		
		*(sregs->timsk) &= ~_BV(1); // OCIEXA
		*(sregs->tccrb) &= 0xf8;

		if (!g_tc[sregs->timer].reset_cmpa_pin)
			*(sregs->port) &= ~_BV(sregs->pin);
	}
	else {
		g_tc[sregs->timer].duration--;
	}
}

static uint8_t __tdc_block(e_timer a_timer, volatile uint8_t *timsk, uint8_t map) {
	if (bit_is_clear((*timsk), map)) {				
		_tdc_set_duration(a_timer, 0x00);
		return 0;
	}
	return 1;
}

#endif

/* ================================================================================ */

#if TDELAY_IMPLEMENT_T2_INT == 1
/**
 * @brief timer 2
 *
 * @param TIMER2_COMPA_vect
 */
ISR(TIMER2_COMPA_vect, ISR_NOBLOCK) {
	volatile struct regs tr2 = { E_TDELAY_TIMER2, &TIMSK2, &TCCR2B, &PORTB, PORTB3 };
	_isr_tdelay_handler( &tr2 );
}

#endif


#if TDELAY_IMPLEMENT_T1_INT == 1
/**
 * @brief timer 1
 *
 * @param TIMER1_COMPA_vect
 */
ISR(TIMER1_COMPA_vect, ISR_NOBLOCK) {
	volatile struct regs tr1 = { E_TDELAY_TIMER1, &TIMSK1, &TCCR1B, &PORTB, PORTB1 };
	_isr_tdelay_handler( &tr1 );
}

#endif


#if TDELAY_IMPLEMENT_T0_INT == 1
/**
 * @brief timer 0
 *
 * @param TIMER0_COMPA_vect
 */
ISR(TIMER0_COMPA_vect, ISR_NOBLOCK) {
	volatile struct regs tr0 = { E_TDELAY_TIMER0, &TIMSK0, &TCCR0B, &PORTD, PORTD6 };
	_isr_tdelay_handler( &tr0 );
}

#endif

/* ================================================================================ */


inline void _tdc_set_duration(e_tdelay_timer a_tim, uint32_t a_dur) {
#if TDELAY_IMPLEMENT_T0_INT == 1 || TDELAY_IMPLEMENT_T1_INT == 1 || TDELAY_IMPLEMENT_T2_INT == 1
	g_tc[a_tim].duration = a_dur;
#endif
}


inline void _tdc_set_cmp_pin(e_tdelay_timer a_tim, uint8_t a_pin) {
#if TDELAY_IMPLEMENT_T0_INT == 1 || TDELAY_IMPLEMENT_T1_INT == 1 || TDELAY_IMPLEMENT_T2_INT == 1
	g_tc[a_tim].reset_cmpa_pin = a_pin;
#endif
}


e_timer _tdc_get_timer(e_tdelay_timer a_timer) {
	switch (a_timer) {
#if TDELAY_IMPLEMENT_T0_INT == 1
		case E_TDELAY_TIMER0:
			return E_TIMER0;
			break;
#endif

#if TDELAY_IMPLEMENT_T1_INT == 1
		case E_TDELAY_TIMER1:
			return E_TIMER1;
			break;
#endif

#if TDELAY_IMPLEMENT_T2_INT == 1
		case E_TDELAY_TIMER2:
			return E_TIMER2;
			break;
#endif

		default:
			break;
	}

	return E_TIMER_LAST;
}


e_tdelay_timer _tdc_get_tdelay_timer(e_timer a_timer) {
	switch (a_timer) {
#if TDELAY_IMPLEMENT_T0_INT == 1
		case E_TIMER0:
			return E_TDELAY_TIMER0;
			break;
#endif

#if TDELAY_IMPLEMENT_T1_INT == 1
		case E_TIMER1:
			return E_TDELAY_TIMER1;
			break;
#endif

#if TDELAY_IMPLEMENT_T2_INT == 1
		case E_TIMER2:
			return E_TDELAY_TIMER2;
			break;
#endif

		default:
			break;
	}

	return E_TDELAY_TIMER_LAST;
}


void _tdc_setup_delay(e_tdelay_timer a_timer, uint32_t a_freq, uint32_t a_delay) {

	uint32_t pocr = 0x00;
	
#if TDELAY_IMPLEMENT_T1_INT == 1
	if (E_TDELAY_TIMER1 == a_timer) {
		pocr = _timer_freq_prescale(E_TIMER1, a_freq, 260);
	}
	else 
#endif	
	{
		pocr = _timer_freq_prescale(a_timer, a_freq, 255);
	}

	_timer_setup_ctc(_tdc_get_timer(a_timer), pocr);
	_tdc_set_duration(a_timer, (uint32_t)((a_freq*a_delay)/500));
}


void _tdc_block(e_tdelay_timer a_timer) {
	unsigned char wait UNUSED = 0;

	// deadlock protection
	switch (a_timer) {
#if TDELAY_IMPLEMENT_T0_INT == 1
		case E_TDELAY_TIMER0:
			wait = __tdc_block(a_timer, &TIMSK0, OCIE0A);
			break;
#endif

#if TDELAY_IMPLEMENT_T1_INT == 1
		case E_TDELAY_TIMER1:
			wait = __tdc_block(a_timer, &TIMSK1, OCIE1A);
			break;
#endif

#if TDELAY_IMPLEMENT_T2_INT == 1
		case E_TDELAY_TIMER2:
			wait = __tdc_block(a_timer, &TIMSK2, OCIE2A);
			break;
#endif

		default:
			break;
	} // switch

#if TDELAY_IMPLEMENT_T0_INT == 1 || TDELAY_IMPLEMENT_T1_INT == 1 || TDELAY_IMPLEMENT_T2_INT == 1
	if (wait) while (g_tc[a_timer].duration);
#endif
}

