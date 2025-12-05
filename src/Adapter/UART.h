#ifndef ADAPTER_UART_H_
#define ADAPTER_UART_H_

// Include general Library definitions
#include "IntroSatLib_def.h"

#ifdef ARDUINO
/*********************************/
/********** Arduino IDE **********/
/*********************************/
	#include "Arduino.h"
	#include <HardwareSerial.h>
	namespace IntroSatLib::interfaces {using UART_HANDLE_TYPE = HardwareSerial;}

#else
/*********************************/
/************ Native *************/
/*********************************/
	// Resolve platform-dependent UART
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

		#ifdef HAL_UART_MODULE_ENABLED
			// define STM32-specific handle type for UART
			namespace IntroSatLib::interfaces {using UART_HANDLE_TYPE = UART_HandleTypeDef;}
		#elif !defined(INTROSATLIB_INTERNAL)
			#error "UART not enabled as part of HAL"
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

// #ifdef UART_HANDLE_TYPE
#define UART_ENABLED
#include "Adapter/System.h"

#define ASSERT_UART_HAVE() \
if(!_huart) { \
logText("No uart handle"); \
return ISL_StatusTypeDef::ISL_ERROR; \
}

#if LOGDATA
    #define LOG_UART_BUFFER(Sep, Data, Nbytes) { \
        logText(" - "); \
        for(uint8_t i = 0; i < Nbytes; i++) { \
        logHEX(Data[i]); \
        if (i != (Nbytes - 1)) logText(Sep); \
        } \
        }
#else
    #define LOG_UART_BUFFER(Sep, Data, Nbytes)
#endif


namespace IntroSatLib {
namespace interfaces {

class UART final {
	UART_HANDLE_TYPE *_huart = 0;
public:
 	UART(UART_HANDLE_TYPE *huart): _huart(huart) { };
	UART(UART_HANDLE_TYPE &huart): _huart(&huart) { };

	uint8_t available();
	ISL_StatusTypeDef receive(uint8_t* rx_buf, uint16_t count, uint16_t timeout = 1000);
	ISL_StatusTypeDef transmit(uint8_t* tx_buf, uint16_t count, uint16_t timeout = 1000);
	ISL_StatusTypeDef transmitAsync(uint8_t* tx_buf, uint16_t count);

	// В Arduino API есть аналоги - find() и findUntil(). Делают похожую вещь, маловероятно, что сильно оптимальнее. 
	// https://docs.arduino.cc/language-reference/en/functions/communication/serial/find/
	ISL_StatusTypeDef waitFor (const uint8_t symbol, uint16_t timeout = 0xFFFF) {
		uint32_t nowTime = system::GetTick();
		uint32_t finishTime = nowTime + timeout;

		uint8_t temp;
		
		ISL_StatusTypeDef status = ISL_OK;

		while (nowTime < finishTime) {
			status = receive(&temp, 1, finishTime - nowTime);
			if (status != ISL_OK) return status;
			
			if (temp == symbol) {
				return ISL_OK;
			}
			nowTime = system::GetTick();
		}
		return ISL_TIMEOUT;

	}

	ISL_StatusTypeDef waitForPattern(uint8_t* pattern, uint8_t patternLength, uint16_t timeout = 0xFFFF) {
		uint32_t nowTime = system::GetTick();
		uint32_t finishTime = nowTime + timeout;
		uint8_t* tempBuf = new uint8_t[patternLength];
		uint8_t matched = 0;
		
		ISL_StatusTypeDef status = ISL_StatusTypeDef::ISL_OK;

		while (matched < patternLength) {
			nowTime = system::GetTick();
			// if (nowTime > finishTime) return ISL_TIMEOUT; // Вроде необязательно тут...

			status = receive(tempBuf + matched, 1, finishTime - system::GetTick());
			
			if (status != ISL_OK) return status;
			
			if (tempBuf[matched] == pattern[matched])
				matched++;
			else 
				matched = 0;
		}
		return status;
	}
};

} /* namespace intefaces */
} /* namespace IntroSatLib */

// #endif /* UART_HANDLE_TYPE */

#endif /* ADAPTER_UART_H_ */
