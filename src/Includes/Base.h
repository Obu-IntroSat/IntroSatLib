#ifndef INCLUDES_BASE_H_
#define INCLUDES_BASE_H_


#ifndef ARDUINO
#	if defined __has_include
#		if __has_include ("stm32f1xx_hal.h")
#			include "stm32f1xx_hal.h"
#		endif
#		if __has_include ("stm32f2xx_hal.h")
#			include "stm32f2xx_hal.h"
#		endif
#		if __has_include ("stm32f3xx_hal.h")
#			include "stm32f3xx_hal.h"
#		endif
#		if __has_include ("stm32f4xx_hal.h")
#			include "stm32f4xx_hal.h"
#		endif
#		if __has_include ("stm32l4xx_hal.h")
#			include "stm32l4xx_hal.h"
#		endif
#	endif
#else
#
#endif

#endif /* INCLUDES_BASE_H_ */
