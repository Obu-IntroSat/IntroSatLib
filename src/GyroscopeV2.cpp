#include "GyroscopeV2.h"

namespace IntroSatLib {

#ifndef ARDUINO
GyroscopeV2::GyroscopeV2(I2C_HandleTypeDef *hi2c, uint8_t address): BaseDevice(hi2c, address)
{
}
#else
GyroscopeV2::GyroscopeV2(TwoWire &hi2c, uint8_t address): BaseDevice(hi2c, address)
{
}
GyroscopeV2::GyroscopeV2(uint8_t address): BaseDevice(address)
{
}
#endif

GyroscopeV2::GyroscopeV2(const GyroscopeV2 &other): BaseDevice(other)
{
	_sensitivity= other._sensitivity;
}
GyroscopeV2::GyroscopeV2(GyroscopeV2 &&other): BaseDevice(other)
{
	_sensitivity= other._sensitivity;
}
GyroscopeV2& GyroscopeV2::operator=(const GyroscopeV2 &other)
{
	if (this != &other)
	{
		this->BaseDevice::operator = (other);
		_sensitivity= other._sensitivity;
	}
	return *this;
}
GyroscopeV2& GyroscopeV2::operator=(GyroscopeV2 &&other)
{
	if (this != &other)
	{
		this->BaseDevice::operator = (other);
		_sensitivity= other._sensitivity;
	}
	return *this;
}

uint8_t GyroscopeV2::Init(Scale sensitivity)
{
	SetScale(sensitivity);
	return 0;
}
uint8_t GyroscopeV2::Init()
{
	return Init(Scale::DPS0250);
}

void GyroscopeV2::SetScale(Scale sensitivity)
{
	uint8_t reg = GetRegister(RegisterMap::GYRO_CONFIG);
	reg &= 0xFF ^ (Scale::DPS2000 << 2);
	reg |= (sensitivity << 2);
	_sensitivity = sensitivity;
	SetRegister(RegisterMap::GYRO_CONFIG, reg);
}

int16_t GyroscopeV2::RawX()
{
	uint8_t buf[2];
	_i2c.read(RegisterMap::GYRO_XOUT_H, buf, 2);
	return buf[0] << 8 | buf[1];
}
int16_t GyroscopeV2::RawY()
{
	uint8_t buf[2];
	_i2c.read(RegisterMap::GYRO_YOUT_H, buf, 2);
	return buf[0] << 8 | buf[1];
}
int16_t GyroscopeV2::RawZ()
{
	uint8_t buf[2];
	_i2c.read(RegisterMap::GYRO_ZOUT_H, buf, 2);
	return buf[0] << 8 | buf[1];
}

float GyroscopeV2::X()
{
	float e = RawX() * (1 << _sensitivity);
	return e * _rawdps;
}
float GyroscopeV2::Y()
{
	float e = RawY() * (1 << _sensitivity);
	return e * _rawdps;
}
float GyroscopeV2::Z()
{
	float e = RawZ() * (1 << _sensitivity);
	return e * _rawdps;
}


GyroscopeV2::~GyroscopeV2()
{
	BaseDevice::~BaseDevice();
}

}
