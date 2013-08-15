#ifndef TDELAY_COMMON_H_NXGPZAZA
#define TDELAY_COMMON_H_NXGPZAZA

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
 * @brief available timers which can be used for delays
 */
typedef enum _e_timer {

#if TDELAY_IMPLEMENT_T0_INT == 1
	E_TIMER0,
#endif

#if TDELAY_IMPLEMENT_T1_INT == 1	
	E_TIMER1,
#endif

#if TDELAY_IMPLEMENT_T2_INT == 1	
	E_TIMER2,
#endif

	E_TIMER_LAST
} e_timer;


/**
 * @brief configure the duration for a given timer
 *
 * @param a_tim timer
 * @param a_dur duration value (in cycles)
 */
void _tdc_set_duration(e_timer a_tim, uint32_t a_dur);

/**
 * @brief set to 1 if you want to toggle OCMPXA PIN down, once timer expires
 *
 * @param a_tim timer
 * @param a_pin 1 - pull down, 0 - do nothing
 */
void _tdc_set_cmp_pin(e_timer a_tim, uint8_t a_pin);

/**
 * @brief configure timer for a specified frequency and duration
 *
 * @param a_timer timer
 * @param a_freq freq
 * @param a_delay duration
 */
void _tdc_setup_delay(e_timer a_timer, uint32_t a_freq, uint32_t a_delay);

/**
 * @brief enable timer interrupt (start counting down)
 *
 * @param a_timer timer
 */
void _tdc_enable_interrupt(e_timer a_timer);

/**
 * @brief disable COMPA interrupt for timer
 *
 * @param a_timer timer
 */
void _tdc_disable_interrupt(e_timer a_timer);

/**
 * @brief block until timer expires (must be configured before)
 *
 * @param a_timer time
 */
void _tdc_block(e_timer a_timer);


/**
 * @brief calculate prescaler and OCR value for a given frequency
 *
 * @param a_freq requested frequency
 * @param a_criterion maximum value for OCR (255 for 8 bit timers 65535 for 16 bit timers)
 *
 * @return 3 bit prescaler value starting at bit 24 + 16 bit OCR starting from bit 0
 */
uint32_t _tdc_freq_prescale(uint32_t a_freq, uint16_t a_criterion);



#endif /* end of include guard: TDELAY_COMMON_H_NXGPZAZA */

