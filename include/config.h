#ifndef CONFIG_H_VPGOK90D
#define CONFIG_H_VPGOK90D

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

// ============================ SERIAL PORT =======================================

/// change to 0 and rebuild if you do not need stats information
#define SERIAL_COLLECT_STATS 1

/// change to 0 and rebuild if you do not with to include serial rx interrupt
/// some functionality will be missing
#define SERIAL_IMPLEMENT_RX_INT 1

/// change to 0 and rebuild if you do not with to include serial tx interrupt
/// some functionality will be missing
#define SERIAL_IMPLEMENT_TX_INT 0

/// 1 - STDIN data will be received by polling, 0 - interrupts
#define SERIAL_STDIN_POLL 0

/// 1 - STDOUT data will be sent by polling, 0 - interrupts
#define SERIAL_STDOUT_POLL 1

/// RX ring buffer size if SERIAL_IMPLEMENT_RX_INT == 1
#define SERIAL_RX_RING_SIZE 32

/// TX ring buffer size if SERIAL_IMPLEMENT_TX_INT == 1
#define SERIAL_TX_RING_SIZE 32

// ============================ TIMERS/BEEPER =====================================

/// change to 0 if to disable timer 0 delay and beeper interrupts and routines
#define TDELAY_IMPLEMENT_T0_INT 1

/// change to 0 if to disable timer 1 delay and beeper interrupts and routines
#define TDELAY_IMPLEMENT_T1_INT 0

/// change to 0 if to disable timer 2 delay and beeper interrupts and routines
#define TDELAY_IMPLEMENT_T2_INT 0

// ================================================================================


#endif /* end of include guard: CONFIG_H_VPGOK90D */

