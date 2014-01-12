#ifndef DEV_TMP36_H_RSHVECXP
#define DEV_TMP36_H_RSHVECXP

#include "config.h"
#include "common.h"

#define tmp36_raw_to_celsius(__reading, __adc_max, __adc_vref) \
	(((double)(__reading)/(__adc_max - 1))*__adc_vref - 500.0f)/10

#endif /* DEV_TMP36_H_RSHVECXP */

