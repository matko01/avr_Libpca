#ifndef SOFT_ONEWIRE_H_ZBD6VGNS
#define SOFT_ONEWIRE_H_ZBD6VGNS

#include "common.h"

/**
 * @brief software one-wire bus declaration
 */
struct soft_ow {
	uint8_t pin; // the pin number
	uint8_t *ddr; // DDRX
	uint8_t *inp; // PINX
	uint8_t *outp; // PORTX
};


void sow_init(struct soft_ow *a_bus);
void sow_string_pullup(struct soft_ow *a_bus, uint8_t a_enable);

uint8_t sow_reset(struct soft_ow *a_bus);

uint8_t sow_read_byte(struct soft_ow *a_bus);
uint8_t sow_write_byte(struct soft_ow *a_bus, uint8_t a_byte);
uint8_t sow_read_data(struct soft_ow *a_bus, uint8_t *a_data, uint8_t a_maxlen);
uint8_t sow_write_data(struct soft_ow *a_bus, uint8_t *a_data, uint8_t a_len);

#endif /* end of include guard: SOFT_ONEWIRE_H_ZBD6VGNS */

