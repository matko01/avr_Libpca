#ifndef DEV_PCD8544_H_PN3AJ51E
#define DEV_PCD8544_H_PN3AJ51E

#include <config.h>
#include <common.h>
#include <bus.h>
#include <gpio.h>


typedef enum _pcd8544_mode {
	E_DISPLAY_BLANK = 0,
	E_DISPLAY_NORMAL = 0x04,
	E_DISPLAY_ALLON = 0x01,
	E_DISPLAY_INVERSE = 0x05,
} pcd8544_mode;


#define PCD8544_CMD 0x00
#define PCD8544_DATA 0x01

#define PCD8544_W 84
#define PCD8544_H 48

#define PCD8544_CMD_FUNCTION_SET(__pd, __v, __h) \
	(0x20 | (__pd << 2) | (__v << 1) | (__h))

#define PCD8544_CMD_SET_X(__x) \
	(0x80 | __x)

#define PCD8544_CMD_SET_Y(__y) \
	(0x40 | (__y & 0x07))

#define PCD8544_CMD_DISPLAY_CTRL(__d) \
	(0x08 | (__d & 0x07))

#define PCD8544_CMD_TMP_CTRL(__tc) \
	(0x04 | (__tc & 0x03))

#define PCD8544_CMD_BIAS(__bias) \
	(0x10 | (__bias & 0x07))

#define PCD8544_CMD_SET_VOP(__vop) \
	(0x80 | (__vop))

#define PCD8544_SET_BIT_HIGH(__mem, __x, __y) \
	__mem[x + (y >> 3)*PCD8544_W] |= _BV( y - ((y >> 3) << 3) )

#define PCD8544_SET_BIT_LOW(__mem, __x, __y) \
	__mem[x + (y >> 3)*PCD8544_W] &= ~_BV( y - ((y >> 3) << 3) )



/**
 * @brief display context
 */
struct dev_pcd8544_ctx {
	struct bus_t *bus;

	// serial chip enable
	gpio_pin sce;

	// data/control 1/0
	gpio_pin dc;

	// reset - active low
	gpio_pin res;

	uint8_t x,y;
};


void pcd8544_init(struct dev_pcd8544_ctx *a_disp);
void pcd8544_write(struct dev_pcd8544_ctx *a_disp, uint8_t mode, uint8_t data);
void pcd8544_clrscr(struct dev_pcd8544_ctx *a_disp);
void pcd8544_gotoxy(struct dev_pcd8544_ctx *a_disp, uint8_t x, uint8_t y);

void pcd8544_putc(struct dev_pcd8544_ctx *a_disp, char c);
void pcd8544_install_stdout(struct dev_pcd8544_ctx *a_disp);
void pcd8544_str(struct dev_pcd8544_ctx *a_disp, char *str);

void pcd8544_putblock(struct dev_pcd8544_ctx *a_disp, uint8_t x, uint8_t y, uint8_t c);
void pcd8544_blit(struct dev_pcd8544_ctx *a_disp, uint8_t *data);
void pcd8544_fill(struct dev_pcd8544_ctx *a_disp, uint8_t data);




#endif /* end of include guard: DEV_PCD8544_H_PN3AJ51E */

