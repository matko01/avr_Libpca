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


/**
 * @file slip.h 
 *
 * @brief API to implement a SLIP protocol over serial line. 
 *
 * The reason behind this is to synchronize the receiver and transmitter, so receiver does not loose
 *  any data send by the transmitter (start to listen a little after transmitter just sent the data) or doesn't receive any junk before the transmitter
 *  started to send the data itself. For more information about SLIP protocol please refer to RFC 1055 http://www.ietf.org/rfc/rfc1055.txt
 *
 * @example serial_04.c
 *
 * Using serial port along with SLIP protocol for binary data IO synchronization
 */

#include "config.h"
#include "common.h"

/**
 * @brief module version definition (just for internal use, to determine API incompatibilities)
 */
#define SLIP_VERSION "0.001"


/**
 * @brief if there are no SEND/RECV routine definitions already existing
 *  use the default definitions provided bellow
 */
#if !defined(SLIP_CHAR_SEND) || !defined(SLIP_CHAR_RECV)

#include "serial.h"

/// redefine it, in your code to use different implementation of SEND routine
#define SLIP_CHAR_SEND(__x)	serial_poll_sendc(__x)

/// redefine it, in your code to use different implementation of RECV routine
#define SLIP_CHAR_RECV(__x) serial_getc(__x)

#endif


// ============================== SLIP specific characters ==============================

/// transmission END marker - SLIP data block should be ended with this marker
#define SLIP_END 0300

/// SLIP escape character, this one will be placed just before a byte if it's value is either 0333 (0xdb) or 0300 (c0)
#define SLIP_ESC 0333

/// this byte will be placed in the data stream instead of the SLIP_END byte, preceded by a SLIP_ESC byte
#define SLIP_ESC_END 0334

/// this byte will be placed in the data stream instead of the SLIP_ESC byte, preceded by a SLIP_ESC byte
#define SLIP_ESC_ESC 0335

// ============================== SLIP specific characters ==============================


/**
 * @brief receive data using slip data synchronization protocol
 *
 * @param a_buff buffer to which data will be written
 * @param a_buflen buffer size
 *
 * @return number of characters received
 */
uint8_t slip_recv(uint8_t *a_buff, uint8_t a_buflen);


/**
 * @brief send data using slip protocol
 *
 * @param a_buff buffer holding the data
 * @param a_buflen buffer size
 *
 * @return number of bytes sent
 */
uint8_t slip_send(uint8_t *a_buff, uint8_t a_buflen);

#if SLIP_IMPLEMENT_CRC16 == 1
/**
 * @brief append the CRC16 checksum to the end of the data buffer
 *
 * @param a_buff buffer holding the data (it must be at least a_datalen + 2 characters big)
 * @param a_datalen length of the data in the buffer
 *
 * @return buffer size with CRC16 (a_datalen +2)
 */
uint8_t slip_append_crc16(uint8_t *a_buff, uint8_t a_datalen);


/**
 * @brief prepend the CRC16 checksum to the beginning of the data buffer
 *
 * @param a_buff buffer holding the data (it must have an uint16_t variable as first field - for the crc, otherwise the data inside will be corrupted)
 * @param a_datalen length of the data in the buffer
 *
 * @return buffer size
 */
uint8_t slip_prepend_crc16(uint8_t *a_buff, uint8_t a_datalen);


/**
 * @brief verify the CRC of the data buffer
 *
 * @param a_buff buffer holding the data
 * @param a_buflen size of the buffer
 * @param a_crcpos position in the buffer where the checksum is located. The CRC may reside at the end of the buffer (buflen - 2) or at the beginning (0)
 *
 * @return crc if the checksum verification went successful or 0 if verification failed
 */
uint8_t slip_verify_crc16(uint8_t *a_buff, uint8_t a_buflen, uint8_t a_crcpos);
#endif


#endif /* __SLIP_H__ */
