/* Copyright (C) 
 * 2013 - Tomasz Wisniewski
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 */


/**
 * @file slip.c
 *
 * @brief Implementation of SLIP Send/Recv routines
 *
 */

#include <slip.h>

#if SLIP_IMPLEMENT_CRC16 == 1
#include <util/crc16.h>
#endif

#include <string.h>
#include <stdint.h>

/* ================================================================================ */

#if SLIP_IMPLEMENT_CRC16 == 1
/**
 * @brief universal implementation calculating the crc of the data in the buffer
 *
 * The result is either appended to the end of the buffer or inserted in the very begining
 *
 * @param a_buff buffer of which the sum should be calculated
 * @param a_datalen datalenght of the buffer
 * @param a_ap whether to append (1) or prepend (0)
 */
static uint8_t _slip_insert_crc16(uint8_t *a_buff, uint8_t a_datalen, uint8_t a_ap);
#endif
	
/* ================================================================================ */


uint8_t slip_recv(uint8_t *a_buff, uint8_t a_buflen) {
	
	uint8_t c = 0x00;
	uint16_t recv = 0x00;

	// inverted logic since negative conditional produces less code
	uint8_t escape = 0xff;

	// collect a full SLIP packet
	while (1) {

		if (!SLIP_CHAR_RECV(&c)) {
			continue;
		}

		// no escape character detected
		switch(c) {
			case SLIP_END:
				if (recv) return recv;
				break;

			case SLIP_ESC:
				// escape character detected set 'escape' variable
				escape = 0x00;
				break;

			default:
				// translate escaped character to it's original value
				if (!escape) {
					c = (c == SLIP_ESC_END ? SLIP_END : 
							(c == SLIP_ESC_ESC ? SLIP_ESC : c));
					escape = 0xff;
				}

				if (recv < a_buflen) {
					a_buff[recv++] = c;
				}
				break;
		} // switch
	} // while

	return 0;
}


uint8_t slip_send(uint8_t *a_buff, uint8_t a_buflen) {

	uint8_t send = a_buflen;

	// flush buffers at the receiving side
	SLIP_CHAR_SEND(SLIP_END);

	// transmit the data
	while (a_buflen) {
		switch (*a_buff) {
			// escape END character
			case SLIP_END:
				SLIP_CHAR_SEND(SLIP_ESC);
				SLIP_CHAR_SEND(SLIP_ESC_END);
				break;

			// escape ESC character
			case SLIP_ESC:
				SLIP_CHAR_SEND(SLIP_ESC);
				SLIP_CHAR_SEND(SLIP_ESC_ESC);
				break;

			default:
				SLIP_CHAR_SEND(*a_buff);
				break;
		} // switch

		a_buff++;
		a_buflen--;
	}

	// mark the transmission end
	SLIP_CHAR_SEND(SLIP_END);
	return send;
}


#if SLIP_IMPLEMENT_CRC16 == 1
uint8_t slip_verify_crc16(uint8_t *a_buff, uint8_t a_buflen, uint8_t a_crcpos) {
	
	uint16_t crc_recv = 0x00;
	uint16_t crc_calcd = 0x00;

	if ((a_crcpos + 2) > a_buflen || !a_buflen) {
		return 0;
	}

	// the transmitter calculated the CRC the same way
	// marking the memory being a CRC placeholder with zeroes
	// and then copying the CRC inside
	memcpy(&crc_recv, &a_buff[a_crcpos], 2);
	memset(&a_buff[a_crcpos], 0x00, 2);

	while (a_buflen) {
		crc_calcd = _crc16_update(crc_calcd, *a_buff);
		a_buff++;
		a_buflen--;
	}

	return (crc_calcd == crc_recv ? crc_calcd : 0);
}


uint8_t slip_append_crc16(uint8_t *a_buff, uint8_t a_datalen) {
	return _slip_insert_crc16(a_buff, a_datalen, 1);
}


uint8_t slip_prepend_crc16(uint8_t *a_buff, uint8_t a_datalen) {
	return _slip_insert_crc16(a_buff, a_datalen, 0);
}



/* ================================================================================ */


static uint8_t _slip_insert_crc16(uint8_t *a_buff, uint8_t a_datalen, uint8_t a_ap) {

	uint16_t crc_calcd = 0x00;

	uint8_t crc_pos = (a_ap == 1 ? a_datalen : 0);
	uint8_t crc_offs = (a_ap == 1 ? 2 : 0);
	
	memset(&a_buff[crc_pos], 0x00, 2);
	for (uint8_t i = 0; i<(a_datalen + crc_offs); i++) {
		crc_calcd = _crc16_update(crc_calcd, a_buff[i]);
	}

	memcpy(&a_buff[crc_pos], &crc_calcd, 2);
	return (a_datalen + crc_offs);
}


/* ================================================================================ */

#endif
