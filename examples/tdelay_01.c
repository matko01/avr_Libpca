#include "pca.h"

int main(void)
{	
	uint32_t delay;

	// initialize timer 1 for delay purposes
	// in addition the timer 1 interrupt implementation must be enabled (TDELAY_IMPLEMENT_T1_INT) in config.h
	tdelay_init(E_TIMER1);

	/* ... */

	// hold execution for 200 ms
	tdelay_ms(E_TIMER1, 200);

	/* ... */

	delay = 300;

	while (1) {
		/* ... */

		// hold execution for 300 us
		tdelay_us(E_TIMER1, delay);
	}

	return 0;
}
