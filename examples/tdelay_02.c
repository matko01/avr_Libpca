#include "pca.h"

int main(void)
{	
	uint32_t delay = 300;

	// initialize timer 1 for delay purposes
	// in addition the timer 1 interrupt implementation must be enabled (TDELAY_IMPLEMENT_T1_INT) in config.h
	tdelay_init(E_TIMER1);

	/* ... */

	// hold execution for 200 ms
	tdelay_ms(E_TIMER1, delay);

	/* ... */

	// reconfigure timer1 as a tone generator
	beeper_init(E_TIMER1);

	// generate 1kHz beep
	beeper_beep(E_TIMER0, 1000, delay);

	// wait until tone generation is finished
	beeper_block(E_TIMER0);

	// reconfigure timer1 again only for delay purposes
	// there will be no tone on OCXA pin
	tdelay_init(E_TIMER1);

	/* ... */

	return 0;
}
