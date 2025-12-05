#include "LightSensor.h"

namespace IntroSatLib {

//#ifndef ARDUINO
LightSensor::LightSensor(const interfaces::I2C &i2c, uint8_t address): I2CDevice(new interfaces::I2C(i2c), address)
{
}
//#else
//LightSensor::LightSensor(TwoWire &hi2c, uint8_t address): BaseDevice(hi2c, address)
//{
//}
//LightSensor::LightSensor(uint8_t address): BaseDevice(address)
//{
//}
//#endif

LightSensor::LightSensor(const LightSensor &other): I2CDevice(other)
{
}

LightSensor::LightSensor(LightSensor &&other): I2CDevice(other)
{
}

LightSensor& LightSensor::operator=(const LightSensor &other)
{
	if (this != &other)
	{
		this->I2CDevice::operator =(other);
	}
	return *this;
}

LightSensor& LightSensor::operator=(LightSensor &&other)
{
	if (this != &other)
	{
		this->I2CDevice::operator =(other);
	}
	return *this;
}
// TODO Возвращать не просто 0, а результат проверки наличия датчика
ISL_StatusTypeDef LightSensor::Init()
{
	// return IsReady();
	uint8_t buf[2];
	WriteI2C(buf, 1);
	return ReadI2C(buf, 2);
}

int16_t LightSensor::GetLight()
{
	static uint8_t buf[2];
	WriteI2C(buf, 1);
	ReadI2C(buf, 2);
	return buf[1] << 8 | buf[0];
}

LightSensor::~LightSensor() { }

} /* namespace IntroSatLib */
