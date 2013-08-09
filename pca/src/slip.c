#include "slip.h"
#include <util/crc16.h>
#include <string.h>

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


unsigned char slip_verify_crc16(unsigned char *a_buff, unsigned char a_buflen, unsigned char a_crcpos) {
	
	uint16_t crc_recv = 0x00;
	uint16_t crc_calcd = 0x00;

	if ((a_crcpos + 2) >= a_buflen || !a_buflen) {
		return 0;
	}

	memcpy(&crc_recv, &a_buff[a_crcpos], 2);
	memset(&a_buff[a_crcpos], 0x00, 2);

	for (uint8_t i = 0; i<a_buflen; i++) {
		crc_calcd = _crc16_update(crc_calcd, a_buff[i]);
	}

	return (crc_calcd == crc_recv ? 1 : 0);
}


unsigned char slip_append_crc16(unsigned char *a_buff, unsigned char a_datalen) {

	uint16_t crc_calcd = 0x00;
	
	memset(&a_buff[a_datalen], 0x00, 2);
	for (uint8_t i = 0; i<a_datalen; i++) {
		crc_calcd = _crc16_update(crc_calcd, a_buff[i]);
	}

	memcpy(&a_buff[a_datalen], &crc_calcd, 2);
	return a_datalen + 2;
}
