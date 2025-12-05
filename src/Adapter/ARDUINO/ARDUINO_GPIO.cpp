#include "Adapter/GPIO.h"
#if defined(ARDUINO)

IntroSatLib::interfaces::GPIO::GPIO(GPIO_HANDLE_TYPE* port, uint16_t pin): _port(nullptr), _pin(*port) { 
    UNUSED(pin);
}

uint8_t IntroSatLib::interfaces::GPIO::read() const
{ return digitalRead(_pin); }

void IntroSatLib::interfaces::GPIO::write(uint8_t state) const
{ digitalWrite(_pin, state > 0 ? HIGH : LOW); }



#endif /* defined(HAL_GPIO_MODULE_ENABLED) && !defined(ARDUINO) */
