#ifndef __TIMER_COMMON_H__
#define __TIMER_COMMON_H__

#include "config.h"
#include "common.h"

/**
 * @brief timer prescalers bitmasks
 */
#define TIMER_PRESCALER_1 0x01
#define TIMER_PRESCALER_8 0x02
#define TIMER_PRESCALER_64 0x03
#define TIMER_PRESCALER_256 0x04
#define TIMER_PRESCALER_1024 0x05

/**
 * @brief available timers
 */
typedef enum _e_timer {
	E_TIMER0,
	E_TIMER1,
	E_TIMER2,
	E_TIMER_LAST
} e_timer;


/**
 * @brief enable timer interrupt (start counting down)
 *
 * @param a_timer timer
 */
void _timer_enable_interrupt(e_timer a_timer);

/**
 * @brief disable COMPA interrupt for timer
 *
 * @param a_timer timer
 */
void _timer_disable_interrupt(e_timer a_timer);

/**
 * @brief calculate prescaler and OCR value for a given frequency
 *
 * @param a_freq requested frequency
 * @param a_criterion maximum value for OCR (255 for 8 bit timers 65535 for 16 bit timers)
 *
 * @return 3 bit prescaler value starting at bit 24 + 16 bit OCR starting from bit 0
 */
uint32_t _timer_freq_prescale(uint32_t a_freq, uint16_t a_criterion);



#endif /* __TIMER_COMMON_H__ */
