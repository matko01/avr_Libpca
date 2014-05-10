#ifndef OW_NETWORK_H_0MYSTYZB
#define OW_NETWORK_H_0MYSTYZB

/* Copyright (C) 
 * 2014 - Tomasz Wisniewski
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


/// replace with other Link Layer implementation
#include <soft_onewire.h>
#include <stdint.h>

#warning NOT IMPLEMENTED YET

/**
 * @brief one wire device rom code identifier
 */
typedef uint64_t ow_romcode_t;


/**
 * @brief Scan one wire bus for device rom identifiers
 *
 * @param a_romcodes 
 *
 * @return 
 */
uint16_t ow_net_search_rom(struct soft_ow *a_bus, ow_romcode_t *a_romcodes);



#endif /* end of include guard: OW_NETWORK_H_0MYSTYZB */

