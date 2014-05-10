#include "pca.h"

#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>


static volatile struct dev_d7seg_ctx g_display;


ISR(TIMER0_COMPA_vect) {
	d7seg_multiplex(&g_display);
}


int main(void)
{
	// initialize adc single shot
	adc_init(E_SINGLE_SHOT);
	adc_channel_set(0);

	adc_reference_set(E_ADC_EXTERNAL_AREF);

	// the lowest possible frequency
	adc_prescaler_set(7);

	_timer_init_ctc(E_TIMER0);
	uint32_t pocr = _timer_freq_prescale(E_TIMER0, 512, 255);
	_timer_setup_ctc(E_TIMER0, pocr);
	_timer_en_compa_int(E_TIMER0);

	// setting up the pins to which the display is connected
	g_display.segments[SEG_A].port = &PORTD;
	g_display.segments[SEG_A].pin = 4;

	g_display.segments[SEG_B].port = &PORTD;
	g_display.segments[SEG_B].pin = 2;

	g_display.segments[SEG_C].port = &PORTD;
	g_display.segments[SEG_C].pin = 5;

	g_display.segments[SEG_D].port = &PORTB;
	g_display.segments[SEG_D].pin = 1;

	g_display.segments[SEG_E].port = &PORTB;
	g_display.segments[SEG_E].pin = 0;

	g_display.segments[SEG_F].port = &PORTD;
	g_display.segments[SEG_F].pin = 6;

	g_display.segments[SEG_G].port = &PORTD;
	g_display.segments[SEG_G].pin = 3;

	g_display.segments[SEG_DOT].port = &PORTD;
	g_display.segments[SEG_DOT].pin = 7;

	// setting up the segment selection pins
	for (uint8_t i = 0; i < 4; i++) {
		g_display.sel[i].port = &PORTB;
		g_display.sel[i].pin = i + 2;
	}

	// initialize the display
	d7seg_init(&g_display);

	// put zero on each segment
	d7seg_chr_p(&g_display, 0, 0);
	d7seg_chr_p(&g_display, 1, 0);
	d7seg_chr_p(&g_display, 2, 0);
	d7seg_chr_p(&g_display, 3, 0);

	sei();

	uint16_t result = 0;
	uint16_t smooth_result = 0;
	uint8_t counter = 0;
	double tmp_result = 0.0f;
	char buff[8] = {0x00};

	while (1) {

		// trigger conversion
		adc_conversion_trigger();

		// get the result
		adc_result_poll_get(result);
		
		if (result != smooth_result) {
			counter++;
		}
		else {
			counter = 0;
		}

		if (counter > 8) {
			smooth_result = result;
		}

		// get the result
		tmp_result = tmp36_raw_to_celsius(smooth_result, 1024, 3300);

		if (tmp_result < 0x00) {
			// indicate a negative temperature
			snprintf(buff, 8, "%+2.1f", tmp_result);
			d7seg_chr_p(&g_display, 0, CHAR_MINUS);
			d7seg_chr_p(&g_display, 1, buff[1] - '0');
			d7seg_chr_dot_p(&g_display, 2, buff[2] - '0');
		}
		else {
			snprintf(buff, 8, "%2.2f", tmp_result);
			d7seg_chr_p(&g_display, 0, buff[0] - '0');

			// print the digit and enable the dot as well
			d7seg_chr_dot_p(&g_display, 1, buff[1] - '0');
			d7seg_chr_p(&g_display, 2, buff[3] - '0');
		}

		d7seg_chr_p(&g_display, 3, buff[4] - '0');		
		_delay_ms(5);
	}

	return 0;
}
