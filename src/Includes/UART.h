#ifndef INCLUDES_UART_H_
#define INCLUDES_UART_H_

#include "./Base.h"

#ifdef HAL_UART_MODULE_ENABLED
#	if defined __has_include
#		if __has_include ("stm32f1xx_hal_uart.h")
#			include "stm32f1xx_hal_uart.h"
#		endif
#		if __has_include ("stm32f2xx_hal_uart.h")
#			include "stm32f2xx_hal_uart.h"
#		endif
#		if __has_include ("stm32f3xx_hal_uart.h")
#			include "stm32f3xx_hal_uart.h"
#		endif
#		if __has_include ("stm32f4xx_hal_uart.h")
#			include "stm32f4xx_hal_uart.h"
#		endif
#		if __has_include ("stm32l4xx_hal_uart.h")
#			include "stm32l4xx_hal_uart.h"
#		endif
#	endif
#else
#
#endif
#endif /* INCLUDES_UART_H_ */
