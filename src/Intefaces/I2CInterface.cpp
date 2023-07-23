#include "I2CInterface.h"

namespace IntroSatLib
{
namespace Interfaces
{

#ifdef INTROSAT_HAVE_I2C

uint8_t I2CInterface::Address()
{
	return _address >> 1;
}

DeviceID I2CInterface::ID() const
{
	return DeviceID::I2C;
}

#ifdef INTROSAT_USE_HAL

I2CInterface::I2CInterface(I2C_HandleTypeDef *i2c, uint8_t address) :
		_i2c(i2c)
{
	if (address > 127)
	{
		address = 127;
	}
	_address = address << 1;
}

Status I2CInterface::IsReady()
{
	return StatusConverter::Convert(
			HAL_I2C_IsDeviceReady(_i2c, _address, 1, 1000));
}

Status I2CInterface::Read(uint8_t *Data, uint8_t Nbytes)
{
	return StatusConverter::Convert(
			HAL_I2C_Master_Receive(_i2c, _address, Data, Nbytes, 1000));
}

Status I2CInterface::Read(uint8_t Register, uint8_t *Data, uint8_t Nbytes)
{
	return StatusConverter::Convert(
			HAL_I2C_Mem_Read(_i2c, _address, Register, I2C_MEMADD_SIZE_8BIT,
					Data, Nbytes, 1000));
}

Status I2CInterface::Write(uint8_t *Data, uint8_t Nbytes)
{
	return StatusConverter::Convert(
			HAL_I2C_Master_Transmit(_i2c, _address, Data, Nbytes, 1000));
}

Status I2CInterface::Write(uint8_t Register, uint8_t *Data, uint8_t Nbytes)
{
	return StatusConverter::Convert(
			HAL_I2C_Mem_Write(_i2c, _address, Register, I2C_MEMADD_SIZE_8BIT,
					Data, Nbytes, 1000));
}

#endif /* INTROSAT_USE_HAL */

#ifdef INTROSAT_USE_ARDUINO

		I2CInterface::I2CInterface(TwoWire &i2c, uint8_t address): _i2c(i2c)
		{
			if (address > 127)
			{
				address = 127;
			}
			_address = address << 1;
		}

#endif /* INTROSAT_USE_ARDUINO */

#endif /* INTROSAT_HAVE_I2C */
}
}
