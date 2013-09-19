#ifndef TDELAY_H_IPOQKAD0
#define TDELAY_H_IPOQKAD0

#include "config.h"
#include "common.h"
#include "tdelay_common.h"


/**
 * @brief initialize a selected timer to act as a delay timer
 *
 * @param a_timer timer
 */
void tdelay_init(e_timer a_timer);


/**
 * @brief delay execution using a given timer
 *
 * @param e_timer timer
 * @param a_delay delay in miliseconds
 */
void tdelay_ms(e_timer, uint32_t a_delay);

/**
 * @brief delay execution using a given timer
 *
 * @param e_timer timer
 * @param a_delay delay in microseconds
 */
void tdelay_us(e_timer, uint32_t a_delay);

#endif /* end of include guard: TDELAY_H_IPOQKAD0 */
