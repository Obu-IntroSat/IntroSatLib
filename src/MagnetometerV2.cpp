#include "MagnetometerV2.h"

namespace IntroSatLib {

#ifndef ARDUINO
MagnetometerV2::MagnetometerV2(I2C_HandleTypeDef *hi2c, uint8_t address): BaseDevice(hi2c, address)
{
}
#else
MagnetometerV2::MagnetometerV2(TwoWire &hi2c, uint8_t address): BaseDevice(hi2c, address)
{
}
MagnetometerV2::MagnetometerV2(uint8_t address): BaseDevice(address)
{
}
#endif

MagnetometerV2::MagnetometerV2(const MagnetometerV2& other): BaseDevice(other)
{
}
MagnetometerV2::MagnetometerV2(MagnetometerV2&& other): BaseDevice(other)
{
}
MagnetometerV2& MagnetometerV2::operator=(const MagnetometerV2& other)
{
	if (this != &other)
	{
		this->BaseDevice::operator = (other);
	}
	return *this;
}
MagnetometerV2& MagnetometerV2::operator=(MagnetometerV2&& other)
{
	if (this != &other)
	{
		this->BaseDevice::operator =(other);
	}
	return *this;
}


uint8_t MagnetometerV2::Init()
{
	SetRegister(0x20, 0x70);
	HAL_Delay(1);
	SetRegister(0x21, 0x60);
	HAL_Delay(1);
	SetRegister(0x22, 0x00);
	HAL_Delay(1);
	SetRegister(0x23, 0x0C);
	return 0;
}

int16_t MagnetometerV2::RawX()
{
	uint8_t buf[2];
	_i2c.read(0x28, buf, 2);
	return buf[1] << 8 | buf[0];
}
int16_t MagnetometerV2::RawY()
{
	uint8_t buf[2];
	_i2c.read(0x2A, buf, 2);
	return buf[1] << 8 | buf[0];
}
int16_t MagnetometerV2::RawZ()
{
	uint8_t buf[2];
	_i2c.read(0x2C, buf, 2);
	return buf[1] << 8 | buf[0];
}

Quaternion<float> MagnetometerV2::GetQuaternion()
{
	std::array<float, 3> buf;
	buf[0] = 0;
	buf[1] = 0;
	buf[2] = std::atan2(RawY(), RawX());
	return from_euler(buf);
}

MagnetometerV2::~MagnetometerV2()
{
	BaseDevice::~BaseDevice();
}

} /* namespace IntroSatLib */
