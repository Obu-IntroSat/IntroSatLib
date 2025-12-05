/*
 * LSM6DS3.cpp
 *
 *  Created on: Mar 17, 2025
 *      Author: Goldfor
 */

#include "LSM6DS3.h"
#include "Adapter/System.h"
#include "stdint.h"

namespace IntroSatLib {

LSM6DS3::LSM6DS3(const interfaces::I2C &i2c, uint8_t address): I2CDevice(new interfaces::I2C(i2c), address)
{
}

ISL_StatusTypeDef LSM6DS3::WhoAmI() {
	uint8_t iAm;
	RETURN_STATUS_IF_NOT_OK_SILENT(ReadRegisterI2C(RegisterMap::WHO_AM_I_REG, &iAm));
	return (ISL_StatusTypeDef) (iAm != 0x69);
}

ISL_StatusTypeDef LSM6DS3::InitGyro(ScaleGyro sensitivityGyro, DataRateGyro dataRateGyro)
{
	system::Delay(15); // wait for power up
	RETURN_STATUS_IF_NOT_OK_SILENT(IsReady())
	RETURN_STATUS_IF_NOT_OK_SILENT(WhoAmI())
	RETURN_STATUS_IF_NOT_OK_SILENT(SetScaleGyro(sensitivityGyro))
	RETURN_STATUS_IF_NOT_OK_SILENT(SetDataRateGyro(dataRateGyro))
	return IsReady();
}

ISL_StatusTypeDef LSM6DS3::InitGyro(ScaleGyro sensitivityGyro)
{
	return InitGyro(sensitivityGyro, DataRateGyro::DR_G_416_Hz);
}
ISL_StatusTypeDef LSM6DS3::InitGyro()
{
	return InitGyro(ScaleGyro::DPS_245);
}

ISL_StatusTypeDef LSM6DS3::InitAccel(ScaleAccel scaleAccel, DataRateAccel dataRateAccel, FilterBandwidthAccel filter) {
	system::Delay(15); // wait for power up
	RETURN_STATUS_IF_NOT_OK_SILENT(IsReady())
	RETURN_STATUS_IF_NOT_OK_SILENT(WhoAmI())
	RETURN_STATUS_IF_NOT_OK_SILENT(SetScaleAccel(scaleAccel))
	RETURN_STATUS_IF_NOT_OK_SILENT(SetDataRateAccel(dataRateAccel))
	RETURN_STATUS_IF_NOT_OK_SILENT(SetFilterAccel(filter))
	return IsReady();
}

ISL_StatusTypeDef LSM6DS3::InitAccel(ScaleAccel scaleAccel, DataRateAccel dataRateAccel)
{
	return InitAccel(scaleAccel, dataRateAccel, FilterBandwidthAccel::FB_A_400_Hz);
}

ISL_StatusTypeDef LSM6DS3::InitAccel(ScaleAccel scaleAccel)
{
	return InitAccel(scaleAccel, DataRateAccel::DR_A_104_Hz);
}
ISL_StatusTypeDef LSM6DS3::InitAccel()
{
	return InitAccel(ScaleAccel::fourG);
}

void LSM6DS3::SetMinCutX(float x)
{
	_cutX = x;
}
void LSM6DS3::SetMinCutY(float y)
{
	_cutY = y;
}
void LSM6DS3::SetMinCutZ(float z)
{
	_cutZ = z;
}

float LSM6DS3::cutMin(float value, float cut)
{
	if (value > 0)
	{
		return (value > cut) ? value - cut : 0;
	}
	else
	{
		return ((-value) > cut) ? value + cut : 0;
	}
}

ISL_StatusTypeDef LSM6DS3::SetScaleGyro(ScaleGyro sensitivityGyro)
{
	uint8_t reg;
	RETURN_STATUS_IF_NOT_OK_SILENT(ReadRegisterI2C(RegisterMap::GYRO_CONFIG, &reg, 1));
	reg &= 0xFF ^ (0b11 << 2);
	reg |= (sensitivityGyro << 2);
	_scaleGyro = sensitivityGyro;
	return SetRegisterI2C(RegisterMap::GYRO_CONFIG, &reg, 1);
}

ISL_StatusTypeDef LSM6DS3::SetDataRateGyro(DataRateGyro dataRateGyro)
{
	uint8_t reg;
	RETURN_STATUS_IF_NOT_OK_SILENT(ReadRegisterI2C(RegisterMap::GYRO_CONFIG, &reg, 1))
	reg &= 0b1111;
	reg |= (dataRateGyro << 4);
	_dataRateGyro = dataRateGyro;
	return SetRegisterI2C(RegisterMap::GYRO_CONFIG, &reg, 1);
}

ISL_StatusTypeDef LSM6DS3::SetScaleAccel(ScaleAccel scaleAccel) {
	uint8_t reg;
	RETURN_STATUS_IF_NOT_OK_SILENT(ReadRegisterI2C(RegisterMap::CTRL1_XL, &reg, 1));
	reg &= 0xFF ^ (0b11 << 2);
	reg |= (scaleAccel << 2);
	_scaleAccel = scaleAccel;
	
	return SetRegisterI2C(RegisterMap::CTRL1_XL, &reg, 1);
}


ISL_StatusTypeDef LSM6DS3::SetDataRateAccel(DataRateAccel dataRateAccel) {
	uint8_t reg;
	RETURN_STATUS_IF_NOT_OK_SILENT(ReadRegisterI2C(RegisterMap::CTRL1_XL, &reg, 1));
	reg &= 0xFF ^ (0b01111<<4);
	reg |= (dataRateAccel<<4);
	_dataRateAccel = dataRateAccel;
	return SetRegisterI2C(RegisterMap::CTRL1_XL, reg);
}

ISL_StatusTypeDef LSM6DS3::SetFilterAccel(FilterBandwidthAccel filterBandwidthAccel) {
	uint8_t reg;
	RETURN_STATUS_IF_NOT_OK_SILENT(ReadRegisterI2C(RegisterMap::CTRL1_XL, &reg, 1));
	reg &= 0xFF ^ 0b11;
	reg |= filterBandwidthAccel;
	_filterBandwidthAccel = filterBandwidthAccel;
	return SetRegisterI2C(RegisterMap::CTRL1_XL, reg);
}

int16_t LSM6DS3::RawGX()
{
	uint8_t buf[2];
	ReadRegisterI2C(RegisterMap::GYRO_XOUT_H, buf, 2);
	return buf[1] << 8 | buf[0];
}
int16_t LSM6DS3::RawGY()
{
	uint8_t buf[2];
	ReadRegisterI2C(RegisterMap::GYRO_YOUT_H, buf, 2);
	return buf[1] << 8 | buf[0];
}
int16_t LSM6DS3::RawGZ()
{
	uint8_t buf[2];
	ReadRegisterI2C(RegisterMap::GYRO_ZOUT_H, buf, 2);
	return buf[1] << 8 | buf[0];
}

float LSM6DS3::GX()
{
	float e = RawGX() * (1 << _scaleGyro);
	return cutMin(e * _rawdps, _cutX);
}

float LSM6DS3::GY()
{
	float e = RawGY() * (1 << _scaleGyro);
	return cutMin(e * _rawdps, _cutY);
}
float LSM6DS3::GZ()
{
	float e = RawGZ() * (1 << _scaleGyro);
	return cutMin(e * _rawdps, _cutZ);
}

int16_t LSM6DS3::RawAX()
{
	uint8_t buf[2];
	ReadRegisterI2C(RegisterMap::OUTX_L_XL, buf, 2);
	return buf[1] << 8 | buf[0];
}
int16_t LSM6DS3::RawAY()
{
	uint8_t buf[2];
	ReadRegisterI2C(RegisterMap::OUTY_L_XL, buf, 2);
	return buf[1] << 8 | buf[0];
}
int16_t LSM6DS3::RawAZ()
{
	uint8_t buf[2];
	ReadRegisterI2C(RegisterMap::OUTZ_L_XL, buf, 2);
	return buf[1] << 8 | buf[0];
}

float LSM6DS3::AX()
{
	float e = RawAX() * (1 << scaleAccelToShift(_scaleAccel)) * _rawg;
	return e;
}
float LSM6DS3::AY()
{
	float e = RawAY() * (1 << scaleAccelToShift(_scaleAccel)) * _rawg;
	return e;
}
float LSM6DS3::AZ()
{
	float e = RawAZ() * (1 << scaleAccelToShift(_scaleAccel)) * _rawg;
	return e;
}

float LSM6DS3::Temp() {
	uint8_t buf[2];
	ReadRegisterI2C(RegisterMap::OUT_TEMP_L, buf, 2);
	return float((int16_t)(buf[1] << 8 | buf[0]) / 16.0 + 25);
}

ISL_StatusTypeDef LSM6DS3::DeinitAccel() {
	return SetRegisterI2C(RegisterMap::CTRL1_XL, 0x00);
}

ISL_StatusTypeDef LSM6DS3::DeinitGyro() {
	return SetRegisterI2C(RegisterMap::CTRL2_G, 0x00);
}

ISL_StatusTypeDef LSM6DS3::Deinit() {
	RETURN_STATUS_IF_NOT_OK_SILENT(DeinitAccel());
	return DeinitGyro();
}

LSM6DS3::~LSM6DS3() { }

} /* namespace IntroSatLib */
