#include "Accelerometer.h"

namespace IntroSatLib {

#ifndef ARDUINO
Accelerometer::Accelerometer(I2C_HandleTypeDef *hi2c, uint8_t address): BaseDevice(hi2c, address)
{
}
#else
Accelerometer::Accelerometer(TwoWire &hi2c, uint8_t address): BaseDevice(hi2c, address)
{
}
Accelerometer::Accelerometer(uint8_t address): BaseDevice(address)
{
}
#endif

Accelerometer::Accelerometer(const Accelerometer& other): BaseDevice(other)
{
	_sensitivity= other._sensitivity;
}
Accelerometer::Accelerometer(Accelerometer&& other): BaseDevice(other)
{
	_sensitivity= other._sensitivity;
}
Accelerometer& Accelerometer::operator=(const Accelerometer& other)
{
	if (this != &other)
	{
		this->BaseDevice::operator = (other);
		_sensitivity= other._sensitivity;
	}
	return *this;
}
Accelerometer& Accelerometer::operator=(Accelerometer&& other)
{
	if (this != &other)
	{
		this->BaseDevice::operator =(other);
		_sensitivity= other._sensitivity;
	}
	return *this;
}

uint8_t Accelerometer::Init(Scale sensitivity, FilterBandwidth filter)
{
	SetRegister(0x37, 0x02);
	SetScale(sensitivity);
	SetFilter(filter);
	return _i2c.isReady(); // Use existing method instead of returning 0
	// return 0;
}
uint8_t Accelerometer::Init(Scale sensitivity)
{
	return Init(sensitivity, FilterBandwidth::F0021);
}
uint8_t Accelerometer::Init()
{
	return Init(Scale::twoG);
}


void Accelerometer::SetScale(Scale sensitivity)
{
	uint8_t reg = GetRegister(RegisterMap::ACCEL_CONFIG);
	reg &= 0xFF ^ (Scale::sixteenG << 3);
	reg |= (sensitivity << 3);
	_sensitivity = sensitivity;
	SetRegister(RegisterMap::ACCEL_CONFIG, reg);
}

void Accelerometer::SetFilter(FilterBandwidth filter)
{
	SetRegister(RegisterMap::ACCEL_CONFIG_2, filter);
}


int16_t Accelerometer::RawX()
{
	uint8_t buf[2];
	_i2c.read(RegisterMap::ACCEL_XOUT_H, buf, 2);
	return buf[0] << 8 | buf[1];
}
int16_t Accelerometer::RawY()
{
	uint8_t buf[2];
	_i2c.read(RegisterMap::ACCEL_YOUT_H, buf, 2);
	return buf[0] << 8 | buf[1];
}
int16_t Accelerometer::RawZ()
{
	uint8_t buf[2];
	_i2c.read(RegisterMap::ACCEL_ZOUT_H, buf, 2);
	return buf[0] << 8 | buf[1];
}

float Accelerometer::X()
{
	float e = RawX() * (1 << _sensitivity);
	return e / _rawg;
}
float Accelerometer::Y()
{
	float e = RawY() * (1 << _sensitivity);
	return e / _rawg;
}
float Accelerometer::Z()
{
	float e = RawZ() * (1 << _sensitivity);
	return e / _rawg;
}

Accelerometer::~Accelerometer()
{
	BaseDevice::~BaseDevice();
}

}
