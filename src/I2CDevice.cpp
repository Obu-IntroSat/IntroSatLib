/*
 * I2CDevice.cpp
 *
 *  Created on: Jul 10, 2022
 *      Author: Almaz
 */

#include "I2CDevice.h"

namespace IntroSatLib
{

#ifndef ARDUINO
I2CDevice::I2CDevice(I2C_HandleTypeDef *hi2c, uint8_t address)
{
	_hi2c = hi2c;
	if (address > 127)
		address = 127;
	_address = address << 1;
}

I2CDevice::I2CDevice(I2C_HandleTypeDef *hi2c, uint8_t address, I2CSpeed speed)
{
	_hi2c = hi2c;
	if (address > 127)
		address = 127;
	_address = address << 1;
	_speed = speed;
}
#else
I2CDevice::I2CDevice(TwoWire &hi2c, uint8_t address)
{
	_hi2c = hi2c.getHandle();
	if (address > 127) address = 127;
	_address = address << 1;
}

I2CDevice::I2CDevice(TwoWire &hi2c, uint8_t address, I2CSpeed speed)
{
	_hi2c = hi2c.getHandle();
	if (address > 127) address = 127;
	_address = address << 1;
	_speed = speed;
}
#endif

I2CDevice::I2CDevice(const I2CDevice &other)
{
	_address = other._address;
	_hi2c = other._hi2c;
	_speed = other._speed;
}

I2CDevice::I2CDevice(I2CDevice &&other)
{
	_address = other._address;
	_hi2c = other._hi2c;
	_speed = other._speed;
}

I2CDevice& I2CDevice::operator=(const I2CDevice &other)
{
	if (this == &other)
	{
		return *this;
	}
	_address = other._address;
	_hi2c = other._hi2c;
	_speed = other._speed;
	return *this;
}

I2CDevice& I2CDevice::operator=(I2CDevice &&other)
{
	if (this == &other)
	{
		return *this;
	}
	_address = other._address;
	_hi2c = other._hi2c;
	_speed = other._speed;
	return *this;
}
HAL_StatusTypeDef I2CDevice::isReady()
{
	return HAL_I2C_IsDeviceReady(_hi2c, _address, 1, 1000);
}

HAL_StatusTypeDef I2CDevice::read(uint8_t *Data, uint8_t Nbytes)
{
	if (!_hi2c)
		return HAL_StatusTypeDef::HAL_ERROR;
	return HAL_I2C_Master_Receive(_hi2c, _address, Data, Nbytes, 1000);
}
HAL_StatusTypeDef I2CDevice::read(uint8_t Register, uint8_t *Data,
		uint8_t Nbytes)
{
	if (!_hi2c)
		return HAL_StatusTypeDef::HAL_ERROR;
	return HAL_I2C_Mem_Read(_hi2c, _address, Register, I2C_MEMADD_SIZE_8BIT,
			Data, Nbytes, 5000);
}
HAL_StatusTypeDef I2CDevice::write(uint8_t *Data, uint8_t Nbytes)
{
	if (!_hi2c)
		return HAL_StatusTypeDef::HAL_ERROR;
	return HAL_I2C_Master_Transmit(_hi2c, _address, Data, Nbytes, 1000);
}
HAL_StatusTypeDef I2CDevice::write(uint8_t Register, uint8_t *Data,
		uint8_t Nbytes)
{
	if (!_hi2c)
		return HAL_StatusTypeDef::HAL_ERROR;
	return HAL_I2C_Mem_Write(_hi2c, _address, Register, I2C_MEMADD_SIZE_8BIT,
			Data, Nbytes, 1000);
}

I2CDevice::~I2CDevice()
{
}

} /* namespace IntroSatLib */
