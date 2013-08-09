#include "slip.h"

unsigned char slip_recv(unsigned char *a_buff, unsigned char a_buflen) {
	
	unsigned char c = 0x00;
	unsigned int recv = 0x00;
	unsigned char mode = 0x00;

	// collect a full slip packet
	while (1) {
		if (!SLIP_CHAR_RECV(&c))
			continue;

		if (mode) {
			a_buff[recv++] = 
				(c == SLIP_ESC_END ? SLIP_END : (c == SLIP_ESC_ESC ? SLIP_ESC : c));
			mode = 0;
		}
		else {
			switch(c) {
				case SLIP_END:
					if (recv) return recv;
					break;

				case SLIP_ESC:
					mode = 0x01;
					break;

				default:
					a_buff[recv++] = c;
					break;

			} // switch
		} // if

		if (recv >= a_buflen) {
			return recv;
		}
	} // while

	return 0;
}

unsigned char slip_send(unsigned char *a_buff, unsigned char a_buflen) {

	SLIP_CHAR_SEND(SLIP_END);
	unsigned char send = 0;

	while(send < a_buflen) {

		switch (*a_buff) {

			case SLIP_END:
				SLIP_CHAR_SEND(SLIP_ESC);
				SLIP_CHAR_SEND(SLIP_ESC_END);
				break;

			case SLIP_ESC:
				SLIP_CHAR_SEND(SLIP_ESC);
				SLIP_CHAR_SEND(SLIP_ESC_ESC);
				break;

			default:
				SLIP_CHAR_SEND(*a_buff);
				break;
		} // switch

		a_buff++;
		send++;
	} // while

	return send;
}
