#ifndef BEEPER_H_URHFWD8M
#define BEEPER_H_URHFWD8M

#include "tdelay.h"

/**
 * @brief block execution until the tone fades out
 *
 * @param __a_timer timer used for tone generation
 *
 * @return void
 */
#define beeper_block(__a_timer) _tdc_block(__a_timer)

/**
 * @brief initialize a given timer to act as a tone generator
 *
 * @param a_timer timer
 */
void beeper_init(e_timer a_timer);

/**
 * @brief generate a tone of specific frequency and duration using a timer
 *
 * @param a_timer timer 
 * @param freq frequency in Hz 
 * @param duration duration in milliseconds
 */
void beeper_beep(e_timer a_timer, uint32_t freq, uint32_t duration);

/**
 * @brief cancel tone generation 
 *
 * @param a_timer timer
 */
void beeper_off(e_timer a_timer);


#endif /* end of include guard: BEEPER_H_URHFWD8M */

