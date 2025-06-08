#include "AccelerometerV2.h"

namespace IntroSatLib {

#ifndef ARDUINO
AccelerometerV2::AccelerometerV2(I2C_HandleTypeDef *hi2c, uint8_t address): BaseDevice(hi2c, address)
{
}
#else
AccelerometerV2::AccelerometerV2(TwoWire &hi2c, uint8_t address): BaseDevice(hi2c, address)
{
}
#endif

uint8_t AccelerometerV2::Init()
{
	uint8_t whoIAm = GetRegister(RegisterMap::WHO_AM_I_REG);
	if (whoIAm != 0x6C && whoIAm != 0x69) { return 1; }
	_sens = 1;
	SetRegister(RegisterMap::CTRL1_XL, BASE_ACCEL_CONF);
	return 0;
}

uint8_t AccelerometerV2::Init(Scale sens)
{ 
	if (Init()) { return 1; }
	SetScale(sens);
	return 0;
}

uint8_t AccelerometerV2::Init(Scale sens, FilterBandwidth filter)
{
	if (Init()) { return 1; }
	SetScale(sens);
	SetFilter(filter);
	return 0;
}

uint8_t AccelerometerV2::Init(Scale sens, FilterBandwidth filter, DataRate datarate)
{
	if (Init()) { return 1; }
	SetScale(sens);
	SetFilter(filter);
	SetDataRate(datarate);
	return 0;
}

uint8_t AccelerometerV2::SetScale(Scale sens) {
	uint8_t reg = GetRegister(RegisterMap::CTRL1_XL);
	reg &= 0xFF ^ (Scale::eightG << 2);
	reg |= (sens << 2);
	switch (sens)
	{
		case 0:
			_sens = 0;
			break;
		case 1:
			_sens = 3;
			break;
		case 2:
			_sens = 1;
			break;
		case 3:
			_sens = 2;
			break;
	}
	SetRegister(RegisterMap::CTRL1_XL, reg);
	return sens;
}

void AccelerometerV2::SetFilter(FilterBandwidth filter) {
	uint8_t reg = GetRegister(RegisterMap::CTRL1_XL);
	reg &= 0xFF ^ (FilterBandwidth::F050H);
	reg |= (filter);
	SetRegister(RegisterMap::CTRL1_XL, reg);
}

void AccelerometerV2::SetDataRate(DataRate datarate) {
	uint8_t reg = GetRegister(RegisterMap::CTRL1_XL);
	reg &= 0xFF ^ (0x0F<<4);
	reg |= (datarate<<4);
	SetRegister(RegisterMap::CTRL1_XL, reg);
}

void AccelerometerV2::end()//отключение акселерометра
{
	SetRegister(RegisterMap::CTRL1_XL, 0x00);
}

int16_t AccelerometerV2::RawX()
{
	uint8_t buf[2];
	_i2c.read(RegisterMap::OUTX_L_XL, buf, 2);
	return buf[1] << 8 | buf[0];
}
int16_t AccelerometerV2::RawY()
{
	uint8_t buf[2];
	_i2c.read(RegisterMap::OUTY_L_XL, buf, 2);
	return buf[1] << 8 | buf[0];
}
int16_t AccelerometerV2::RawZ()
{
	uint8_t buf[2];
	_i2c.read(RegisterMap::OUTZ_L_XL, buf, 2);
	return buf[1] << 8 | buf[0];
}

float AccelerometerV2::X()
{
	float e = RawX() * (1 << _sens) / _rawg;
	return e;
}
float AccelerometerV2::Y()
{
	float e = RawY() * (1 << _sens) / _rawg;
	return e;
}
float AccelerometerV2::Z()
{
	float e = RawZ() * (1 << _sens) / _rawg;
	return e;
}

float AccelerometerV2::Temp() {
	uint8_t buf[2];
	_i2c.read(RegisterMap::OUT_TEMP_L, buf, 2);
	return (int16_t)(buf[1] << 8 | buf[0]) / 16.0 + 25;
}

AccelerometerV2::~AccelerometerV2() { }

}
