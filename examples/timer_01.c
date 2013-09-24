#include "pca.h"

static vuint32_t x = 0;

/**
 * in order for that to work the interrupt implementation of TIMER0 interrupt
 * should be disabled (TDELAY_IMPLEMENT_T0_INT = 0)
 */
ISR(TIMER0_COMPA_vect) {
	x++;
}

int main(int argc, char const *argv[])
{
	// configure timer to overflow every 1/100 = 10 ms
	uint32_t pocr = _timer_freq_prescale(E_TIMER0, 100, 255);

	_timer_init_ctc(E_TIMER0);
	_timer_setup_ctc(E_TIMER0, pocr);

	// enable interrupt
	_timer_en_compa_int(E_TIMER0);

	while (1);
	return 0;
}
