#ifndef INTERFACES_GPIO_H_
#define INTERFACES_GPIO_H_

#ifdef ARDUINO
	#include "Arduino.h"
	#include "Wire.h"
#else
	#if __has_include ("stm32f4xx_hal.h")
		#include "stm32f4xx_hal.h"
	#endif

	#if __has_include ("stm32f1xx_hal.h")
		#include "stm32f1xx_hal.h"
	#endif
#endif

namespace IntroSatLib {
namespace intefaces {

class GPIO final {
private:
	GPIO_TypeDef* _port = 0;
	uint16_t _pin = 0;
public:
	GPIO(GPIO_TypeDef* port, uint16_t pin): _port(port), _pin(pin) { }

	uint8_t read() const
	{ return HAL_GPIO_ReadPin(_port, _pin); }

	void write(uint8_t state) const
	{ HAL_GPIO_WritePin(_port, _pin, state > 0 ? GPIO_PIN_SET : GPIO_PIN_RESET); }

	void reset() const { write(0); }
	void set() const { write(1); }

	uint8_t wait(uint8_t state, uint16_t timeout = 0xFFFF) const
	{
		state = !!state;
		uint32_t firstTime = HAL_GetTick();
		while(read() != state)
		{
			if ((HAL_GetTick() - firstTime) > timeout) { return 1; }
		}
		return 0;
	}

	uint8_t waitReset(uint16_t timeout = 0xFFFF) const
	{ return wait(0, timeout); }

	uint8_t waitSet(uint16_t timeout = 0xFFFF) const
	{ return wait(1, timeout); }
};

} /* namespace intefaces */
} /* namespace IntroSatLib */

#endif /* INTERFACES_GPIO_H_ */
