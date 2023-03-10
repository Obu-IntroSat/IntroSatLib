#ifndef INCLUDES_BASE_H_
#define INCLUDES_BASE_H_

#if defined(USE_HAL_DRIVER) && !defined(ARDUINO)
	#include "stm32f1xx_hal.h"
	#define INTROSAT_USE_HAL
#endif

#if defined(ARDUINO)
#define USE_ARDUINO
#include "Arduino.h"
#endif

#endif /* INCLUDES_BASE_H_ */
