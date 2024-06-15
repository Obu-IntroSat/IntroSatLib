#ifndef INCLUDES_GPIO_H_
#define INCLUDES_GPIO_H_

#include "./Base.h"

#ifdef HAL_GPIO_MODULE_ENABLED
#	if defined __has_include
#		if __has_include ("stm32f1xx_hal_gpio.h")
#			include "stm32f1xx_hal_gpio.h"
#		endif
#		if __has_include ("stm32f2xx_hal_gpio.h")
#			include "stm32f2xx_hal_gpio.h"
#		endif
#		if __has_include ("stm32f3xx_hal_gpio.h")
#			include "stm32f3xx_hal_gpio.h"
#		endif
#		if __has_include ("stm32f4xx_hal_gpio.h")
#			include "stm32f4xx_hal_gpio.h"
#		endif
#		if __has_include ("stm32l4xx_hal_gpio.h")
#			include "stm32l4xx_hal_gpio.h"
#		endif
#	endif
#else
#
#endif

#endif /* INCLUDES_GPIO_H_ */
