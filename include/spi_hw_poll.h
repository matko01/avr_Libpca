#ifndef SPI_HW_POLL_H_JM4LGPKV
#define SPI_HW_POLL_H_JM4LGPKV

#include <config.h>
#include <common.h>

#include <bus.h>
#include <spi_common.h>


typedef enum _spi_speed {
	E_SPI_SPEED_F4 = 0,
	E_SPI_SPEED_F16,
	E_SPI_SPEED_F64,
	E_SPI_SPEED_F128,
	E_SPI_SPEED_F2,
	E_SPI_SPEED_F8,
	E_SPI_SPEED_F32,
} spi_speed;


void spi_hw_poll_init(spi_mode mode, spi_speed);
void spi_hw_poll_clock_mode_set(spi_clock_mode mode);

uint8_t spi_hw_poll_getc(uint8_t *a_data);
uint8_t spi_hw_poll_sendc(uint8_t *a_data);
uint8_t spi_hw_poll_recv(void *a_data, uint8_t a_len, uint8_t a_waitall);
uint8_t spi_hw_poll_send(void *a_data, uint8_t a_len, uint8_t a_waitall);
uint8_t spi_hw_poll_available(void);

// predeclared abstract bus
extern struct bus_t g_bus_spi_hw_poll;


#endif /* end of include guard: SPI_HW_POLL_H_JM4LGPKV */


