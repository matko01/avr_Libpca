#include <soft_onewire_network.h>

// undefine this to save some bytes
#define DESIGN_BY_CONTRACT 1


uint8_t ow_net_search_rom(soft_ow *a_bus, uint8_t max_devices, ow_romcode_t *a_romcodes) {

	uint8_t bit_index = 0x00;
	uint8_t dev_cnt = 0x00;
	
	/* B | A */
	uint8_t bits = 0x00;

#ifdef DESIGN_BY_CONTRACT	
	// input parameters validation
	if (!a_romcodes || !max_devices) {
		return 0;
	}

	// single drop configuration
	if (!(a_bus & 0x02)) {
		max_devices = 1;
	}
#endif

	// no devices on the bus
	if (!sow_reset(a_bus)) {
		return 0;
	}

	memset(a_romcodes, 0x00, max_devices * sizeof(ow_romcode_t));
	sow_write_byte(a_bus, OWN_SEARCH_ROM);

	bits = _sow_read_bit(a_bus); // bit A
	bits |= (_sow_read_bit(a_bus) << 4); // bit B

	if (0x11 == bits) {
		// two ones indicate no response from the devices (no one pulled the line low)
		return 0;
	}

	// we've two zeros - there's a address bit conflict
	if (!bits) {
	}



}
