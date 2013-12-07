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

#include <dev_ds18b20.h>


void ds18b20_start_conversion(volatile struct soft_ow *a_bus, ow_romcode_t *a_romcode) {
	sow_reset(a_bus);

	if (NULL == a_romcode || 
			OW_SINGLEDROP == SOFT_OW_TOPOLOGY_GET(a_bus->conf)) {

		// start conversion
		sow_write_byte(a_bus, OWN_SKIP_ROM);
		sow_write_byte(a_bus, DS18B20_CMD_CONVERT_T);
		
		// parasite powered sensors requires strong pull-up
		// during the conversion
		if (OW_POWER_PARASITE == SOFT_OW_POWER_MODE_GET(a_bus->conf)) {
			sow_strong_pullup(a_bus, 1);
		}
	}
	
}


void ds18b20_write_rom(volatile struct soft_ow *a_bus, ow_romcode_t *a_romcode, uint8_t a_th, uint8_t a_tl, uint8_t a_res, uint8_t a_force) {
	uint8_t tmp[5] = { 0x00 };
	
	if (NULL == a_romcode || 
			OW_SINGLEDROP == SOFT_OW_TOPOLOGY_GET(a_bus->conf)) {

		if (!a_force) {
			// first read ROM and check if the configuration register 
			// is not already set to the requested value
			ds18b20_read_scratchpad(a_bus, a_romcode, tmp, sizeof(tmp));
		}

		// build the configuration value
		tmp[0] = a_th;
		tmp[1] = a_tl;
		tmp[2] = ((a_res & 0x03) << 5) | 0x1f;

		// write only if configuration register differs or force has been
		// requested
		if (tmp[4] != tmp[2] || a_force) {
			sow_reset(a_bus);

			sow_write_byte(a_bus, OWN_SKIP_ROM);
			sow_write_byte(a_bus, DS18B20_CMD_WRITE_SP);

			// save to eeprom
			for (uint8_t len = 0; len < 3; len++)
				sow_write_byte(a_bus, tmp[len]);

			// persistently store in rom
			sow_reset(a_bus);
			sow_write_byte(a_bus, OWN_SKIP_ROM);
			sow_write_byte(a_bus, DS18B20_CMD_COPY_SP);
		}		
	}
}


void ds18b20_read_scratchpad(volatile struct soft_ow *a_bus, ow_romcode_t *a_romcode, volatile uint8_t *a_data, uint8_t len) {
	if (OW_POWER_PARASITE == SOFT_OW_POWER_MODE_GET(a_bus->conf))
		sow_strong_pullup(a_bus, 0);

	sow_reset(a_bus);

	if (NULL == a_romcode || 
			OW_SINGLEDROP == SOFT_OW_TOPOLOGY_GET(a_bus->conf)) {

		// read the data
		sow_write_byte(a_bus, OWN_SKIP_ROM);
		sow_write_byte(a_bus, DS18B20_CMD_READ_SP);

		while (len--) {
			*a_data++ = sow_read_byte(a_bus);
		}
	} // if
}

