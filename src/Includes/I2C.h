#ifndef INCLUDES_I2C_H_
#define INCLUDES_I2C_H_

#include "./Base.h"

#ifdef INTROSAT_USE_HAL
#ifdef HAL_I2C_MODULE_ENABLED
#define INTROSAT_HAVE_I2C
#include "stm32f1xx_hal_i2c.h"
#endif /* HAL_I2C_MODULE_ENABLED */
#endif /* INTROSAT_USE_HAL */

#ifdef INTROSAT_USE_ARDUINO
	#define INTROSAT_HAVE_I2C
	#include "Wire.h"
#endif /* INTROSAT_USE_ARDUINO */

#endif /* INCLUDES_I2C_H_ */
