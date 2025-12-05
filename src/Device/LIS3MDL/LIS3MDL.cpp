/*
 * LIS3MDL.cpp
 *
 *  Created on: Mar 18, 2025
 *      Author: unflesh
 */

#include "LIS3MDL.h"
#include "Adapter/System.h"

namespace IntroSatLib {


LIS3MDL::LIS3MDL(const interfaces::I2C &i2c, uint8_t address): I2CDevice(new interfaces::I2C(i2c), address)
{
}

ISL_StatusTypeDef LIS3MDL::Init(Scale scale, uint8_t force)
{
	system::Delay(15); // wait for powerup
	RETURN_STATUS_IF_NOT_OK_SILENT(IsReady());
	RETURN_STATUS_IF_NOT_OK_SILENT(SetRegisterI2C(RegisterMap::CTRL_REG1, 0x7C))
	system::Delay(1);
	RETURN_STATUS_IF_NOT_OK_SILENT(SetScale(scale));
	system::Delay(1);
	RETURN_STATUS_IF_NOT_OK_SILENT(SetRegisterI2C(RegisterMap::CTRL_REG3, 0x00))
	system::Delay(1);
	RETURN_STATUS_IF_NOT_OK_SILENT(SetRegisterI2C(RegisterMap::CTRL_REG4, 0x0C))
	system::Delay(1);
	RETURN_STATUS_IF_NOT_OK_SILENT(SetRegisterI2C(RegisterMap::CTRL_REG5, 0x40))
	return IsReady();
}

ISL_StatusTypeDef LIS3MDL::SetScale(Scale scale)
{
	uint8_t bitScale = 2 * (scale - 1);
	uint8_t reg = bitScale << 4;
	_scale = scale;
	return SetRegisterI2C(RegisterMap::CTRL_REG2, reg);
}

int16_t LIS3MDL::RawMX()
{
	if (GetRegisterI2C(RegisterMap::STATUS_REG) & 0x01) {
		uint8_t buf[2];
		ReadRegisterI2C(RegisterMap::OUT_X_L, buf, 2);
		_mx = buf[1] << 8 | buf[0];
	}
	return _mx;
}
int16_t LIS3MDL::RawMY()
{
	if (GetRegisterI2C(RegisterMap::STATUS_REG)&0x02) {
		uint8_t buf[2];
		ReadRegisterI2C(RegisterMap::OUT_Y_L, buf, 2);
		_my = buf[1] << 8 | buf[0];
	}
	return _my;
}
int16_t LIS3MDL::RawMZ()
{
	if (GetRegisterI2C(RegisterMap::STATUS_REG)&0x04) {
		uint8_t buf[2];
		ReadRegisterI2C(RegisterMap::OUT_Z_L, buf, 2);
		_mz = buf[1] << 8 | buf[0];
	}
	return _mz;
}

float LIS3MDL::MX()
{
	RawMX();
	float e = int16_t(_mx) * _scale;
	return e / _rawg;
}
float LIS3MDL::MY()
{
	RawMY();
	float e = int16_t(_my) * _scale;
	return e / _rawg;
}
float LIS3MDL::MZ()
{
	RawMZ();
	float e = int16_t(_mz) * _scale;
	return e / _rawg;
}


LIS3MDL::~LIS3MDL() { }

} /* namespace IntroSatLib */



