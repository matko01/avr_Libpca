#ifndef SPI_COMMON_H_RYRWDC2M
#define SPI_COMMON_H_RYRWDC2M

#include <common.h>


/**
 * @brief bus role
 */
typedef enum _spi_mode {
	E_SPI_MODE_SLAVE = 0,
	E_SPI_MODE_MASTER,
} spi_mode;


/**
 * @brief spi mode
 */
typedef enum _spi_clock_mode {
	E_CLOCK_MODE0 = 0,
	E_CLOCK_MODE1,
	E_CLOCK_MODE2,
	E_CLOCK_MODE3,
} spi_clock_mode;


#endif /* end of include guard: SPI_COMMON_H_RYRWDC2M */

