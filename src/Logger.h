#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdint.h>

#define LOGDATA 0

#ifdef ARDUINO
#include <Arduino.h>
#else
	#if __has_include ("stm32f4xx_hal.h")
		#include "stm32f4xx_hal.h"
	#endif

	#if __has_include ("stm32f1xx_hal.h")
		#include "stm32f1xx_hal.h"
	#endif
#endif /* ARDUINO */

void __attribute__((weak)) LOGGER(const char *text, uint8_t len);

void logText(const char *text);
void logHEX(uint8_t val);
void logNumber(uint8_t val);
HAL_StatusTypeDef logStatus(HAL_StatusTypeDef status);


#define LOG_I2C_ADDRESS() \
logText("Device in "); \
logHEX(_address >> 1);

#if LOGDATA
#define LOG_I2C_BUFFER(Sep, Data, Nbytes) { \
logText(" - "); \
for(uint8_t i = 0; i < Nbytes; i++) { \
	logHEX(Data[i]); \
	if (i != (Nbytes - 1)) logText(Sep); \
} \
}

#else
#define LOG_I2C_BUFFER(Sep, Data, Nbytes)
#endif

#endif /* LOGGER_H_ */
