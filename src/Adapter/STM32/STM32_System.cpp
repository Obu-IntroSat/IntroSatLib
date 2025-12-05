#define INTROSATLIB_INTERNAL
#include "Adapter/System.h"

#if defined(HAL_I2C_MODULE_ENABLED) && !defined(ARDUINO)
#include "stm32xxyy_hal.h"

uint32_t IntroSatLib::system::GetTick() {
	return HAL_GetTick();
}

void IntroSatLib::system::Delay(uint32_t milliseconds) {
	HAL_Delay(milliseconds);
}

#endif /* defined(HAL_I2C_MODULE_ENABLED) && !defined(ARDUINO) */
