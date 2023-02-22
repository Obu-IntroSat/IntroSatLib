#include "MotorFlyWheel.h"

namespace IntroSatLib {

#ifndef ARDUINO
MotorFlyWheel::MotorFlyWheel(I2C_HandleTypeDef *hi2c, uint8_t address): BaseFlyWheel(hi2c, address)
{
	_channel = 1;
}
#else
MotorFlyWheel::MotorFlyWheel(TwoWire &hi2c, uint8_t address): BaseFlyWheel(hi2c, address)
{
	_channel = 1;
}
MotorFlyWheel::MotorFlyWheel(uint8_t address): BaseFlyWheel(address)
{
	_channel = 1;
}
#endif

MotorFlyWheel::MotorFlyWheel(const MotorFlyWheel &other): BaseFlyWheel(other)
{
}

MotorFlyWheel::MotorFlyWheel(MotorFlyWheel &&other): BaseFlyWheel(other)
{
}

MotorFlyWheel& MotorFlyWheel::operator=(const MotorFlyWheel &other)
{
	if (this != &other)
	{
		this->BaseFlyWheel::operator =(other);
	}
	return *this;
}

MotorFlyWheel& MotorFlyWheel::operator=(MotorFlyWheel &&other)
{
	if (this != &other)
	{
		this->BaseFlyWheel::operator =(other);
	}
	return *this;
}

MotorFlyWheel::~MotorFlyWheel()
{
	BaseFlyWheel::~BaseFlyWheel();
}

} /* namespace IntroSatLib */
