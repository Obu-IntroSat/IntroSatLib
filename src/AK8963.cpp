#include "AK8963.h"

namespace IntroSatLib {

#ifndef ARDUINO
AK8963::AK8963(I2C_HandleTypeDef *hi2c, uint8_t address): BaseDevice(hi2c, address)
{
}
#else
AK8963::AK8963(TwoWire &hi2c, uint8_t address): BaseDevice(hi2c, address)
{
}
AK8963::AK8963(uint8_t address): BaseDevice(address)
{
}
#endif

AK8963::AK8963(const AK8963& other): BaseDevice(other)
{
}
AK8963::AK8963(AK8963&& other): BaseDevice(other)
{
}
AK8963& AK8963::operator=(const AK8963& other)
{
	if (this != &other)
	{
		this->BaseDevice::operator = (other);
	}
	return *this;
}
AK8963& AK8963::operator=(AK8963&& other)
{
	if (this != &other)
	{
		this->BaseDevice::operator =(other);
	}
	return *this;
}


uint8_t AK8963::Init()
{
	SetRegister(0x0A, 0x00);
	HAL_Delay(100);
	SetRegister(0x0A, 0x0F);
	HAL_Delay(100);
	SetRegister(0x0A, 0x02);
	HAL_Delay(100);
}

void AK8963::Read()
{
	uint8_t buf[7];
	_i2c.read(0x03, buf, 7);
	if (!(buf[6] & 0x08))
	{
		_x = (buf[1] << 8) | buf[0];
		_y = (buf[3] << 8) | buf[2];
		_z = (buf[5] << 8) | buf[4];
		_i2c.read(0x10, buf, 3);
		_calX = buf[0];
		_calY = buf[1];
		_calZ = buf[2];
	}
}


int16_t AK8963::RawX()
{
	return _x + _calX;
}
int16_t AK8963::RawY()
{
	return _y + _calY;
}
int16_t AK8963::RawZ()
{
	return _z + _calZ;
}

AK8963::~AK8963()
{
	BaseDevice::~BaseDevice();
}

} /* namespace IntroSatLib */
