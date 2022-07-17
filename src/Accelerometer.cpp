#include <Accelerometer.h>

namespace IntroSatLib {

#ifndef ARDUINO
	Accelerometer::Accelerometer(I2C_HandleTypeDef *hi2c, uint8_t address)
	{
		_i2c = I2CDevice(hi2c, address);
	}
#else
	Accelerometer::Accelerometer(TwoWire &hi2c, uint8_t address)
	{
		_i2c = I2CDevice(hi2c, address);
	}
#endif

Accelerometer::Accelerometer(const Accelerometer& other)
{
	*(&_i2c) = other._i2c;
	_sensitivity= other._sensitivity;
}
Accelerometer::Accelerometer(Accelerometer&& other)
{
	*(&_i2c) = other._i2c;
	_sensitivity= other._sensitivity;
}
Accelerometer& Accelerometer::operator=(const Accelerometer& other)
{
	if (this == &other)
	{
		return *this;
	}
	*(&_i2c) = other._i2c;
	_sensitivity= other._sensitivity;
	return *this;
}
Accelerometer& Accelerometer::operator=(Accelerometer&& other)
{
	if (this == &other)
	{
		return *this;
	}
	*(&_i2c) = other._i2c;
	_sensitivity= other._sensitivity;
	return *this;
}

void Accelerometer::Init(Scale sensitivity, FilterBandwidth filter)
{
	SetScale(sensitivity);
	SetFilter(filter);
}
void Accelerometer::Init(Scale sensitivity)
{
	Init(sensitivity, FilterBandwidth::F0021);
}
void Accelerometer::Init()
{
	Init(Scale::twoG);
}


void Accelerometer::SetScale(Scale sensitivity)
{
	uint8_t reg = GetRegister(RegisterMap::ACCEL_CONFIG);
	reg &= 0xFF ^ (Scale::sixteenG << 3);
	reg |= (sensitivity << 3);
	_sensitivity = sensitivity;
	SetRegister(RegisterMap::ACCEL_CONFIG, reg);
}

void Accelerometer::SetFilter(FilterBandwidth filter)
{
	SetRegister(RegisterMap::ACCEL_CONFIG_2, filter);
}


int16_t Accelerometer::RawX()
{
	uint8_t buf[2];
	_i2c.read(RegisterMap::ACCEL_XOUT_H, buf, 2);
	return buf[0] << 8 | buf[1];
}
int16_t Accelerometer::RawY()
{
	uint8_t buf[2];
	_i2c.read(RegisterMap::ACCEL_YOUT_H, buf, 2);
	return buf[0] << 8 | buf[1];
}
int16_t Accelerometer::RawZ()
{
	uint8_t buf[2];
	_i2c.read(RegisterMap::ACCEL_ZOUT_H, buf, 2);
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

void Accelerometer::SetRegister(RegisterMap reg, uint8_t value)
{
	_i2c.write(reg, &value, 1);
}

uint8_t Accelerometer::GetRegister(RegisterMap reg)
{
	uint8_t value = 0;
	_i2c.read(reg, &value, 1);
	return value;
}

Accelerometer::~Accelerometer()
{
	_i2c.~I2CDevice();
}

}
