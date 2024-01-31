#include "GyroscopeV2.h"

namespace IntroSatLib {

#ifndef ARDUINO
GyroscopeV2::GyroscopeV2(I2C_HandleTypeDef *hi2c, uint8_t address): BaseDevice(hi2c, address)
{
}
#else
GyroscopeV2::GyroscopeV2(TwoWire &hi2c, uint8_t address): BaseDevice(hi2c, address)
{
}
GyroscopeV2::GyroscopeV2(uint8_t address): BaseDevice(address)
{
}
#endif

GyroscopeV2::GyroscopeV2(const GyroscopeV2 &other): BaseDevice(other)
{
	_sensitivity = other._sensitivity;
	_dataRate = other._dataRate;
}
GyroscopeV2::GyroscopeV2(GyroscopeV2 &&other): BaseDevice(other)
{
	_sensitivity = other._sensitivity;
	_dataRate = other._dataRate;
}
GyroscopeV2& GyroscopeV2::operator=(const GyroscopeV2 &other)
{
	if (this != &other)
	{
		this->BaseDevice::operator = (other);
		_sensitivity= other._sensitivity;
		_dataRate = other._dataRate;
	}
	return *this;
}
GyroscopeV2& GyroscopeV2::operator=(GyroscopeV2 &&other)
{
	if (this != &other)
	{
		this->BaseDevice::operator = (other);
		_sensitivity= other._sensitivity;
		_dataRate = other._dataRate;
	}
	return *this;
}
uint8_t GyroscopeV2::Init(Scale sensitivity, DataRate dataRate)
{
	SetScale(sensitivity);
	SetDataRate(dataRate);
	_lastXTime = HAL_GetTick();
	_lastYTime = _lastXTime;
	_lastZTime = _lastXTime;
	return 0;
}

uint8_t GyroscopeV2::Init(Scale sensitivity)
{
	return Init(sensitivity, DataRate::F_104_Hz);
}
uint8_t GyroscopeV2::Init()
{
	return Init(Scale::DPS0250);
}

void GyroscopeV2::SetMinCutX(float x)
{
	_cutX = x;
}
void GyroscopeV2::SetMinCutY(float y)
{
	_cutY = x;
}
void GyroscopeV2::SetMinCutZ(float z)
{
	_cutZ = z;
}

float GyroscopeV2::cutMin(float value, float cut)
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

void GyroscopeV2::SetScale(Scale sensitivity)
{
	uint8_t reg = GetRegister(RegisterMap::GYRO_CONFIG);
	reg &= 0xFF ^ (Scale::DPS2000 << 2);
	reg |= (sensitivity << 2);
	_sensitivity = sensitivity;
	SetRegister(RegisterMap::GYRO_CONFIG, reg);
}

void GyroscopeV2::SetDataRate(DataRate dataRate)
{
	uint8_t reg = GetRegister(RegisterMap::GYRO_CONFIG);
	reg &= 0x0F;
	reg |= (dataRate << 4);
	_dataRate = dataRate;
	SetRegister(RegisterMap::GYRO_CONFIG, reg);
}

int16_t GyroscopeV2::RawX()
{
	uint8_t buf[2];
	_i2c.read(RegisterMap::GYRO_XOUT_H, buf, 2);
	return buf[1] << 8 | buf[0];
}
int16_t GyroscopeV2::RawY()
{
	uint8_t buf[2];
	_i2c.read(RegisterMap::GYRO_YOUT_H, buf, 2);
	return buf[1] << 8 | buf[0];
}
int16_t GyroscopeV2::RawZ()
{
	uint8_t buf[2];
	_i2c.read(RegisterMap::GYRO_ZOUT_H, buf, 2);
	return buf[1] << 8 | buf[0];
}

float GyroscopeV2::X()
{
	float e = RawX() * (1 << _sensitivity);
	float speed = cutMin(e * _rawdps, _cutX);
	uint32_t time = HAL_GetTick();
	uint32_t deltaTime = time - _lastXTime;
	float value = (_lastX + speed) * (deltaTime >> 1) * 0.001;
	_lastX = speed;
	_lastXTime = time;
	return value;
}
float GyroscopeV2::Y()
{
	float e = RawY() * (1 << _sensitivity);
	float speed = cutMin(e * _rawdps, _cutY);
	uint32_t time = HAL_GetTick();
	uint32_t deltaTime = time - _lastYTime;
	float value = (_lastY + speed) * (deltaTime >> 1) * 0.001;
	_lastY = speed;
	_lastYTime = time;
	return value;
}
float GyroscopeV2::Z()
{
	float e = RawZ() * (1 << _sensitivity);
	float speed = cutMin(e * _rawdps, _cutZ);
	uint32_t time = HAL_GetTick();
	uint32_t deltaTime = time - _lastZTime;
	float value = (_lastZ + speed) * (deltaTime >> 1) * 0.001;
	_lastZ = speed;
	_lastZTime = time;
	return value;
}

Quaternion<float> GyroscopeV2::GetQuaternion()
{
	std::array<float, 3> buf;
	buf[0] = X();
	buf[1] = Y();
	buf[2] = Z();
	return from_euler(buf);
}


GyroscopeV2::~GyroscopeV2()
{
	BaseDevice::~BaseDevice();
}

}
