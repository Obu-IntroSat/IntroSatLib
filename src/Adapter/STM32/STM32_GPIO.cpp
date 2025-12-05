#include "Adapter/GPIO.h"
#if defined(HAL_GPIO_MODULE_ENABLED) && !defined(ARDUINO)

IntroSatLib::interfaces::GPIO::GPIO(GPIO_HANDLE_TYPE* port, uint16_t pin): _port(port), _pin(pin) { }

uint8_t IntroSatLib::interfaces::GPIO::read() const
{ return HAL_GPIO_ReadPin(_port, _pin); }

void IntroSatLib::interfaces::GPIO::write(uint8_t state) const
{ HAL_GPIO_WritePin(_port, _pin, state > 0 ? GPIO_PIN_SET : GPIO_PIN_RESET); }



#endif /* defined(HAL_GPIO_MODULE_ENABLED) && !defined(ARDUINO) */
