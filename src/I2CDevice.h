/*
 * I2CDevice.h
 *
 *  Created on: Jul 10, 2022
 *      Author: Almaz
 */

#ifndef I2CDEVICE_H_
#define I2CDEVICE_H_

#include <stdint.h>
#ifdef ARDUINO
	#include "Arduino.h"
	#include "Wire.h"
#endif

#include "stm32f1xx_hal.h"
#ifdef HAL_I2C_MODULE_ENABLED
	#include "stm32f1xx_hal_i2c.h"
#endif



namespace IntroSatLib {

enum class I2CSpeed
{
	Standard = 0,
	Fast = 1
};

class I2CDevice {
private:
	I2CSpeed _speed = I2CSpeed::Standard;
	uint8_t _address = 0;
	I2C_HandleTypeDef *_hi2c = 0;
public:
	I2CDevice();
#ifndef ARDUINO
	I2CDevice(I2C_HandleTypeDef *hi2c, uint8_t address);
	I2CDevice(I2C_HandleTypeDef *hi2c, uint8_t address, I2CSpeed speed);
#else
	I2CDevice(TwoWire &hi2c, uint8_t address);
	I2CDevice(TwoWire &hi2c, uint8_t address, I2CSpeed speed);
#endif
	I2CDevice(const I2CDevice& other);
	I2CDevice(I2CDevice&& other);
	I2CDevice& operator=(const I2CDevice& other);
	I2CDevice& operator=(I2CDevice&& other);
	HAL_StatusTypeDef read(uint8_t* Data, uint8_t Nbytes);
	HAL_StatusTypeDef read(uint8_t Register, uint8_t* Data, uint8_t Nbytes);
	HAL_StatusTypeDef write(uint8_t* Data, uint8_t Nbytes);
	HAL_StatusTypeDef write(uint8_t Register, uint8_t* Data, uint8_t Nbytes);
	~I2CDevice();
};

} /* namespace IntroSatLib */

#endif /* I2CDEVICE_H_ */
