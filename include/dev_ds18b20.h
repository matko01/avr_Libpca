#ifndef DEV_DS18B20_H_1QHHIDKK
#define DEV_DS18B20_H_1QHHIDKK

#include <config.h>
#include <common.h>
#include <soft_onewire.h>
#include <soft_onewire_network.h>

// ds18b20 specific commands
#define DS18B20_CMD_ALARM_SEARCH 0xec
#define DS18B20_CMD_CONVERT_T 0x44
#define DS18B20_CMD_WRITE_SP 0x4e
#define DS18B20_CMD_READ_SP 0xbe
#define DS18B20_CMD_COPY_SP 0x48
#define DS18B20_CMD_RECALL_EE 0xb8
#define DS18B20_CMD_READ_POWER 0xb4

void ds18b20_start_conversion(struct soft_ow *a_bus, ow_romcode_t *a_romcode);
void ds18b20_read_scratchpad(struct soft_ow *a_bus, uint8_t *a_data, uint8_t len);

#endif /* end of include guard: DEV_DS18B20_H_1QHHIDKK */

