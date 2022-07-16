/*
 * Accelerometer.cpp
 *
 *  Created on: Jul 12, 2022
 *      Author: Almaz
 */

#include <Accelerometer.h>

namespace IntroSatLib {

#ifndef ARDUINO
Accelerometer::Accelerometer(I2C_HandleTypeDef *hi2c, uint8_t addres)
{
	_i2c = I2CDevice(hi2c, addres);
}
#else
Accelerometer::Accelerometer(TwoWire &hi2c, uint8_t addres)
{
	_i2c = I2CDevice(hi2c, addres);
}
#endif

Accelerometer::Accelerometer(const Accelerometer& other)
{
	*(&_i2c) = other._i2c;
	_sensivity= other._sensivity;
}

Accelerometer::Accelerometer(Accelerometer&& other)
{
	*(&_i2c) = other._i2c;
	_sensivity= other._sensivity;
}

Accelerometer& Accelerometer::operator=(const Accelerometer& other)
{
	if (this == &other)
	{
		return *this;
	}
	*(&_i2c) = other._i2c;
	_sensivity= other._sensivity;
	return *this;
}

Accelerometer& Accelerometer::operator=(Accelerometer&& other)
{
	if (this == &other)
	{
		return *this;
	}
	*(&_i2c) = other._i2c;
	_sensivity= other._sensivity;
	return *this;
}

void Accelerometer::Init(MaxAccelScale sensivity, Bandwidth filter)
{
	SetScale(sensivity);
	SetFilter(filter);
}

void Accelerometer::Init(MaxAccelScale sensivity)
{
	Init(sensivity, Bandwidth::F0020);
}

void Accelerometer::Init()
{
	Init(_sensivity);
}

void Accelerometer::SetRegister(RegisterMap reg, uint8_t value)
{
	_i2c.write(reg, &value, 1);
}

uint8_t Accelerometer::GetRegister(RegisterMap reg)
{
	uint8_t value = 0;
	_i2c.read(reg, &value, 1);
	return value;
}

void Accelerometer::SetScale(MaxAccelScale sensivity)
{
	uint8_t reg = GetRegister(RegisterMap::ACCEL_CONFIG);
	reg &= 0xFF ^ (MaxAccelScale::sixteenG << 3);
	reg |= (sensivity << 3);
	_sensivity = sensivity;
	SetRegister(RegisterMap::ACCEL_CONFIG, reg);
}

void Accelerometer::SetFilter(Bandwidth filter)
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
	float e = RawX() * (1 << _sensivity);
	return e / _rawg;
}
float Accelerometer::Y()
{
	float e = RawY() * (1 << _sensivity);
	return e / _rawg;
}
float Accelerometer::Z()
{
	float e = RawZ() * (1 << _sensivity);
	return e / _rawg;
}

Accelerometer::~Accelerometer()
{
	_i2c.~I2CDevice();
}

} /* namespace IntroSatLib */
