#include "MagnetometerV2.h"

namespace IntroSatLib {

#ifndef ARDUINO
MagnetometerV2::MagnetometerV2(I2C_HandleTypeDef *hi2c, uint8_t address): BaseDevice(hi2c, address)
{
}
#else
MagnetometerV2::MagnetometerV2(TwoWire &hi2c, uint8_t address): BaseDevice(hi2c, address)
{
}
MagnetometerV2::MagnetometerV2(uint8_t address): BaseDevice(address)
{
}
#endif

MagnetometerV2::MagnetometerV2(const MagnetometerV2& other): BaseDevice(other)
{
}
MagnetometerV2::MagnetometerV2(MagnetometerV2&& other): BaseDevice(other)
{
}
MagnetometerV2& MagnetometerV2::operator=(const MagnetometerV2& other)
{
	if (this != &other)
	{
		this->BaseDevice::operator = (other);
	}
	return *this;
}
MagnetometerV2& MagnetometerV2::operator=(MagnetometerV2&& other)
{
	if (this != &other)
	{
		this->BaseDevice::operator =(other);
	}
	return *this;
}


uint8_t MagnetometerV2::Init(Scale sensitivity)
{
	SetRegister(RegisterMap::CTRL_REG1, 0x7C);
	HAL_Delay(1);
	SetScale(sensitivity);
	HAL_Delay(1);
	SetRegister(RegisterMap::CTRL_REG3, 0x00);
	HAL_Delay(1);
	SetRegister(RegisterMap::CTRL_REG4, 0x0C);
	HAL_Delay(1);
	SetRegister(RegisterMap::CTRL_REG5, 0x40);
	return _i2c.isReady(); // Use existing method instead of returning 0
	// return 0;
}

uint8_t MagnetometerV2::Init()
{
	return Init(Scale::G16);
}	

void MagnetometerV2::SetScale(Scale sensitivity)
{
	uint8_t bitSensitivity = 2 * (sensitivity - 1);
	uint8_t reg = bitSensitivity << 4;
	_sensitivity = sensitivity;
	SetRegister(RegisterMap::CTRL_REG2, reg);  
}

void MagnetometerV2::Read()
{
	if (GetRegister(RegisterMap::STATUS_REG)&0x08) 
	{
		uint8_t buf[6];
		_i2c.read(RegisterMap::OUT_X_L, buf, 6);
		_x = buf[1] << 8 | buf[0];
		_y = buf[3] << 8 | buf[2];
		_z = buf[5] << 8 | buf[4];
	}
}

int16_t MagnetometerV2::RawX()
{
	return _x;
}
int16_t MagnetometerV2::RawY()
{
	return _y;
}
int16_t MagnetometerV2::RawZ()
{
	return _z;
}

float MagnetometerV2::X()
{
	float e = int16_t(_x) * _sensitivity;
	return e / _rawg;
}
float MagnetometerV2::Y()
{
	float e = int16_t(_y) * _sensitivity;
	return e / _rawg;	
}
float MagnetometerV2::Z()
{
	float e = int16_t(_z) * _sensitivity;
	return e / _rawg;	
}

Quaternion<float> MagnetometerV2::GetQuaternion()
{
	std::array<float, 3> buf;
	buf[0] = 0;
	buf[1] = 0;
	buf[2] = std::atan2(RawY(), RawX());
	return from_euler(buf);
}

MagnetometerV2::~MagnetometerV2()
{
	BaseDevice::~BaseDevice();
}

} /* namespace IntroSatLib */
