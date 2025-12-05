#include "I2CDevice.h"

namespace IntroSatLib {

ISL_StatusTypeDef I2CDevice::IsReady() {
	return _i2c->isReady(_address);
}

ISL_StatusTypeDef I2CDevice::WriteI2C(uint8_t* buf, uint8_t nBytes)
{
	return _i2c->write(_address, buf, nBytes);
}

ISL_StatusTypeDef I2CDevice::WriteI2C(uint8_t byte)
{
	return _i2c->write(_address, &byte, 1);
}

ISL_StatusTypeDef I2CDevice::ReadI2C(uint8_t* buf, uint8_t nBytes)
{
	return _i2c->read(_address, buf, nBytes);
}

ISL_StatusTypeDef I2CDevice::ReadRegisterI2C(uint8_t reg, uint8_t* data) {
	return ReadRegisterI2C(reg, data, 1);
}

ISL_StatusTypeDef I2CDevice::ReadRegisterI2C(uint8_t reg, uint8_t* data, uint8_t nBytes) {
	return _i2c->readMem(_address, reg, data, nBytes);
}

ISL_StatusTypeDef I2CDevice::SetRegisterI2C(uint8_t reg, uint8_t value)
{
	return SetRegisterI2C(reg, &value, 1);
}


ISL_StatusTypeDef I2CDevice::SetRegisterI2C(uint8_t reg, uint8_t* value, uint8_t nBytes)
{
	return _i2c->writeMem(_address, reg, value, nBytes);
}

uint8_t I2CDevice::GetRegisterI2C(uint8_t reg)
{
	uint8_t value = 0;
	_i2c->readMem(_address, reg, &value, 1);
	return value;
}

ISL_StatusTypeDef I2CDevice::SetBitRegisterI2C(uint8_t reg, uint8_t bit)
{
	if (bit > 7) bit = 7;
	uint8_t tmp;
	RETURN_STATUS_IF_NOT_OK_SILENT(ReadRegisterI2C(reg, &tmp));
	tmp |= (1 << bit);
	return SetRegisterI2C(reg, tmp);
}
ISL_StatusTypeDef I2CDevice::ResetBitRegisterI2C(uint8_t reg, uint8_t bit)
{
	if (bit > 7) bit = 7;
	uint8_t tmp;
	RETURN_STATUS_IF_NOT_OK_SILENT(ReadRegisterI2C(reg, &tmp));
	tmp &= ~(1 << bit);
	return SetRegisterI2C(reg, tmp);
}
ISL_StatusTypeDef I2CDevice::BitRegisterI2C(uint8_t reg, uint8_t bit, uint8_t value)
{
	if (value)
	{
		return SetBitRegisterI2C(reg, bit);
	}
	else
	{
		return ResetBitRegisterI2C(reg, bit);
	}
}

I2CDevice::I2CDevice(interfaces::I2C *i2c, uint8_t address): _i2c(i2c), _address(address)
{
}

ISL_StatusTypeDef I2CDevice::Init()
{
	return IsReady();
}

I2CDevice::I2CDevice(const I2CDevice& other): _i2c(other._i2c), _address(other._address)
{
}
I2CDevice::I2CDevice(I2CDevice&& other): _i2c(other._i2c), _address(other._address)
{
}
I2CDevice& I2CDevice::operator=(const I2CDevice& other)
{
	if (this != &other)
	{
		_i2c = other._i2c;
		_address = other._address;
//		delete &other._i2c;
	}
	return *this;
}
I2CDevice& I2CDevice::operator=(I2CDevice&& other)
{
	if (this != &other)
	{
		_i2c = other._i2c;
		_address = other._address;
//		delete &other._i2c;
	}
	return *this;
}

bool I2CDevice::CheckRegisterI2C(uint8_t reg, uint8_t value) {
	uint8_t tmp;
	return (((bool) ReadRegisterI2C(reg, &tmp)) && (tmp = value));
}

I2CDevice::~I2CDevice()
{
	delete _i2c;
}

} /* namespace IntroSatLib */
