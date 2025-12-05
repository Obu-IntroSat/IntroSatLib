#ifndef ADAPTER_GPIO_H_
#define ADAPTER_GPIO_H_

#include "IntroSatLib_def.h"
#include "Adapter/System.h"
#ifdef ARDUINO
/*********************************/
/********** Arduino IDE **********/
/*********************************/
	#include "Arduino.h"
	#define ISL_GPIO_ENABLED
	namespace IntroSatLib::interfaces {using GPIO_HANDLE_TYPE = uint8_t;}

#else
/*********************************/
/************ Native *************/
/*********************************/
	#if defined(AVR)
	/************** AVR  **************/
		//  This is not yet supported, but it is here for future reference.
		//  AVR-series in Arduino IDE
		#error "Bare AVR outside of Arduino IDE is not yet supported"

	#elif defined(USE_HAL_DRIVER) // TODO Change to more reusable symbol
	/************  STM32 **************/

		// Include HAL for supported STM32 platforms
		#include "./STM32/stm32xxyy_hal.h"

		#ifndef HAL_MODULE_ENABLED
			#error "HAL configuration incorrect"
		#endif

		#ifdef HAL_GPIO_MODULE_ENABLED
			// define STM32-specific handle type for GPIO
			#define ISL_GPIO_ENABLED
			namespace IntroSatLib::interfaces {using GPIO_HANDLE_TYPE = GPIO_TypeDef;}

		#elif !defined(INTROSATLIB_INTERNAL)
			#error "GPIO not enabled as part of HAL"
		#endif


	// #elif defined(AMUR)
	/************** AMUR *************/
		// #error "AMUR not yet supported"

	#else
	/************ UNKNOWN ************/
	//#ifndef INTROSATLIB_INTERNAL
		#error "Unsupported system: neither AVR/ARDUINO nor USE_HAL_DRIVER defined. Please check your platform macros."
		#error "Currently supported systems are: stm32 with HAL, stm32duino. AVR planned for future support."
	//#endif
	#endif
#endif /* ARDUINO */


#ifdef ISL_GPIO_ENABLED

namespace IntroSatLib {
namespace interfaces {



class GPIO final {
private:
	GPIO_HANDLE_TYPE* _port = 0;
	uint16_t _pin = 0;
public:
	GPIO(GPIO_HANDLE_TYPE* port, uint16_t pin = 0);

	uint8_t read() const;

	void write(uint8_t state) const;

	void reset() const { write(0); }
	void set() const { write(1); }

	uint8_t wait(uint8_t state, uint16_t timeout = 0xFFFF) const {
		using ::IntroSatLib::system::GetTick;
		state = !!state;
		uint32_t startTime = ::IntroSatLib::system::GetTick();
		while(read() != state)
		{
			if ((system::GetTick() - startTime) > timeout) { return 1; }
		}
		return 0;
	};

	uint8_t waitReset(uint16_t timeout = 0xFFFF) const { return wait(0, timeout); }
	uint8_t waitSet(uint16_t timeout = 0xFFFF) const { return wait(1, timeout); }
};

} /* namespace intefaces */
} /* namespace IntroSatLib */





#endif /* ISL_GPIO_ENABLED */
#endif /* ADAPTER_GPIO_H_ */
