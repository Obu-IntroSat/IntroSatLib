#ifndef _IS_BLUETOOTH_H_
#define _IS_BLUETOOTH_H_

#include <stdint.h>

#ifndef ARDUINO
#include "stm32f1xx_hal.h"
#else
#include <Arduino.h>
#endif /* ARDUINO */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

extern void enter_bootloader();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _IS_BLUETOOTH_H_ */
