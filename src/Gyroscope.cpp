#include <Gyroscope.h>

namespace IntroSatLib {

#ifndef ARDUINO
Gyroscope::Gyroscope(I2C_HandleTypeDef *hi2c, uint8_t addres): BaseDevice(hi2c, addres)
{
}
#else
Gyroscope::Gyroscope(TwoWire &hi2c, uint8_t addres): BaseDevice(hi2c, addres)
{
}
#endif

Gyroscope::Gyroscope(const Gyroscope &other): BaseDevice(other)
{
	_sensitivity= other._sensitivity;
}
Gyroscope::Gyroscope(Gyroscope &&other): BaseDevice(other)
{
	_sensitivity= other._sensitivity;
}
Gyroscope& Gyroscope::operator=(const Gyroscope &other)
{
	if (this != &other)
	{
		this->BaseDevice::operator = (other);
		_sensitivity= other._sensitivity;
	}
	return *this;
}
Gyroscope& Gyroscope::operator=(Gyroscope &&other)
{
	if (this != &other)
	{
		this->BaseDevice::operator = (other);
		_sensitivity= other._sensitivity;
	}
	return *this;
}

uint8_t Gyroscope::Init(Scale sensitivity, FilterBandwidth filter)
{
	SetScale(sensitivity);
	SetFilter(filter);
	return 0;
}
uint8_t Gyroscope::Init(Scale sensitivity)
{
	return Init(sensitivity, FilterBandwidth::F0005);
}
uint8_t Gyroscope::Init()
{
	return Init(Scale::DPS0250);
}

void Gyroscope::SetScale(Scale sensitivity)
{
	uint8_t reg = GetRegister(RegisterMap::GYRO_CONFIG);
	reg &= 0xFF ^ (Scale::DPS2000 << 3);
	reg |= (sensitivity << 3);
	_sensitivity = sensitivity;
	SetRegister(RegisterMap::GYRO_CONFIG, reg);
}

void Gyroscope::SetFilter(FilterBandwidth filter)
{
	uint8_t reg = GetRegister(RegisterMap::GYRO_CONFIG);
	reg &= 0xFF ^ 3;
	reg |= (filter >> 3);
	SetRegister(RegisterMap::GYRO_CONFIG, reg);

	reg = GetRegister(RegisterMap::CONFIG);
	reg &= 0xFF ^ 7;
	reg |= (filter & 7);
	SetRegister(RegisterMap::CONFIG, reg);
}

int16_t Gyroscope::RawX()
{
	uint8_t buf[2];
	_i2c.read(RegisterMap::GYRO_XOUT_H, buf, 2);
	return buf[0] << 8 | buf[1];
}
int16_t Gyroscope::RawY()
{
	uint8_t buf[2];
	_i2c.read(RegisterMap::GYRO_YOUT_H, buf, 2);
	return buf[0] << 8 | buf[1];
}
int16_t Gyroscope::RawZ()
{
	uint8_t buf[2];
	_i2c.read(RegisterMap::GYRO_ZOUT_H, buf, 2);
	return buf[0] << 8 | buf[1];
}

float Gyroscope::X()
{
	float e = RawX() * (1 << _sensitivity);
	return e / _rawdps;
}
float Gyroscope::Y()
{
	float e = RawY() * (1 << _sensitivity);
	return e / _rawdps;
}
float Gyroscope::Z()
{
	float e = RawZ() * (1 << _sensitivity);
	return e / _rawdps;
}


Gyroscope::~Gyroscope()
{
	BaseDevice::~BaseDevice();
}

}
