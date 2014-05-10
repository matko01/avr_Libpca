#include <spi_hw_poll.h>
#include <avr/power.h>

/* ================================================================================ */

static uint8_t _spi_hw_poll_getc(void *priv UNUSED, uint8_t *a_data);
static uint8_t _spi_hw_poll_sendc(void *priv UNUSED, uint8_t *a_data);
static uint8_t _spi_hw_poll_recv(void *priv UNUSED, void *a_data, uint8_t a_len, uint8_t a_waitall);
static uint8_t _spi_hw_poll_send(void *priv UNUSED, void *a_data, uint8_t a_len, uint8_t a_waitall);
static uint8_t _spi_hw_poll_available(void *priv UNUSED);

/* ================================================================================ */

static uint8_t _spi_hw_poll_getc(void *priv, uint8_t *a_data) {
	return spi_hw_poll_getc(a_data);
}

static uint8_t _spi_hw_poll_sendc(void *priv, uint8_t *a_data) {
	return spi_hw_poll_sendc(a_data);
}

static uint8_t _spi_hw_poll_recv(void *priv, void *a_data, uint8_t a_len, uint8_t a_waitall) {
	return spi_hw_poll_recv(a_data, a_len, a_waitall);
}

static uint8_t _spi_hw_poll_send(void *priv, void *a_data, uint8_t a_len, uint8_t a_waitall) {
	return spi_hw_poll_send(a_data, a_len, a_waitall);
}

static uint8_t _spi_hw_poll_available(void *priv) {
	return spi_hw_poll_available();
}

/* ================================================================================ */

struct bus_t spi_hw_poll_bus_get() {

	struct bus_t bus = {
		.f_getc = _spi_hw_poll_getc,
		.f_sendc = _spi_hw_poll_sendc,

		.f_recv = _spi_hw_poll_recv,
		.f_send = _spi_hw_poll_send,

		.f_avail = _spi_hw_poll_available,
		.f_peek = NULL,

		.priv = NULL,
	};
	return bus;
}


void spi_hw_poll_init(spi_mode mode, spi_speed speed) {
	power_spi_enable();
	SPCR = 0x00;

	if (mode) {
		SPCR |= _BV(MSTR);
		
		// SCK && MOSI && SS
		DDRB |= _BV(PORTB5) | _BV(PORTB3) | _BV(PORTB2);

		// MISO 
		DDRB &= ~_BV(PORTB4) ;
	}
	else {
		// SCK && MOSI
		DDRB &= ~_BV(PORTB5) & ~_BV(PORTB3) & ~_BV(PORTB2);
		DDRB |= _BV(PORTB4);
	}

	SPCR |= (speed & 0x03);
	SPCR |= _BV(SPE);
	SPSR = (speed & 0x04) >> 2;
}


void spi_hw_poll_clock_mode_set(spi_clock_mode mode) {
	SPCR |= (mode & 0x03) << 2;
}


uint8_t spi_hw_poll_sendc(uint8_t *a_data) {
	return spi_hw_poll_send(a_data, 1, 0);
}


uint8_t spi_hw_poll_getc(uint8_t *a_data) {
	return spi_hw_poll_recv(a_data, 1, 0);
}


uint8_t spi_hw_poll_recv(void *a_data, uint8_t a_len, uint8_t a_waitall) {
	uint8_t i = 0x00;

	while (i < a_len) {
		while (bit_is_clear(SPSR, SPIF));
		((uint8_t *)a_data)[i++] = SPDR;
	}

	return i;
}

#include <util/delay.h>

uint8_t spi_hw_poll_send(void *a_data, uint8_t a_len, uint8_t a_waitall) {
	uint8_t i = 0x00;
	while(i < a_len) {
		SPDR = ((uint8_t *)a_data)[i++];
		while (bit_is_clear(SPSR, SPIF));
	}
	return i;	
}


uint8_t spi_hw_poll_available(void) {
	return bit_is_set(SPSR, SPIF);
}

