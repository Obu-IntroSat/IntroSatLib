#include "BaseFlyWheel.h"

namespace IntroSatLib {

#ifndef ARDUINO
BaseFlyWheel::BaseFlyWheel(I2C_HandleTypeDef *hi2c, uint8_t address): BaseDevice(hi2c, address)
{

}
#else
BaseFlyWheel::BaseFlyWheel(TwoWire &hi2c, uint8_t address): BaseDevice(hi2c, address)
{

}
BaseFlyWheel::BaseFlyWheel(uint8_t address): BaseDevice(address)
{

}
#endif

uint8_t BaseFlyWheel::Init()
{
	_version = GetRegister(RegisterMap::Version);

	if (_version < 2)
	{
		return 1;
	}

	return 0;
}

void BaseFlyWheel::DirectMode(uint8_t directMode)
{
	if(_channel == 1)
	{
		BitRegister(RegisterMap::Config, 1, directMode);
	}
	if(_channel == 2)
	{
		BitRegister(RegisterMap::Config, 0, directMode);
	}
}
uint8_t BaseFlyWheel::DirectMode()
{
	uint8_t result = 0;
	if(_channel == 1)
	{
		result = GetRegister(RegisterMap::Config) & (1 << 1);
	}
	if(_channel == 2)
	{
		result = GetRegister(RegisterMap::Config) & (1 << 0);
	}
	return result ? 1 : 0;
}

void BaseFlyWheel::SilentMode(uint8_t silentMode)
{
	if(_channel == 1)
	{
		BitRegister(RegisterMap::Config, 3, silentMode);
	}
	if(_channel == 2)
	{
		BitRegister(RegisterMap::Config, 2, silentMode);
	}
}
uint8_t BaseFlyWheel::SilentMode()
{
	uint8_t result = 0;
	if(_channel == 1)
	{
		result = GetRegister(RegisterMap::Config) & (1 << 3);
	}
	if(_channel == 2)
	{
		result = GetRegister(RegisterMap::Config) & (1 << 2);
	}
	return result ? 1 : 0;
}

void BaseFlyWheel::MinForceMode(uint8_t minForceMode)
{
	if(_channel == 1)
	{
		BitRegister(RegisterMap::Config, 7, minForceMode);
	}
	if(_channel == 2)
	{
		BitRegister(RegisterMap::Config, 6, minForceMode);
	}
}
uint8_t BaseFlyWheel::MinForceMode()
{
	uint8_t result = 0;
	if(_channel == 1)
	{
		result = GetRegister(RegisterMap::Config) & (1 << 7);
	}
	if(_channel == 2)
	{
		result = GetRegister(RegisterMap::Config) & (1 << 6);
	}
	return result ? 1 : 0;
}

void BaseFlyWheel::ReverseMode(uint8_t reverceMode)
{
	if(_channel == 1)
	{
		BitRegister(RegisterMap::Config, 5, reverceMode);
	}
	if(_channel == 2)
	{
		BitRegister(RegisterMap::Config, 4, reverceMode);
	}
}
uint8_t BaseFlyWheel::ReverseMode()
{
	uint8_t result = 0;
	if(_channel == 1)
	{
		result = GetRegister(RegisterMap::Config) & (1 << 5);
	}
	if(_channel == 2)
	{
		result = GetRegister(RegisterMap::Config) & (1 << 4);
	}
	return result ? 1 : 0;
}

void BaseFlyWheel::NeedSpeed(int16_t needSpeed)
{
	if(_channel == 1)
	{
		_i2c.write(RegisterMap::Need_F_ChannelSpeed, (uint8_t*)&needSpeed, 2);
	}
	if(_channel == 2)
	{
		_i2c.write(RegisterMap::Need_S_ChannelSpeed, (uint8_t*)&needSpeed, 2);
	}
}
int16_t BaseFlyWheel::NeedSpeed()
{
	int16_t result = 0;
	if(_channel == 1)
	{
		_i2c.read(RegisterMap::Need_F_ChannelSpeed, (uint8_t*)&result, 2);
	}
	if(_channel == 2)
	{
		_i2c.read(RegisterMap::Need_S_ChannelSpeed, (uint8_t*)&result, 2);
	}
	return result;
}

int16_t BaseFlyWheel::CurrentSpeed()
{
	int16_t result = 0;
	if(_channel == 1)
	{
		_i2c.read(RegisterMap::Current_F_ChannelSpeed, (uint8_t*)&result, 2);
	}
	if(_channel == 2)
	{
		_i2c.read(RegisterMap::Current_S_ChannelSpeed, (uint8_t*)&result, 2);
	}
	return result;
}

void BaseFlyWheel::MaxAbsSpeed(uint16_t maxAbsSpeed)
{
	if(_channel == 1)
	{
		_i2c.write(RegisterMap::Need_F_ChannelSpeed, (uint8_t*)&maxAbsSpeed, 2);
	}
	if(_channel == 2)
	{
		_i2c.write(RegisterMap::Need_S_ChannelSpeed, (uint8_t*)&maxAbsSpeed, 2);
	}
}
uint16_t BaseFlyWheel::MaxAbsSpeed()
{
	int16_t result = 0;
	if(_channel == 1)
	{
		_i2c.read(RegisterMap::F_MaxAbsSpeed, (uint8_t*)&result, 2);
	}
	if(_channel == 2)
	{
		_i2c.read(RegisterMap::S_MaxAbsSpeed, (uint8_t*)&result, 2);
	}
	return result;
}

void BaseFlyWheel::MinForce(uint16_t minForce)
{
	if(_channel == 1)
	{
		_i2c.write(RegisterMap::Need_F_ChannelSpeed, (uint8_t*)&minForce, 2);
	}
	if(_channel == 2)
	{
		_i2c.write(RegisterMap::Need_S_ChannelSpeed, (uint8_t*)&minForce, 2);
	}
}
uint16_t BaseFlyWheel::MinForce()
{
	int16_t result = 0;
	if(_channel == 1)
	{
		_i2c.read(RegisterMap::F_Min_Force, (uint8_t*)&result, 2);
	}
	if(_channel == 2)
	{
		_i2c.read(RegisterMap::S_Min_Force, (uint8_t*)&result, 2);
	}
	return result;
}

void BaseFlyWheel::PID_P(float p)
{
	if(_channel == 1)
	{
		_i2c.write(RegisterMap::F_P, (uint8_t*)&p, 4);
	}
	if(_channel == 2)
	{
		_i2c.write(RegisterMap::S_P, (uint8_t*)&p, 4);
	}
}
float BaseFlyWheel::PID_P()
{
	float result = 0;
	if(_channel == 1)
	{
		_i2c.read(RegisterMap::F_P, (uint8_t*)&result, 4);
	}
	if(_channel == 2)
	{
		_i2c.read(RegisterMap::S_P, (uint8_t*)&result, 4);
	}
	return result;
}

void BaseFlyWheel::PID_I(float i)
{
	if(_channel == 1)
	{
		_i2c.write(RegisterMap::F_I, (uint8_t*)&i, 4);
	}
	if(_channel == 2)
	{
		_i2c.write(RegisterMap::S_I, (uint8_t*)&i, 4);
	}
}
float BaseFlyWheel::PID_I()
{
	float result = 0;
	if(_channel == 1)
	{
		_i2c.read(RegisterMap::F_I, (uint8_t*)&result, 4);
	}
	if(_channel == 2)
	{
		_i2c.read(RegisterMap::S_I, (uint8_t*)&result, 4);
	}
	return result;
}

void BaseFlyWheel::PID_D(float d)
{
	if(_channel == 1)
	{
		_i2c.write(RegisterMap::F_D, (uint8_t*)&d, 4);
	}
	if(_channel == 2)
	{
		_i2c.write(RegisterMap::S_D, (uint8_t*)&d, 4);
	}
}
float BaseFlyWheel::PID_D()
{
	float result = 0;
	if(_channel == 1)
	{
		_i2c.read(RegisterMap::F_D, (uint8_t*)&result, 4);
	}
	if(_channel == 2)
	{
		_i2c.read(RegisterMap::S_D, (uint8_t*)&result, 4);
	}
	return result;
}

BaseFlyWheel::BaseFlyWheel(const BaseFlyWheel &other): BaseDevice(other)
{
	_version = other._version;
	_channel = other._channel;
}

BaseFlyWheel::BaseFlyWheel(BaseFlyWheel &&other): BaseDevice(other)
{
	_version = other._version;
	_channel = other._channel;
}

BaseFlyWheel& BaseFlyWheel::operator=(const BaseFlyWheel &other)
{
	if (this != &other)
	{
		this->BaseDevice::operator =(other);
		_version = other._version;
		_channel = other._channel;
	}
	return *this;
}

BaseFlyWheel& BaseFlyWheel::operator=(BaseFlyWheel &&other)
{
	if (this != &other)
	{
		this->BaseDevice::operator =(other);
		_version = other._version;
		_channel = other._channel;
	}
	return *this;
}

BaseFlyWheel::~BaseFlyWheel() { }

} /* namespace IntroSatLib */
