#include "MotorFlyWheel.h"

namespace IntroSatLib {

MotorFlyWheel::MotorFlyWheel(const interfaces::I2C &i2c, uint8_t address): BaseFlyWheel(i2c, address)
{
	_channel = 1;
}

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

MotorFlyWheel::~MotorFlyWheel() { }

} /* namespace IntroSatLib */
