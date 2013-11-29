#ifndef TWI_COMMON_H_2M5RXJHG
#define TWI_COMMON_H_2M5RXJHG

#include "config.h"
#include "common.h"


// #define TWI_DEBUG 1
#undef TWI_DEBUG


/** 
 * @brief defines if the transaction should end with sending the STOP bit
 */
#define E_TWI_BIT_SEND_STOP 0x40


/**
 * @brief if set indicates that the TWI HW is in the middle of operation
 */
#define	E_TWI_BIT_BUSY 0x10


/**
 * @brief standard i2c frequencies enumeration
 */
typedef enum _e_twi_scl_freq {
	E_TWI_SCL_100K = 0,
	E_TWI_SCL_250K,
	E_TWI_SCL_400K
} e_twi_scl_freq;


/**
 *  +-------------- unused
 *  |	+---------- user_send_stop
 *  |	|   +------ repeated_start status
 *  |   |   |   +-- state (IDLE/BUSY)
 *  |   |   |   |
 *  v | v | v | v | error
 * ---+---+---+---+--------
 *  7 | 6 | 5 | 4 | 3 - 0
 */
typedef volatile uint8_t twi_status_t;


/**
 * @brief debug function prototype
 */
typedef void (*twi_debug_hook_t)(void);


/**
 * @brief setup frequency prescaler and divider
 *
 * @param a_freq requested frequency enumeration
 */
void _twi_common_frequency_setup(uint8_t a_freq);


#if TWI_SLAVE_TRANSMITTER == 1 || TWI_SLAVE_RECEIVER == 1
#error TWI SLAVE MODE NOT SUPPORTED AT THE MOMENT
/**
 * @brief initialize MCU as a TWI slave with given address
 *
 * @param a_address
 */
void twi_common_setup_slave(uint8_t a_address, uint8_t a_mask);
#endif



#endif /* end of include guard: TWI_COMMON_H_2M5RXJHG */
