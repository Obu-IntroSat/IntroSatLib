#include "AK8963.h"

namespace IntroSatLib {

#ifndef ARDUINO
AK8963::AK8963(I2C_HandleTypeDef *hi2c, uint8_t address): BaseDevice(hi2c, address)
{
}
#else
AK8963::AK8963(TwoWire &hi2c, uint8_t address): BaseDevice(hi2c, address)
{
}
AK8963::AK8963(uint8_t address): BaseDevice(address)
{
}
#endif

AK8963::AK8963(const AK8963& other): BaseDevice(other)
{
}
AK8963::AK8963(AK8963&& other): BaseDevice(other)
{
}
AK8963& AK8963::operator=(const AK8963& other)
{
	if (this != &other)
	{
		this->BaseDevice::operator = (other);
	}
	return *this;
}
AK8963& AK8963::operator=(AK8963&& other)
{
	if (this != &other)
	{
		this->BaseDevice::operator =(other);
	}
	return *this;
}


uint8_t AK8963::Init()
{
	SetRegister(0x20, 0x70);
	HAL_Delay(1);
	SetRegister(0x21, 0x00);
	HAL_Delay(1);
	SetRegister(0x22, 0x00);
	HAL_Delay(1);
	SetRegister(0x23, 0x0C);
	return 0;
}

int16_t AK8963::RawX()
{
	uint8_t buf[2];
	_i2c.read(0x28, buf, 2);
	return buf[0] << 8 | buf[1];
}
int16_t AK8963::RawY()
{
	uint8_t buf[2];
	_i2c.read(0x2A, buf, 2);
	return buf[0] << 8 | buf[1];
}
int16_t AK8963::RawZ()
{
	uint8_t buf[2];
	_i2c.read(0x2C, buf, 2);
	return buf[0] << 8 | buf[1];
}

AK8963::~AK8963()
{
	BaseDevice::~BaseDevice();
}

} /* namespace IntroSatLib */
