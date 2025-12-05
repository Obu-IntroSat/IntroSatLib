#include "BaseFlyWheel.h"

namespace IntroSatLib {

BaseFlyWheel::BaseFlyWheel(const interfaces::I2C &i2c, uint8_t address): I2CDevice(new interfaces::I2C(i2c), address)
{

}


ISL_StatusTypeDef BaseFlyWheel::Init()
{
	RETURN_STATUS_IF_NOT_OK_SILENT(ReadRegisterI2C(RegisterMap::Version, &_version));

	if (_version < 2)
	{
		return ISL_StatusTypeDef::ISL_ERROR;
	}

	return ISL_StatusTypeDef::ISL_OK;
}

ISL_StatusTypeDef BaseFlyWheel::DirectMode(uint8_t directMode)
{
	if(_channel == 1)
	{
		return BitRegisterI2C(RegisterMap::Config, 1, directMode);
	}
	if(_channel == 2)
	{
		return BitRegisterI2C(RegisterMap::Config, 0, directMode);
	}
	return ISL_StatusTypeDef::ISL_ERROR;
}
uint8_t BaseFlyWheel::DirectMode()
{
	uint8_t result = 0;
	if(_channel == 1)
	{
		result = GetRegisterI2C(RegisterMap::Config) & (1 << 1);
	}
	if(_channel == 2)
	{
		result = GetRegisterI2C(RegisterMap::Config) & (1 << 0);
	}
	return result ? 1 : 0;
}

ISL_StatusTypeDef BaseFlyWheel::SilentMode(uint8_t silentMode)
{
	if(_channel == 1)
	{
		return BitRegisterI2C(RegisterMap::Config, 3, silentMode);
	}
	if(_channel == 2)
	{
		return BitRegisterI2C(RegisterMap::Config, 2, silentMode);
	}
	return ISL_StatusTypeDef::ISL_ERROR;
}
uint8_t BaseFlyWheel::SilentMode()
{
	uint8_t result = 0;
	if(_channel == 1)
	{
		result = GetRegisterI2C(RegisterMap::Config) & (1 << 3);
	}
	if(_channel == 2)
	{
		result = GetRegisterI2C(RegisterMap::Config) & (1 << 2);
	}
	return result ? 1 : 0;
}

ISL_StatusTypeDef BaseFlyWheel::MinForceMode(uint8_t minForceMode)
{
	if(_channel == 1)
	{
		return BitRegisterI2C(RegisterMap::Config, 7, minForceMode);
	}
	if(_channel == 2)
	{
		return BitRegisterI2C(RegisterMap::Config, 6, minForceMode);
	}
	return ISL_StatusTypeDef::ISL_ERROR;
}
uint8_t BaseFlyWheel::MinForceMode()
{
	uint8_t result = 0;
	if(_channel == 1)
	{
		result = GetRegisterI2C(RegisterMap::Config) & (1 << 7);
	}
	if(_channel == 2)
	{
		result = GetRegisterI2C(RegisterMap::Config) & (1 << 6);
	}
	return result ? 1 : 0;
}

ISL_StatusTypeDef BaseFlyWheel::ReverseMode(uint8_t reverceMode)
{
	if(_channel == 1)
	{
		return BitRegisterI2C(RegisterMap::Config, 5, reverceMode);
	}
	if(_channel == 2)
	{
		return BitRegisterI2C(RegisterMap::Config, 4, reverceMode);
	}
	return ISL_StatusTypeDef::ISL_ERROR;
}
uint8_t BaseFlyWheel::ReverseMode()
{
	uint8_t result = 0;
	if(_channel == 1)
	{
		result = GetRegisterI2C(RegisterMap::Config) & (1 << 5);
	}
	if(_channel == 2)
	{
		result = GetRegisterI2C(RegisterMap::Config) & (1 << 4);
	}
	return result ? 1 : 0;
}

ISL_StatusTypeDef BaseFlyWheel::NeedSpeed(int16_t needSpeed)
{
	if(_channel == 1)
	{
		return SetRegisterI2C(RegisterMap::Need_F_ChannelSpeed, (uint8_t*)&needSpeed, 2);
	}
	if(_channel == 2)
	{
		return SetRegisterI2C(RegisterMap::Need_S_ChannelSpeed, (uint8_t*)&needSpeed, 2);
	}
	return ISL_StatusTypeDef::ISL_ERROR;
}
int16_t BaseFlyWheel::NeedSpeed()
{
	int16_t result = 0;
	if(_channel == 1)
	{
		ReadRegisterI2C(RegisterMap::Need_F_ChannelSpeed, (uint8_t*)&result, 2);
	}
	if(_channel == 2)
	{
		ReadRegisterI2C(RegisterMap::Need_S_ChannelSpeed, (uint8_t*)&result, 2);
	}
	return result;
}

int16_t BaseFlyWheel::CurrentSpeed()
{
	int16_t result = 0;
	if(_channel == 1)
	{
		ReadRegisterI2C(RegisterMap::Current_F_ChannelSpeed, (uint8_t*)&result, 2);
	}
	if(_channel == 2)
	{
		ReadRegisterI2C(RegisterMap::Current_S_ChannelSpeed, (uint8_t*)&result, 2);
	}
	return result;
}

ISL_StatusTypeDef BaseFlyWheel::MaxAbsSpeed(uint16_t maxAbsSpeed)
{
	if(_channel == 1)
	{
		return SetRegisterI2C(RegisterMap::Need_F_ChannelSpeed, (uint8_t*)&maxAbsSpeed, 2);
	}
	if(_channel == 2)
	{
		return SetRegisterI2C(RegisterMap::Need_S_ChannelSpeed, (uint8_t*)&maxAbsSpeed, 2);
	}
	return ISL_StatusTypeDef::ISL_ERROR;
}
uint16_t BaseFlyWheel::MaxAbsSpeed()
{
	int16_t result = 0;
	if(_channel == 1)
	{
		ReadRegisterI2C(RegisterMap::F_MaxAbsSpeed, (uint8_t*)&result, 2);
	}
	if(_channel == 2)
	{
		ReadRegisterI2C(RegisterMap::S_MaxAbsSpeed, (uint8_t*)&result, 2);
	}
	return result;
}

ISL_StatusTypeDef BaseFlyWheel::MinForce(uint16_t minForce)
{
	if(_channel == 1)
	{
		return SetRegisterI2C(RegisterMap::Need_F_ChannelSpeed, (uint8_t*)&minForce, 2);
	}
	if(_channel == 2)
	{
		return SetRegisterI2C(RegisterMap::Need_S_ChannelSpeed, (uint8_t*)&minForce, 2);
	}
	return ISL_StatusTypeDef::ISL_ERROR;
}
uint16_t BaseFlyWheel::MinForce()
{
	int16_t result = 0;
	if(_channel == 1)
	{
		ReadRegisterI2C(RegisterMap::F_Min_Force, (uint8_t*)&result, 2);
	}
	if(_channel == 2)
	{
		ReadRegisterI2C(RegisterMap::S_Min_Force, (uint8_t*)&result, 2);
	}
	return result;
}

ISL_StatusTypeDef BaseFlyWheel::PID_P(float p)
{
	if(_channel == 1)
	{
		return SetRegisterI2C(RegisterMap::F_P, (uint8_t*)&p, 4);
	}
	if(_channel == 2)
	{
		return SetRegisterI2C(RegisterMap::S_P, (uint8_t*)&p, 4);
	}
	return ISL_StatusTypeDef::ISL_ERROR;
}
float BaseFlyWheel::PID_P()
{
	float result = 0;
	if(_channel == 1)
	{
		ReadRegisterI2C(RegisterMap::F_P, (uint8_t*)&result, 4);
	}
	if(_channel == 2)
	{
		ReadRegisterI2C(RegisterMap::S_P, (uint8_t*)&result, 4);
	}
	return result;
}

ISL_StatusTypeDef BaseFlyWheel::PID_I(float i)
{
	if(_channel == 1)
	{
		return SetRegisterI2C(RegisterMap::F_I, (uint8_t*)&i, 4);
	}
	if(_channel == 2)
	{
		return SetRegisterI2C(RegisterMap::S_I, (uint8_t*)&i, 4);
	}
	return ISL_StatusTypeDef::ISL_ERROR;
}
float BaseFlyWheel::PID_I()
{
	float result = 0;
	if(_channel == 1)
	{
		ReadRegisterI2C(RegisterMap::F_I, (uint8_t*)&result, 4);
	}
	if(_channel == 2)
	{
		ReadRegisterI2C(RegisterMap::S_I, (uint8_t*)&result, 4);
	}
	return result;
}

ISL_StatusTypeDef BaseFlyWheel::PID_D(float d)
{
	if(_channel == 1)
	{
		return SetRegisterI2C(RegisterMap::F_D, (uint8_t*)&d, 4);
	}
	if(_channel == 2)
	{
		return SetRegisterI2C(RegisterMap::S_D, (uint8_t*)&d, 4);
	}
	return ISL_StatusTypeDef::ISL_ERROR;
}
float BaseFlyWheel::PID_D()
{
	float result = 0;
	if(_channel == 1)
	{
		ReadRegisterI2C(RegisterMap::F_D, (uint8_t*)&result, 4);
	}
	if(_channel == 2)
	{
		ReadRegisterI2C(RegisterMap::S_D, (uint8_t*)&result, 4);
	}
	return result;
}

BaseFlyWheel::BaseFlyWheel(const BaseFlyWheel &other): I2CDevice(other)
{
	_version = other._version;
	_channel = other._channel;
}

BaseFlyWheel::BaseFlyWheel(BaseFlyWheel &&other): I2CDevice(other)
{
	_version = other._version;
	_channel = other._channel;
}

BaseFlyWheel& BaseFlyWheel::operator=(const BaseFlyWheel &other)
{
	if (this != &other)
	{
		this->I2CDevice::operator =(other);
		_version = other._version;
		_channel = other._channel;
	}
	return *this;
}

BaseFlyWheel& BaseFlyWheel::operator=(BaseFlyWheel &&other)
{
	if (this != &other)
	{
		this->I2CDevice::operator =(other);
		_version = other._version;
		_channel = other._channel;
	}
	return *this;
}

BaseFlyWheel::~BaseFlyWheel() { }

} /* namespace IntroSatLib */
