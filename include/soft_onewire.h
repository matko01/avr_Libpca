#ifndef SOFT_ONEWIRE_H_ZBD6VGNS
#define SOFT_ONEWIRE_H_ZBD6VGNS

#include "common.h"


/**
 * @brief software one-wire bus declaration
 */
struct soft_ow {
	uint8_t pin; // the pin number
	volatile uint8_t *ddr; // DDRX
	volatile uint8_t *inp; // PINX
	volatile uint8_t *outp; // PORTX
};


/**
 * @brief initialize the bus, described by the bus descriptor
 *
 * @param a_bus bus descriptor
 */
void sow_init(struct soft_ow *a_bus);


/**
 * @brief enable strong pullup
 *
 * This will drive the pin high. Useful for parasite powered devices, which temporary
 * require high amount of current (which cannot be drawn though the bus pull-up resistor)
 *
 * @param a_bus bus descriptor
 * @param a_enable 1 - enable pull-up, 0 - disable
 */
void sow_strong_pullup(struct soft_ow *a_bus, uint8_t a_enable);


/**
 * @brief generate one wire reset pulse
 *
 * @param a_bus bus descriptor
 *
 * @return return 1 if devices detected, 0 otherwise
 */
uint8_t sow_reset(struct soft_ow *a_bus);


/**
 * @brief read byte from the one wire bus
 *
 * Generates read time slot in order to read a byte from the one wire bus
 *
 * @param a_bus bus descriptor
 *
 * @return byte read
 */
uint8_t sow_read_byte(struct soft_ow *a_bus);


/**
 * @brief write byte on one-wire bus
 *
 * Generates write time slot in order to write a byte to the one wire bus
 *
 * @param a_bus bus descriptor
 * @param a_byte byte to written
 *
 * @return 1 one success
 */
uint8_t sow_write_byte(struct soft_ow *a_bus, uint8_t a_byte);


/**
 * @brief read the requested amount of data from the one wire bus
 *
 * Generates a requested amount of read slots and retrieves data read
 *
 * @param a_bus bus descriptor
 * @param a_data buffer for incoming data
 * @param a_maxlen how much data to read
 *
 * @return number of bytes read
 */
uint8_t sow_read_data(struct soft_ow *a_bus, uint8_t *a_data, uint8_t a_maxlen);


/**
 * @brief write the given amount of data to one wire bus
 *
 * @param a_bus bus descriptor
 * @param a_data data buffer holding the data
 * @param a_len how many bytes to write
 *
 * @return number of bytes written
 */
uint8_t sow_write_data(struct soft_ow *a_bus, uint8_t *a_data, uint8_t a_len);


#endif /* end of include guard: SOFT_ONEWIRE_H_ZBD6VGNS */

