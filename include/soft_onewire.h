#ifndef SOFT_ONEWIRE_H_ZBD6VGNS
#define SOFT_ONEWIRE_H_ZBD6VGNS

#include <config.h>
#include <common.h>
#include <gpio.h>

/// network layer commands
#define OWN_READ_ROM 		0x33
#define OWN_READ_ROM_ALT	0x0f
#define	OWN_SKIP_ROM		0xcc
#define OWN_MATCH_ROM		0x55
#define OWN_SEARCH_ROM		0xf0
#define OWN_OV_SKIP_ROM		0x3c
#define OWN_OV_MATCH_ROM	0x69

/// transport layer commands
#define OWT_READ_MEMORY		0xf0
#define OWT_READ_MEMORY_EXT	0xa5
#define OWT_READ_SUBKEY		0x66
#define OWT_WRITE_SUBKEY	0x99
#define OWT_WRITE_SP		0x0f
#define OWT_WRITE_SP_ALT	0x96
#define OWT_READ_SP			0xaa
#define OWT_READ_SP_ALT		0x69
#define OWT_COPY_SP			0x55
#define OWT_COPY_SP_ALT		0x3c
#define OWT_WRITE_PASSWD	0x5a

// eeprom devices
#define OWT_WRITE_MEMORY	0x0f
#define OWT_WRITE_STATUS	0x55
#define OWT_READ_STATUS		0xaa


/**
 * @brief defines power mode of the one wire device as a parasite (feed from data line) or external
 */
typedef enum _soft_ow_power_mode {
	OW_POWER_PARASITE = 0,
	OW_POWER_EXTERNAL,

	OW_POWER_LAST
} soft_ow_power_mode;


/**
 * @brief bus topology
 */
typedef enum _soft_ow_topology {
	/// bus contains a single slave device only
	OW_SINGLEDROP = 0,

	/// bus contains more than one device connected
	OW_MULTIDROP
} soft_ow_topology;

#define SOFT_OW_POWER_MODE_GET(__conf) (__conf & 0x01)
#define SOFT_OW_TOPOLOGY_GET(__conf) ((__conf & 0x02) >> 1)



/**
 * @brief software one-wire bus declaration
 */
struct soft_ow {
	
	// one wire bus gpio
	volatile gpio_pin bus;

	// 0 - power_mode
	// 1 - topology
	//  ----------- | x | x
	// -------------+---+---
	//  7 6 5 4 3 2 | 1 | 0
	uint8_t conf;
};


/**
 * @brief builds the configuration bitfield
 */
#define SOFT_OW_CONF_MAKE(__power, __topology) \
	(((__topology & 0x01) << 1) | (__power & 0x01))


/**
 * @brief initialize the bus, described by the bus descriptor
 *
 * @param a_bus bus descriptor
 */
void sow_init(volatile struct soft_ow *a_bus);


/**
 * @brief enable strong pullup
 *
 * This will drive the pin high. Useful for parasite powered devices, which temporary
 * require high amount of current (which cannot be drawn though the bus pull-up resistor)
 *
 * @param a_bus bus descriptor
 * @param a_enable 1 - enable pull-up, 0 - disable
 */
#define sow_strong_pullup(__a_bus, __a_enable) \
	if (__a_enable) { \
		GPIO_CONFIGURE_AS_OUTPUT(&__a_bus->bus); \
		GPIO_SET_HIGH(&__a_bus->bus); \
	} \
	else { \
		GPIO_CONFIGURE_AS_INPUT(&__a_bus->bus); \
	}


/**
 * @brief generate one wire reset pulse
 *
 * @param a_bus bus descriptor
 *
 * @return return 1 if presence detected, 0 otherwise
 */
uint8_t sow_reset(volatile struct soft_ow *a_bus);


/**
 * @brief read a single bit out of one wire bus
 *
 * @param a_bus bus descriptor
 *
 * @return bit read
 */
uint8_t _sow_read_bit(volatile struct soft_ow *a_bus);


/**
 * @brief write a single bit to one wire bus
 *
 * @param a_bus bus descriptor
 * @param a_bit bit to be written
 *
 */
void _sow_write_bit(volatile struct soft_ow *a_bus, uint8_t a_bit);


/**
 * @brief read byte from the one wire bus
 *
 * Generates read time slot in order to read a byte from the one wire bus
 *
 * @param a_bus bus descriptor
 *
 * @return byte read
 */
uint8_t sow_read_byte(volatile struct soft_ow *a_bus);


/**
 * @brief write byte on one-wire bus
 *
 * Generates write time slot in order to write a byte to the one wire bus
 *
 * @param a_bus bus descriptor
 * @param a_byte byte to written
 *
 */
void sow_write_byte(volatile struct soft_ow *a_bus, uint8_t a_byte);


/**
 * @brief read the requested amount of data from the one wire bus
 *
 * Generates a requested amount of read slots and retrieves data read
 *
 * @param a_bus bus descriptor
 * @param a_data buffer for incoming data
 * @param a_maxlen how much data to read
 *
 */
void sow_read_data(volatile struct soft_ow *a_bus, uint8_t *a_data, uint8_t a_maxlen);


/**
 * @brief write the given amount of data to one wire bus
 *
 * @param a_bus bus descriptor
 * @param a_data data buffer holding the data
 * @param a_len how many bytes to write
 *
 */
void sow_write_data(volatile struct soft_ow *a_bus, uint8_t *a_data, uint8_t a_len);


#endif /* end of include guard: SOFT_ONEWIRE_H_ZBD6VGNS */

