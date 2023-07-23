/*
 * CoilFlyWheel.cpp
 *
 *  Created on: 15 окт. 2022 г.
 *      Author: Almaz
 */

#include "CoilFlyWheel.h"

namespace IntroSatLib
{

#ifndef ARDUINO
CoilFlyWheel::CoilFlyWheel(I2C_HandleTypeDef *hi2c, uint8_t address) :
		BaseFlyWheel(hi2c, address)
{
	_channel = 2;
}
#else
CoilFlyWheel::CoilFlyWheel(TwoWire &hi2c, uint8_t address): BaseFlyWheel(hi2c, address)
{
	_channel = 2;
}
#endif

CoilFlyWheel::CoilFlyWheel(const CoilFlyWheel &other) :
		BaseFlyWheel(other)
{
}

CoilFlyWheel::CoilFlyWheel(CoilFlyWheel &&other) :
		BaseFlyWheel(other)
{
}

CoilFlyWheel& CoilFlyWheel::operator=(const CoilFlyWheel &other)
{
	if (this != &other)
	{
		this->BaseFlyWheel::operator =(other);
	}
	return *this;
}

CoilFlyWheel& CoilFlyWheel::operator=(CoilFlyWheel &&other)
{
	if (this != &other)
	{
		this->BaseFlyWheel::operator =(other);
	}
	return *this;
}

CoilFlyWheel::~CoilFlyWheel()
{
	BaseFlyWheel::~BaseFlyWheel();
}

} /* namespace IntroSatLib */
