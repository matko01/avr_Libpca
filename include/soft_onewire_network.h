#ifndef OW_NETWORK_H_0MYSTYZB
#define OW_NETWORK_H_0MYSTYZB

/// replace with other Link Layer implementation
#include <soft_onewire.h>
#include <stdint.h>


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
uint16_t ow_net_search_rom(soft_ow *a_bus, ow_romcode_t *a_romcodes);



#endif /* end of include guard: OW_NETWORK_H_0MYSTYZB */

