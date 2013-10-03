#ifndef SOFT_ONEWIRE_H_ZBD6VGNS
#define SOFT_ONEWIRE_H_ZBD6VGNS

/**
 * @brief software one-wire bus declaration
 */
struct soft_ow {
	uint8_t pin;
	uint8_t *ddr;
	uint8_t *inp; // PINX
	uint8_t *outp; // PORTX
};


void sow_init(struct soft_ow *a_bus);
void sow_string_pullup(struct soft_ow *a_bus, uint8_t a_enable);

#endif /* end of include guard: SOFT_ONEWIRE_H_ZBD6VGNS */

