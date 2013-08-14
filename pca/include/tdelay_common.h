#ifndef TDELAY_COMMON_H_NXGPZAZA
#define TDELAY_COMMON_H_NXGPZAZA

#include "config.h"
#include "common.h"

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
 * @brief setup the timer to expire after given amount of miliseconds
 *
 * @param a_timer timer
 * @param a_delay delay in ms
 */
void _tdc_setup_ms(e_timer a_timer, uint32_t a_delay);

/**
 * @brief enable timer interrupt (start counting down)
 *
 * @param a_timer timer
 */
void _tdc_enable_interrupt(e_timer a_timer);

/**
 * @brief block until timer expires (must be configured before)
 *
 * @param a_timer time
 */
void _tdc_block(e_timer a_timer);

#endif /* end of include guard: TDELAY_COMMON_H_NXGPZAZA */

