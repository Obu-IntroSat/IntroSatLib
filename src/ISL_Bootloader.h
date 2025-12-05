#ifndef BOOTLOADER_H_
#define BOOTLOADER_H_

#include <stdint.h>

// TODO move to stm32xxyy_hal.h
// Set boot memory address depending on platform
#if defined(ARDUINO_AVR_ATmega328)
    #include "avr/eeprom.h"
    #include "Arduino.h"
    #define BOOT_ADDR 0x7e8e

#elif defined(STM32F4xx)
    #include "stm32f4xx_hal.h"
    #define BOOT_ADDR 0x1FFFF000
#elif defined(STM32F103xB)
    #include "stm32f1xx_hal.h"
    #define BOOT_ADDR 0x1FFFF000
#elif defined(STM32H750xx)
    #include "stm32h7xx_hal.h"
    #define BOOT_ADDR 0x1FF09800
#endif

// raise warn if boot address is not set
#if !defined(BOOT_ADDR) and !defined(INTROSATLIB_INTERNAL)
    // Looks like unsupported platform
	#warning "IntroSatLib::EnterBootloader() not supported by selected package"
	#warning "IntroSatLib::EnterBootloader() currently supports STM32F1xx, STM32F4xx, STM32H750xx."
	#warning "Calls to IntroSatLib::EnterBootloader() will have no effect"
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern void EnterBootloader();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* IS_BLUETOOTH_H_ */
