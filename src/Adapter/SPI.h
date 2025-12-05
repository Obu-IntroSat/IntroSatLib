#ifndef ADAPTER_SPI_H_
#define ADAPTER_SPI_H_

// Include general Library definitions
#include "IntroSatLib_def.h"

#ifdef ARDUINO
/*********************************/
/********** Arduino IDE **********/
/*********************************/
	#include "Arduino.h"
	#include <SPI.h>
	#define ISL_SPI_ENABLED
	namespace IntroSatLib::interfaces {using SPI_HANDLE_TYPE = SPIClass;}

#else
/*********************************/
/************ Native *************/
/*********************************/
	// Resolve platform-dependent SPI
	#if defined(AVR)
	/************** AVR  **************/
		//  This is not yet supported, but it is here for future reference.
		//  AVR-series in Arduino IDE
		#error "AVR not yet supported"

	#elif defined(USE_HAL_DRIVER)
	/*****  STM32 and stm32duino ******/

		// Include hal for supported STM32 platforms
		#include "./STM32/stm32xxyy_hal.h"

		#ifndef HAL_MODULE_ENABLED
			#error "HAL configuration incorrect"
		#endif

		#ifdef HAL_SPI_MODULE_ENABLED
			// define STM32-specific handle type for SPI
			#define ISL_SPI_ENABLED
			namespace IntroSatLib::interfaces {using SPI_HANDLE_TYPE = SPI_HandleTypeDef;}
		#elif !defined(INTROSATLIB_INTERNAL)
			#error "SPI not enabled as part of HAL"
		#endif

	/************** AMUR *************/
	// #elif defined(AMUR)
		// #error "AMUR not yet supported"
	/************ UNKNOWN ************/
	#else
	// #ifndef INTROSATLIB_INTERNAL
		#error "Unsupported system: neither AVR/ARDUINO nor USE_HAL_DRIVER defined. Please check your platform macros."
		#error "Currently supported systems are: stm32, stm32duino. AVR planned for future support."
	// #endif
	#endif
#endif

#ifdef ISL_SPI_ENABLED

#if __has_include(<array>)
#define STL_AVAILABLE
#include <array>
#endif

namespace IntroSatLib {
namespace interfaces {

class SPI final {
	SPI_HANDLE_TYPE *_hspi = 0;
public:
 	SPI(SPI_HANDLE_TYPE *hspi): _hspi(hspi) { };

// STL not available in Arduino IDE by default
#ifdef STL_AVAILABLE
 	template<size_t N>
 	ISL_StatusTypeDef transfer(const std::array<uint8_t, N> out, std::array<uint8_t, N> in)
 	{
 		return transfer(out.data(), in.data(), N);
 	}
 	template<size_t N>
 	ISL_StatusTypeDef transfer(const std::array<uint8_t, N> out, uint8_t* in)
 	{
 		return transfer(out.data(), in, N);
 	}

 	template<size_t N>
 	ISL_StatusTypeDef transfer(const uint8_t* out, std::array<uint8_t, N> in)
 	{
 		return transfer(out, in.data(), N);
 	}
#endif

 	ISL_StatusTypeDef transfer(const uint8_t* out, uint8_t* in, uint8_t len);
};

} /* namespace intefaces */
} /* namespace IntroSatLib */

#endif /* ISL_SPI_ENABLED */

#endif /* ADAPTER_SPI_H_ */
