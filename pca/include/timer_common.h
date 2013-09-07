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
void _timer_en_compa_int(e_timer a_timer);

/**
 * @brief disable COMPA interrupt for timer
 *
 * @param a_timer timer
 */
void _timer_dis_compa_int(e_timer a_timer);

/**
 * @brief calculate prescaler and OCR value for a given frequency
 *
 * @param a_freq requested frequency
 * @param a_criterion maximum value for OCR (255 for 8 bit timers 65535 for 16 bit timers)
 *
 * @return 3 bit prescaler value starting at bit 24 + 16 bit OCR starting from bit 0
 */
uint32_t _timer_freq_prescale(e_timer a_timer, uint32_t a_freq, uint16_t a_criterion);

/**
 * @brief initialize requested timer to CTC mode. Prescaler is set to 0x00 (no clock)
 *
 * @param a_timer timer
 */
void _timer_init_ctc(e_timer a_timer);

/**
 * @brief setup the top value in OCRXA and the prescaler value accordingly to the contents of combined variable pocr
 *
 * @param a_timer timer
 * @param a_pocr combined variable containing info about OCRXA value and prescaler
 */
void _timer_setup_ctc(e_timer a_timer, uint32_t a_pocr);

/**
 * @brief enable OCXA pin as output
 *
 * @param a_timer timer
 */
void _timer_en_oca(e_timer a_timer);

/**
 * @brief enable OCXB pin as output
 *
 * @param a_timer timer
 */
void _timer_en_ocb(e_timer a_timer);

/**
 * @brief configure given timer to fast pwm mode
 *
 * @param a_timer
 */
void _timer_init_fpwm(e_timer a_timer);

/**
 * @brief configure given timer to phase correct mode
 *
 * @param a_timer
 */
void _timer_init_pwm(e_timer a_timer);


#endif /* __TIMER_COMMON_H__ */
