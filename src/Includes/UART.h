#ifndef INCLUDES_UART_H_
#define INCLUDES_UART_H_

#include "./Base.h"

#ifdef INTROSAT_USE_HAL
#if defined(HAL_UART_MODULE_ENABLED) || defined(HAL_USART_MODULE_ENABLED)
#define INTROSAT_HAVE_UART
#include "stm32f1xx_hal_i2c.h"
#endif /* HAL_I2C_MODULE_ENABLED */
#endif /* INTROSAT_USE_HAL */

#ifdef INTROSAT_USE_ARDUINO
	#define INTROSAT_HAVE_UART
#endif /* INTROSAT_USE_ARDUINO */

#endif /* INCLUDES_UART_H_ */
