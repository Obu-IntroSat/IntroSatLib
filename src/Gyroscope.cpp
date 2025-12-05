#include "Gyroscope.h"

namespace IntroSatLib {

//#ifndef ARDUINO
Gyroscope::Gyroscope(const interfaces::I2C &i2c, uint8_t address): I2CDevice(new interfaces::I2C(i2c), address)
{
}
//#else
//Gyroscope::Gyroscope(TwoWire &hi2c, uint8_t address): I2CDevice(hi2c, address)
//{
//}
//Gyroscope::Gyroscope(uint8_t address): I2CDevice(address)
//{
//}
//#endif

Gyroscope::Gyroscope(const Gyroscope &other): I2CDevice(other)
{
	_sensitivity= other._sensitivity;
}
Gyroscope::Gyroscope(Gyroscope &&other): I2CDevice(other)
{
	_sensitivity= other._sensitivity;
}
Gyroscope& Gyroscope::operator=(const Gyroscope &other)
{
	if (this != &other)
	{
		this->I2CDevice::operator = (other);
		_sensitivity= other._sensitivity;
	}
	return *this;
}
Gyroscope& Gyroscope::operator=(Gyroscope &&other)
{
	if (this != &other)
	{
		this->I2CDevice::operator = (other);
		_sensitivity= other._sensitivity;
	}
	return *this;
}

ISL_StatusTypeDef Gyroscope::Init(Scale sensitivity, FilterBandwidth filter)
{
	ISL_StatusTypeDef status = ISL_StatusTypeDef::ISL_OK;
	RETURN_STATUS_IF_NOT_OK(IsReady(), status);
	RETURN_STATUS_IF_NOT_OK(SetRegisterI2C(0x37, 0x02), status);
	RETURN_STATUS_IF_NOT_OK(SetScale(sensitivity), status);
	return SetFilter(filter);
}
ISL_StatusTypeDef Gyroscope::Init(Scale sensitivity)
{
	return Init(sensitivity, FilterBandwidth::F0005);
}
ISL_StatusTypeDef Gyroscope::Init()
{
	return Init(Scale::DPS0250);
}

ISL_StatusTypeDef Gyroscope::SetScale(Scale sensitivity)
{
	uint8_t reg;
	RETURN_STATUS_IF_NOT_OK_SILENT(ReadRegisterI2C(RegisterMap::GYRO_CONFIG, &reg));
	reg &= 0xFF ^ (Scale::DPS2000 << 3);
	reg |= (sensitivity << 3);
	_sensitivity = sensitivity;
	return SetRegisterI2C(RegisterMap::GYRO_CONFIG, reg);
}

ISL_StatusTypeDef Gyroscope::SetFilter(FilterBandwidth filter)
{
	ISL_StatusTypeDef status = ISL_StatusTypeDef::ISL_OK;
	uint8_t reg;
	RETURN_STATUS_IF_NOT_OK_SILENT(ReadRegisterI2C(RegisterMap::GYRO_CONFIG, &reg));
	reg &= 0xFF ^ 3;
	reg |= (filter >> 3);
	RETURN_STATUS_IF_NOT_OK(SetRegisterI2C(RegisterMap::GYRO_CONFIG, reg), status)

	RETURN_STATUS_IF_NOT_OK_SILENT(ReadRegisterI2C(RegisterMap::CONFIG, &reg));
	reg &= 0xFF ^ 7;
	reg |= (filter & 7);
	return SetRegisterI2C(RegisterMap::CONFIG, reg);
}

int16_t Gyroscope::RawX()
{
	uint8_t buf[2];
	ReadRegisterI2C(RegisterMap::GYRO_XOUT_H, buf, 2);
	return buf[0] << 8 | buf[1];
}
int16_t Gyroscope::RawY()
{
	uint8_t buf[2];
	ReadRegisterI2C(RegisterMap::GYRO_YOUT_H, buf, 2);
	return buf[0] << 8 | buf[1];
}
int16_t Gyroscope::RawZ()
{
	uint8_t buf[2];
	ReadRegisterI2C(RegisterMap::GYRO_ZOUT_H, buf, 2);
	return buf[0] << 8 | buf[1];
}

float Gyroscope::X()
{
	float e = RawX() * (1 << _sensitivity);
	return e / _rawdps;
}
float Gyroscope::Y()
{
	float e = RawY() * (1 << _sensitivity);
	return e / _rawdps;
}
float Gyroscope::Z()
{
	float e = RawZ() * (1 << _sensitivity);
	return e / _rawdps;
}


Gyroscope::~Gyroscope() { }

}
