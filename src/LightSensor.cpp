/*
 * LightSensor.cpp
 *
 *  Created on: 24 июл. 2022 г.
 *      Author: Almaz
 */

#include "LightSensor.h"

namespace IntroSatLib {

#ifndef ARDUINO
LightSensor::LightSensor(I2C_HandleTypeDef *hi2c, uint8_t address): BaseDevice(hi2c, address)
{
}
#else
LightSensor::LightSensor(TwoWire &hi2c, uint8_t address): BaseDevice(hi2c, address)
{
}
#endif

LightSensor::LightSensor(const LightSensor &other): BaseDevice(other)
{
}

LightSensor::LightSensor(LightSensor &&other): BaseDevice(other)
{
}

LightSensor& LightSensor::operator=(const LightSensor &other)
{
	if (this != &other)
	{
		this->BaseDevice::operator =(other);
	}
	return *this;
}

LightSensor& LightSensor::operator=(LightSensor &&other)
{
	if (this != &other)
	{
		this->BaseDevice::operator =(other);
	}
	return *this;
}

uint8_t LightSensor::Init()
{
	return 0;
}

int16_t LightSensor::GetLight()
{
	static uint8_t buf[2];
	_i2c.write(buf, 1);
	_i2c.read(buf, 2);
	return buf[1] << 8 | buf[0];
}

LightSensor::~LightSensor()
{
	BaseDevice::~BaseDevice();
}

} /* namespace IntroSatLib */
