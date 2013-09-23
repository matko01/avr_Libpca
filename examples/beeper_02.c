#include "pca.h"

int main(void)
{
	// initialize timer 0 as a tone generator
	// TDELAY_IMPLEMENT_T0_INT must be set to 1 in config.h
	beeper_init(E_TIMER0);

	
	// start generating 1kHz tone for two seconds
	beeper_beep(E_TIMER0, 1000, 2000);

	/* ... */

	// stop generating tone if x == 1
	if (x == 1) {
		beeper_off(E_TIMER0);
	}
	else {
		// is x != 1 block execution until tone fades out
		beeper_block(E_TIMER0);
	}

	/* ... */

	return 0;
}
