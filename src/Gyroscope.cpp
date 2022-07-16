#include <Gyroscope.h>

namespace IntroSatLib {

#ifndef ARDUINO
	Gyroscope::Gyroscope(I2C_HandleTypeDef *hi2c, uint8_t addres)
	{
		_i2c = I2CDevice(hi2c, addres);
	}
#else
	Gyroscope::Gyroscope(TwoWire &hi2c, uint8_t addres)
	{
		_i2c = I2CDevice(hi2c, addres);
	}
#endif

Gyroscope::Gyroscope(const Gyroscope &other)
{
	*(&_i2c) = other._i2c;
	_sensivity= other._sensivity;
}
Gyroscope::Gyroscope(Gyroscope &&other)
{
	*(&_i2c) = other._i2c;
	_sensivity= other._sensivity;
}
Gyroscope& Gyroscope::operator=(const Gyroscope &other)
{
	if (this == &other)
	{
		return *this;
	}
	*(&_i2c) = other._i2c;
	_sensivity= other._sensivity;
	return *this;
}
Gyroscope& Gyroscope::operator=(Gyroscope &&other)
{
	if (this == &other)
	{
		return *this;
	}
	*(&_i2c) = other._i2c;
	_sensivity= other._sensivity;
	return *this;
}

void Gyroscope::Init(Scale sensivity, FilterBandwidth filter)
{
	SetScale(sensivity);
	SetFilter(filter);
}
void Gyroscope::Init(Scale sensivity)
{
	Init(sensivity, FilterBandwidth::F0005);
}
void Gyroscope::Init()
{
	Init(Scale::DPS0250);
}

void Gyroscope::SetScale(Scale sensivity)
{
	uint8_t reg = GetRegister(RegisterMap::GYRO_CONFIG);
	reg &= 0xFF ^ (Scale::DPS2000 << 3);
	reg |= (sensivity << 3);
	_sensivity = sensivity;
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
	float e = RawX() * (1 << _sensivity);
	return e / _rawdps;
}
float Gyroscope::Y()
{
	float e = RawY() * (1 << _sensivity);
	return e / _rawdps;
}
float Gyroscope::Z()
{
	float e = RawZ() * (1 << _sensivity);
	return e / _rawdps;
}

void Gyroscope::SetRegister(RegisterMap reg, uint8_t value)
{
	_i2c.write(reg, &value, 1);
}
uint8_t Gyroscope::GetRegister(RegisterMap reg)
{
	uint8_t value = 0;
	_i2c.read(reg, &value, 1);
	return value;
}

Gyroscope::~Gyroscope()
{
	_i2c.~I2CDevice();
}

}
