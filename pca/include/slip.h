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


#define SLIP_END 0300
#define SLIP_ESC 0333
#define SLIP_ESC_END 0334
#define SLIP_ESC_ESC 0335

unsigned char slip_recv(unsigned char *a_buff, unsigned char a_buflen);
unsigned char slip_send(unsigned char *a_buff, unsigned char a_buflen);

#endif /* __SLIP_H__ */
