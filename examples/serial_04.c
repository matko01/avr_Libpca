#include "pca.h"

/**
 * @brief the format of binary data, the host will send a 64 bytes of data preceded by a crc16 checksum
 */
struct packet {
	uint16_t crc16;
	uint8_t data[64];
};

// variable for the data
static volatile struct packet p;


static void serial_collect_samples(void *a_data) {

	struct packet *data = (struct packet *)a_data;
	uint8_t size = 0x00;

	// exit in case of reception error
	if (0 >= (size = slip_recv((void *)data, sizeof(struct packet)))) {
		return;
	}

	// ignore incomplete data chunks
	if (size < 4) {
		return;
	}

	// verify if the information is genuine
	// of course this is only additional step, one do not really need to do that
	// it he only wants to be sure that the data send is correctly synchronized
	if (!slip_verify_crc16((unsigned char *)data, size, 0)) {
		return;
	}

	/* at this point the received data for sure is unique, synchronized and error free */
	/* it can be treated as genuine and undamaged */

	/* process the data */

}


int main(void)
{
	serial_init(E_BAUD_115200);	
	serial_install_interrupts(E_FLAGS_SERIAL_RX_INTERRUPT);
	serial_install_stdio();
	serial_flush();

	while (1) {
		serial_collect_samples(buffer);
	}

	return 0;
}
