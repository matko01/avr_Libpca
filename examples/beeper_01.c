#include "pca.h"

int main(void)
{
	// initialize timer 0 as a tone generator
	// TDELAY_IMPLEMENT_T0_INT must be set to 1 in config.h
	beeper_init(E_TIMER0);

	for (uint8_t i = 0; i < 10; i++) {
		// start generating tone with timer 0, of frequency (440 + (110*i)),
		// lasting 300 ms
		beeper_beep(E_TIMER0, 440 + (100*i), 300);

		// wait until tone generation is finished
		beeper_block(E_TIMER0);
	}

	return 0;
}
