#include "tdelay_common.h"

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/* ================================================================================ */

#if TDELAY_IMPLEMENT_T0_INT == 1 || TDELAY_IMPLEMENT_T1_INT == 1 || TDELAY_IMPLEMENT_T2_INT == 1
static volatile struct {
	volatile uint32_t duration;
	volatile uint8_t reset_cmpa_pin;
} g_tc[E_TIMER_LAST];


/**
 * @brief interrupt context structure
 */
struct regs {
	volatile e_timer timer;
	volatile uint8_t *timsk;
	volatile uint8_t *tccrb;
	volatile uint8_t *port;
	volatile uint8_t tccval;
	volatile uint8_t pin;
};

/* ================================================================================ */

static void _isr_tdelay_handler(volatile struct regs *sregs) {

	if (!g_tc[sregs->timer].duration) {
		// MASK the interrupt + disable the clock;		
		*(sregs->timsk) &= ~_BV(1); // OCIEXA
		*(sregs->tccrb) = sregs->tccval;

		if (g_tc[sregs->timer].reset_cmpa_pin)
			*(sregs->port) &= ~_BV(sregs->pin);
	}
	else {
		g_tc[sregs->timer].duration--;
	}
}

#endif

static uint8_t __tdc_block(e_timer a_timer, volatile uint8_t *timsk, uint8_t map) {
	if (bit_is_clear((*timsk), map)) {				
		_tdc_set_duration(a_timer, 0x00);
		return 0;
	}
	return 1;
}

/* ================================================================================ */

#if TDELAY_IMPLEMENT_T2_INT == 1
/**
 * @brief timer 2
 *
 * @param TIMER2_COMPA_vect
 */
ISR(TIMER2_COMPA_vect, ISR_NOBLOCK) {
	volatile struct regs tr2 = { E_TIMER2, &TIMSK2, &TCCR2B, &PORTB, 0x00, PORTB3 };
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
	volatile struct regs tr1 = { E_TIMER1, &TIMSK1, &TCCR1B, &PORTB, 0x08, PORTB1 };
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
	volatile struct regs tr0 = { E_TIMER0, &TIMSK0, &TCCR0B, &PORTD, 0x00, PORTD6 };
	_isr_tdelay_handler( &tr0 );
}

#endif

/* ================================================================================ */


void _tdc_set_duration(e_timer a_tim, uint32_t a_dur) {
	g_tc[a_tim].duration = a_dur;
}


void _tdc_set_cmp_pin(e_timer a_tim, uint8_t a_pin) {
	g_tc[a_tim].reset_cmpa_pin = a_pin;
}


void _tdc_setup_ms(e_timer a_timer, uint32_t a_delay) {

	// which pin
	switch(a_timer) {

#if TDELAY_IMPLEMENT_T0_INT == 1
		case E_TIMER0:
			// interrupt every 1ms
			TCCR0A &= 0x0f;
			TCCR0B = 0x03;			
			TCNT0 = 0x00;
			OCR0A = 250;
			break;
#endif

#if TDELAY_IMPLEMENT_T1_INT == 1
		case E_TIMER1:
			TCCR1A = 0x00;
			TCCR1B = 0x0b;
			// setup delay counter
			TCNT1L = 0x00;
			TCNT1H = 0x00;
			OCR1AH = 0x00;
			OCR1AL = 250;
			break;
#endif

#if TDELAY_IMPLEMENT_T2_INT == 1
		case E_TIMER2:
			// interrupt every 1ms
			TCCR2A &= 0x0f;
			TCCR2B = 0x03;			
			TCNT2 = 0x00;
			OCR2A = 250;
			break;
#endif

		default:
			return;
			break;
	} // switch

	_tdc_set_duration(a_timer, a_delay);
}


void _tdc_enable_interrupt(e_timer a_timer) {
	// which pin
	switch(a_timer) {

#if TDELAY_IMPLEMENT_T0_INT == 1
		case E_TIMER0:
			TIMSK0 |= _BV(OCIE0A);
			break;
#endif

#if TDELAY_IMPLEMENT_T1_INT == 1
		case E_TIMER1:
			TIMSK1 |= _BV(OCIE1A);
			break;
#endif

#if TDELAY_IMPLEMENT_T2_INT == 1
		case E_TIMER2:
			TIMSK2 |= _BV(OCIE2A);
			break;
#endif

		default:
			return;
			break;
	} // switch
}


void _tdc_block(e_timer a_timer) {
	unsigned char wait = 0;

	// deadlock protection
	switch (a_timer) {

#if TDELAY_IMPLEMENT_T0_INT == 1
		case E_TIMER0:
			wait = __tdc_block(a_timer, &TIMSK0, OCIE0A);
			break;
#endif

#if TDELAY_IMPLEMENT_T1_INT == 1
		case E_TIMER1:
			wait = __tdc_block(a_timer, &TIMSK1, OCIE1A);
			break;
#endif

#if TDELAY_IMPLEMENT_T2_INT == 1
		case E_TIMER2:
			wait = __tdc_block(a_timer, &TIMSK2, OCIE2A);
			break;
#endif

		default:
			break;
	} // switch

	if (wait) while (g_tc[a_timer].duration);
}
