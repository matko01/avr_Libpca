#ifndef __SLIP_H__
#define __SLIP_H__

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

#define SLIP_VERSION "0.001"

#if !defined(SLIP_CHAR_SEND) || !defined(SLIP_CHAR_RECV)

#include "serial.h"

// redefine those two in your code to use different interface
#define SLIP_CHAR_SEND(__x)	serial_sendc(__x)
#define SLIP_CHAR_RECV(__x) serial_getc(__x)

#endif

// escape markers + data END marker
#define SLIP_END 0300
#define SLIP_ESC 0333
#define SLIP_ESC_END 0334
#define SLIP_ESC_ESC 0335


/**
 * @brief receive data using slip data synchronization protocol
 *
 * @param a_buff buffer to which data will be written
 * @param a_buflen buffer size
 *
 * @return number of characters received
 */
unsigned char slip_recv(unsigned char *a_buff, unsigned char a_buflen);

/**
 * @brief send data using slip protocol
 *
 * @param a_buff buffer holding the data
 * @param a_buflen buffer size
 *
 * @return number of bytes sent
 */
unsigned char slip_send(unsigned char *a_buff, unsigned char a_buflen);

/**
 * @brief append the CRC16 checksum to the end of the data buffer
 *
 * @param a_buff buffer holding the data (it must be at least a_datalen + 2 characters big)
 * @param a_datalen length of the data in the buffer
 *
 * @return buffer size with CRC16 (a_datalen +2)
 */
unsigned char slip_append_crc16(unsigned char *a_buff, unsigned char a_datalen);

/**
 * @brief verify the CRC of the data buffer
 *
 * @param a_buff buffer holding the data
 * @param a_buflen size of the buffer
 * @param a_crcpos position in the buffer where the checksum is located
 *
 * @return 
 */
unsigned char slip_verify_crc16(unsigned char *a_buff, unsigned char a_buflen, unsigned char a_crcpos);


#endif /* __SLIP_H__ */
