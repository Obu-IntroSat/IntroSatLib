#include "AK8963.h"
#include "Adapter/System.h"
namespace IntroSatLib {

//#ifndef ARDUINO
AK8963::AK8963(const interfaces::I2C &i2c, uint8_t address): I2CDevice(new interfaces::I2C(i2c), address)
{
}

AK8963::AK8963(const AK8963& other): I2CDevice(other)
{
}
AK8963::AK8963(AK8963&& other): I2CDevice(other)
{
}
AK8963& AK8963::operator=(const AK8963& other)
{
	if (this != &other)
	{
		this->I2CDevice::operator = (other);
	}
	return *this;
}
AK8963& AK8963::operator=(AK8963&& other)
{
	if (this != &other)
	{
		this->I2CDevice::operator =(other);
	}
	return *this;
}


ISL_StatusTypeDef AK8963::Init()
{
	RETURN_STATUS_IF_NOT_OK_SILENT(SetRegisterI2C(0x0A, 0x00));
	system::Delay(100);
	RETURN_STATUS_IF_NOT_OK_SILENT(SetRegisterI2C(0x0A, 0x0F)); // Fuse ROM
	system::Delay(100);
	RETURN_STATUS_IF_NOT_OK_SILENT(ReadCal());
	RETURN_STATUS_IF_NOT_OK_SILENT(SetRegisterI2C(0x0A, 0x00));
	system::Delay(100);
	RETURN_STATUS_IF_NOT_OK_SILENT(SetRegisterI2C(0x0A, 0x06)); // Continuous measurement mode 2
	system::Delay(100);
	return ISL_StatusTypeDef::ISL_OK;
}

ISL_StatusTypeDef AK8963::ReadCal()
{
	uint8_t buf[3];
	ISL_StatusTypeDef status = ISL_StatusTypeDef::ISL_OK;
	if ((status = ReadRegisterI2C(0x10, buf, 3)) != ISL_StatusTypeDef::ISL_OK) { return status; }
//	ReadRegisterI2C(0x10, buf, 3);
	_calX = buf[0];
	_calY = buf[1];
	_calZ = buf[2];
	return status;
}

ISL_StatusTypeDef AK8963::Read()
{
	uint8_t buf[7];
	ISL_StatusTypeDef status = ISL_StatusTypeDef::ISL_OK;
	if ((status = ReadRegisterI2C(0x03, buf, 7)) != ISL_StatusTypeDef::ISL_OK) { return status; }
//	ReadRegisterI2C(0x03, buf, 7);
	if (!(buf[6] & 0x08))
	{
		_x = (buf[1] << 8) | buf[0];
		_y = (buf[3] << 8) | buf[2];
		_z = (buf[5] << 8) | buf[4];
	}
}


int16_t AK8963::RawX()
{
	Read();
	return int16_t(_x) * ((int8_t(_calX) - 128) / 256.0f + 1);
}
int16_t AK8963::RawY()
{
	Read();
	return int16_t(_y) * ((int8_t(_calY) - 128) / 256.0f + 1);
}
int16_t AK8963::RawZ()
{
	Read();
	return int16_t(_z) * ((int8_t(_calZ) - 128) / 256.0f + 1);
}

float AK8963::X()
{
	return RawX() / _rawmt;
}

float AK8963::Y()
{
	return RawY() / _rawmt;
}

float AK8963::Z()
{
	return RawZ() / _rawmt;
}

AK8963::~AK8963() { }

} /* namespace IntroSatLib */
