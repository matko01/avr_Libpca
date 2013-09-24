#include "pca.h"


int main(void)
{
	uint8_t c = 0x00;

	serial_init(E_BAUD_4800);

	while (1) {

		// this call is blocking it will until a character is received
		serial_poll_getc(&c);

		/* char received */

		// send the character back. This call is blocking - it will block until the UART is ready to send
		serial_poll_send(c);
	}
	
	return 0;
}
