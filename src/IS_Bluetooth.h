#ifndef IS_BLUETOOTH_H_
#define IS_BLUETOOTH_H_

#include <stdint.h>

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

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern void enter_bootloader();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* IS_BLUETOOTH_H_ */
