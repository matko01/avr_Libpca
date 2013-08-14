#include "beeper.h"

#include <avr/io.h>

/* ================================================================================ */

/**
 * @brief helper function to calculate the presclaer and OCR
 *
 * @param ocr
 * @param prescaler
 * @param a_freq
 * @param criterion
 */
static void _find_ocr_prescaler(uint32_t *ocr, uint8_t *prescaler, uint32_t a_freq, uint32_t criterion) {
	uint8_t prescalers[] = { 0, 3, 6, 8, 10 };
	uint8_t pnum = sizeof(prescalers);
	*prescaler = 0;

	do {
		*ocr = F_CPU / ((a_freq << 1) * (0x01 << prescalers[*prescaler]));
	} while ((*ocr > criterion) && (++(*prescaler) < pnum));
}

/**
 * @brief calculate prescaler and OCR value for 8 bit counters
 *
 * @param a_freq requested frequency
 * @param a_ocr ocr value returned by reference
 * @param a_prescaler prescaler value returned by reference
 */
static void _timer8_prescaler(uint32_t a_freq, 
		uint8_t *a_ocr, 
		uint8_t *a_prescaler) {

	uint32_t ocr = 0x00;

	// smallest achievable frequency with this timer
	if (a_freq < 62) {
		*a_prescaler = 5;
		*a_ocr = 255;
		return;
	}

	_find_ocr_prescaler(&ocr, a_prescaler, a_freq, 255);
	*a_ocr = (ocr & 0xff);	
}

/**
 * @brief calculate prescaler and ocr values for 16 bit counter
 *
 * @param a_freq requested frequency
 * @param a_ocrh ocr high
 * @param a_ocrl ocr low
 * @param a_prescaler prescaler value
 */
static void _timer16_prescaler(uint32_t a_freq,
		uint8_t *a_ocrh,
		uint8_t *a_ocrl,
		uint8_t *a_prescaler) {

	uint32_t ocr = 0x00;

	// smallest achievable frequency with this timer
	if (a_freq < 62) {
		*a_prescaler = 5;
		*a_ocrh = 0x00;
		*a_ocrl = 255;
		return;
	}

	// highest frequency we can generate
	if (a_freq > 18000) {
		*a_prescaler = 2;
		*a_ocrh = 0x00;
		*a_ocrl = 55;
		return;
	}

		_find_ocr_prescaler(&ocr, a_prescaler, a_freq, 512);
	*a_ocrh = (ocr & 0xff00) >> 8;

	*a_ocrl = (ocr & 0xff);
}

/* ================================================================================ */

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
			TCCR1B |= 0x08;
			break;
#endif

#if TDELAY_IMPLEMENT_T2_INT == 1
		case E_TIMER2:
			DDRB |= _BV(PORTB3);
			TCCR2A |= 0x40;
			break;
#endif

		default:
			return;
			break;
	} // switch

	_tdc_set_cmp_pin(a_timer, 0x01);
}

void beeper_beep(e_timer a_timer, 
	uint32_t freq, 
	uint32_t duration) {
	
	uint8_t ocr = 0x00;
	uint8_t ocrh = 0x00;
	uint8_t presc = 0x00;

	beeper_off(a_timer);
	_tdc_setup_ms(a_timer, duration);

	if (freq) {

		switch(a_timer) {

#if TDELAY_IMPLEMENT_T0_INT == 1
			case E_TIMER0:
				TCCR0A |= 0x40;
				_timer8_prescaler(freq, &ocr, &presc);
				TCCR0B = 0x00 | (presc & 0x07);
				OCR0A = ocr;
				freq = freq < 62 ? 62 : freq;
				break;
#endif

#if TDELAY_IMPLEMENT_T1_INT == 1
			case E_TIMER1:
				_timer16_prescaler(freq, &ocrh, &ocr, &presc);

				OCR1AH = ocrh;
				OCR1AL = ocr;
				TCCR1A |= 0x40;
				TCCR1B = 0x08 | (presc & 0x07);
				break;
#endif

#if TDELAY_IMPLEMENT_T2_INT == 1
			case E_TIMER2:
				_timer8_prescaler(freq, &ocr, &presc);
				OCR2A = ocr;
				TCCR2A |= 0x40;
				TCCR2B = 0x00 | (presc & 0x07);
				freq = freq < 62 ? 62 : freq;
				break;
#endif

			default:
				return;
				break;
		} // switch
		// settings correction
		_tdc_set_duration(a_timer, (uint32_t)((freq*duration) / 500));
	}	

	_tdc_enable_interrupt(a_timer);
}

void beeper_off(e_timer a_timer) {
	switch (a_timer) {

#if TDELAY_IMPLEMENT_T0_INT == 1
		case E_TIMER0:
			TIMSK0 &= ~_BV(OCIE0A);
			TCCR0B = 0x00;
			break;
#endif

#if TDELAY_IMPLEMENT_T1_INT == 1
		case E_TIMER1:
			TIMSK1 &= ~_BV(OCIE1A);
			TCCR1B = 0x08;
			break;
#endif

#if TDELAY_IMPLEMENT_T2_INT == 1
		case E_TIMER2:
			TIMSK2 &= ~_BV(OCIE2A);
			TCCR2B = 0x00;
			break;
#endif

		default:
			break;
	} // switch
	_tdc_set_duration(a_timer, 0x00);
}

void beeper_block(e_timer a_timer) {
	_tdc_block(a_timer);
}
