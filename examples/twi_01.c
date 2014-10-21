#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <avr/io.h>
#include "pca.h"

#define EEPROM_HW_ADDR 0x00
#define EEPROM_ADDR (0x50 | EEPROM_HW_ADDR)


void debug() {
	printf("status: %02x\n", TWSR & 0xf8);
}


int main(void)
{
	char test_data[] = "data to be place int the EEPROM";
	char buff[128] = {0x00};

	serial_init(E_BAUD_4800);	
	serial_install_interrupts(E_FLAGS_SERIAL_RX_INTERRUPT);
	serial_install_stdio();
	serial_flush();

	struct twi_ctx twi_ctx;

	// write test data
	twi_minit(&twi_ctx, E_TWI_SCL_100K);
	const uint16_t start_addr = 0x00;

	twi_common_debug_hook_install(&twi_ctx, debug);

	strcpy(&buff[2], test_data);
	twi_mtx(&twi_ctx, EEPROM_ADDR, (uint8_t *)buff, 2 + strlen(test_data), E_TWI_BIT_SEND_STOP);
	twi_sync();

	memset(buff, 0x00, sizeof(buff));

	// send the address to the EEPROM
	twi_mtx(&twi_ctx, EEPROM_ADDR, (uint8_t *)&start_addr, 2, E_TWI_BIT_SEND_STOP);
	twi_sync();

	// obtain the data from the EEPROM
	twi_mrx(&twi_ctx, EEPROM_ADDR, (uint8_t *)buff, strlen(test_data), E_TWI_BIT_SEND_STOP);
	twi_sync();

	printf("data: [%s]\n", buff);

	while(1);
	return 0;
}
