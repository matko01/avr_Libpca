#include "pca.h"

int main(void)
{
	uint8_t c = 0x00;

	serial_init(E_BAUD_4800);
	serial_install_interrupts(E_FLAGS_SERIAL_RX_INTERRUPT);
	serial_flush();

	while (1) {
		// this call is not blocking it will exit immediately if there is no character available
		if (!serial_getc(&c))
			continue;

		/* char received */

		// send the character back. This call is blocking - it will block until the UART is ready to send
		serial_poll_send(c);
	}
	
	return 0;
}
