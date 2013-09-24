#include "pca.h"

int main(void)
{
	uint8_t x = 0;
	tpwm_pwm_init(E_TIMER1);

	// configure the timer so we have 8 bit PWM resolution available
	// (the duty cycle can be adjusted in range 0 - 255)
	// this function will try to establish highest possible frequency
	// for this bit resolution (62.5k for single slope PWM)
	tpwm_setup_for_bitres(E_TIMER1, 8);

	// generate saw-tooth analog wave
	while (1) {
		OCR1AH = 0x00;
		OCR1AL = x++;
	}

	return 0;
}
