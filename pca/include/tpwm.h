#ifndef TPWM_H_IUK1KDC0
#define TPWM_H_IUK1KDC0

#include "config.h"
#include "common.h"
#include "timer_common.h"


/**
 * @brief frequencies allowed to be configured
 */
typedef enum _e_pwm_freq {
	E_PWM_FREQ_4M = 2,	// 2 
	E_PWM_FREQ_2M,		// 3
	E_PWM_FREQ_1M,		// 4
	E_PWM_FREQ_500K,	// 5
	E_PWM_FREQ_250K,	// 6
	E_PWM_FREQ_125K,	// 7
	E_PWM_FREQ_62500,	// 8
	E_PWM_FREQ_31250,	// 9
	E_PWM_FREQ_15625,	// 10
	E_PWM_FREQ_7812,	// 11
	E_PWM_FREQ_3906,	// 12
	E_PWM_FREQ_1953,	// 13
	E_PWM_FREQ_976,		// 14
	E_PWM_FREQ_488,		// 15
	E_PWM_FREQ_244,		// 16
	E_PWM_FREQ_61		// 8 bit timers
} e_pwm_freq;

void tpwm_fpwm_init(e_timer a_timer);
void tpwm_pwm_init(e_timer a_timer);
	
uint32_t tpwm_setup_for_bitres(e_timer a_timer, uint8_t a_bitres);
uint8_t tpwm_setup_for_freq(e_timer a_timer, e_pwm_freq a_freq);

#endif /* TPWM_H_IUK1KDC0 */

