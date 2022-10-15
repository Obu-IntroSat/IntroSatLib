/*
 * BaseDevice1.cpp
 *
 *  Created on: 5 окт. 2022 г.
 *      Author: Almaz
 */

#include <BaseDevice.h>

namespace IntroSatLib {

void BaseDevice::SetRegister(uint8_t reg, uint8_t value)
{
	_i2c.write(reg, &value, 1);
}

uint8_t BaseDevice::GetRegister(uint8_t reg)
{
	uint8_t value = 0;
	_i2c.read(reg, &value, 1);
	return value;
}

void BaseDevice::SetBitRegister(uint8_t reg, uint8_t bit)
{
	if (bit > 7) bit = 7;
	uint8_t tmp = GetRegister(reg);
	tmp |= (1 << bit);
	SetRegister(reg, tmp);
}
void BaseDevice::ResetBitRegister(uint8_t reg, uint8_t bit)
{
	if (bit > 7) bit = 7;
	uint8_t tmp = GetRegister(reg);
	tmp &= ~(1 << bit);
	SetRegister(reg, tmp);
}
void BaseDevice::BitRegister(uint8_t reg, uint8_t bit, uint8_t value)
{
	if (value)
	{
		SetBitRegister(reg, bit);
	}
	else
	{
		ResetBitRegister(reg, bit);
	}
}

#ifndef ARDUINO
BaseDevice::BaseDevice(I2C_HandleTypeDef *hi2c, uint8_t address): _i2c(* new I2CDevice(hi2c, address))
{
}
#else
BaseDevice::BaseDevice(TwoWire &hi2c, uint8_t address): _i2c(* new I2CDevice(hi2c, address))
{
};
#endif

uint8_t BaseDevice::Init()
{
	return _i2c.isReady();
}

BaseDevice::BaseDevice(const BaseDevice& other): _i2c(other._i2c)
{
}
BaseDevice::BaseDevice(BaseDevice&& other): _i2c(other._i2c)
{
}
BaseDevice& BaseDevice::operator=(const BaseDevice& other)
{
	if (this != &other)
	{
		_i2c = other._i2c;
//		delete &other._i2c;
	}
	return *this;
}
BaseDevice& BaseDevice::operator=(BaseDevice&& other)
{
	if (this != &other)
	{
		_i2c = other._i2c;
//		delete &other._i2c;
	}
	return *this;
}

BaseDevice::~BaseDevice()
{
	delete &_i2c;
}

} /* namespace IntroSatLib */
