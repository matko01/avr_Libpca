#ifndef __TWI_POLL_H__
#define __TWI_POLL_H__

#include "config.h"
#include "common.h"
#include "twi_common.h"

#warning NOT IMPLEMENTED YET

/**
 * @brief TWI bus context structure
 */
struct twi_poll_ctx {
	/// status
	twi_status_t status; 

#ifdef TWI_DEBUG
	twi_debug_hook_t debug_hook;
#endif
};


#ifdef TWI_DEBUG
/**
 * @brief install debug hook which will be called in the interrupt
 *
 * @param a_dbg debug hook to be called
 */
void twi_common_debug_hook_install(twi_debug_hook_t);
#endif


void twi_poll_init(uint8_t a_freq);
// twi_poll_mtx()
// twi_poll_mrx()
// twi_poll_search_devices()


#endif /* __TWI_POLL_H__ */
