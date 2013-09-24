#include "pca.h"
#include <stdio.h>


int main(void)
{
	uint8_t i = 0;

	// initialize UART at 4800 bps
	serial_init(E_BAUD_4800);

	// install rx interrupt (not needed really since we do not collect any data)
	serial_install_interrupts(E_FLAGS_SERIAL_RX_INTERRUPT);

	// flush any garbage in the buffers
	serial_flush();

	// install serial STDIO
	serial_install_stdio();

	// initialize timer 0 for delay purposes
	tdelay_init(E_TIMER0);

	// every one second print a message to serial console
	while(1) {
		printf("STDIO Serial Hello. i: [%d]\n", i++);
		tdelay_ms(E_TIMER0, 1000);
	}

	return 0;
}
