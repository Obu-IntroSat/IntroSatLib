#include "CoilFlyWheel.h"

namespace IntroSatLib {

CoilFlyWheel::CoilFlyWheel(const interfaces::I2C &i2c, uint8_t address): BaseFlyWheel(i2c, address)
{
	_channel = 2;
}

CoilFlyWheel::CoilFlyWheel(const CoilFlyWheel &other): BaseFlyWheel(other)
{
}

CoilFlyWheel::CoilFlyWheel(CoilFlyWheel &&other): BaseFlyWheel(other)
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

CoilFlyWheel::~CoilFlyWheel() { }

} /* namespace IntroSatLib */
