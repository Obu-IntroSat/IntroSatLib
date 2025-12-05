#include "Accelerometer.h"

namespace IntroSatLib {

Accelerometer::Accelerometer(const interfaces::I2C &i2c, uint8_t address): I2CDevice(new interfaces::I2C(i2c), address)
{
}

Accelerometer::Accelerometer(const Accelerometer& other): I2CDevice(other)
{
	_sensitivity= other._sensitivity;
}
Accelerometer::Accelerometer(Accelerometer&& other): I2CDevice(other)
{
	_sensitivity= other._sensitivity;
}
Accelerometer& Accelerometer::operator=(const Accelerometer& other)
{
	if (this != &other)
	{
		this->I2CDevice::operator = (other);
		_sensitivity= other._sensitivity;
	}
	return *this;
}
Accelerometer& Accelerometer::operator=(Accelerometer&& other)
{
	if (this != &other)
	{
		this->I2CDevice::operator =(other);
		_sensitivity= other._sensitivity;
	}
	return *this;
}

ISL_StatusTypeDef Accelerometer::Init(Scale sensitivity, FilterBandwidth filter)
{
	RETURN_STATUS_IF_NOT_OK_SILENT(SetRegisterI2C(0x37, 0x02));
	RETURN_STATUS_IF_NOT_OK_SILENT(SetScale(sensitivity));
	return SetFilter(filter);
	//TODO  возвращать _i2c.isReady();
//	return 0;
}
ISL_StatusTypeDef Accelerometer::Init(Scale sensitivity)
{
	return Init(sensitivity, FilterBandwidth::F0021);
}
ISL_StatusTypeDef Accelerometer::Init()
{
	return Init(Scale::twoG);
}


ISL_StatusTypeDef Accelerometer::SetScale(Scale sensitivity)
{
	ISL_StatusTypeDef status = ISL_StatusTypeDef::ISL_OK;
	uint8_t reg;
	RETURN_STATUS_IF_NOT_OK(ReadRegisterI2C(RegisterMap::ACCEL_CONFIG, &reg), status);
	reg &= 0xFF ^ (Scale::sixteenG << 3);
	reg |= (sensitivity << 3);
	_sensitivity = sensitivity;
	return SetRegisterI2C(RegisterMap::ACCEL_CONFIG, reg);
}

ISL_StatusTypeDef Accelerometer::SetFilter(FilterBandwidth filter)
{
	return SetRegisterI2C(RegisterMap::ACCEL_CONFIG_2, filter);
}


int16_t Accelerometer::RawX()
{
	uint8_t buf[2];
	ReadRegisterI2C(RegisterMap::ACCEL_XOUT_H, buf, 2);
	return buf[0] << 8 | buf[1];
}
int16_t Accelerometer::RawY()
{
	uint8_t buf[2];
	ReadRegisterI2C(RegisterMap::ACCEL_YOUT_H, buf, 2);
	return buf[0] << 8 | buf[1];
}
int16_t Accelerometer::RawZ()
{
	uint8_t buf[2];
	ReadRegisterI2C(RegisterMap::ACCEL_ZOUT_H, buf, 2);
	return buf[0] << 8 | buf[1];
}

float Accelerometer::X()
{
	float e = RawX() * (1 << _sensitivity);
	return e / _rawg;
}
float Accelerometer::Y()
{
	float e = RawY() * (1 << _sensitivity);
	return e / _rawg;
}
float Accelerometer::Z()
{
	float e = RawZ() * (1 << _sensitivity);
	return e / _rawg;
}

Accelerometer::~Accelerometer() { }

}
