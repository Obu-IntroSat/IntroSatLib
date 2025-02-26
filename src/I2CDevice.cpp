#include "./I2CDevice.h"

#include "./Logger.h"

#include "./I2C_Err.h"


#define ASSERT_I2C_HAVE() \
if(!_hi2c) { \
	logText("Haven't i2c handle"); \
	return HAL_StatusTypeDef::HAL_ERROR; \
}

#define LOG_I2C_ADDRESS() \
logText("Device in "); \
logHEX(_address >> 1);

#if LOGDATA
#define LOG_I2C_BUFFER(Sep, Data, Nbytes) { \
logText(" - "); \
for(uint8_t i = 0; i < Nbytes; i++) { \
	logHEX(Data[i]); \
	if (i != (Nbytes - 1)) logText(Sep); \
} \
}

#else
#define LOG_I2C_BUFFER(Sep, Data, Nbytes)
#endif


namespace IntroSatLib {

#ifndef ARDUINO
I2CDevice::I2CDevice(I2C_HandleTypeDef *hi2c, uint8_t address)
{
	_hi2c = hi2c;
	if (address > 127) address = 127;
	_address = address << 1;
}

I2CDevice::I2CDevice(I2C_HandleTypeDef *hi2c, uint8_t address, I2CSpeed speed)
{
	_hi2c = hi2c;
	if (address > 127) address = 127;
	_address = address << 1;
	_speed = speed;
}
#else
I2CDevice::I2CDevice(TwoWire &hi2c, uint8_t address)
{
	_hi2c = hi2c.getHandle();
	if (address > 127) address = 127;
	_address = address << 1;
}

I2CDevice::I2CDevice(TwoWire &hi2c, uint8_t address, I2CSpeed speed)
{
	_hi2c = hi2c.getHandle();
	if (address > 127) address = 127;
	_address = address << 1;
	_speed = speed;
}
#endif

I2CDevice::I2CDevice(const I2CDevice& other)
{
	_address = other._address;
	_hi2c = other._hi2c;
	_speed = other._speed;
}

I2CDevice::I2CDevice(I2CDevice&& other)
{
	_address = other._address;
	_hi2c = other._hi2c;
	_speed = other._speed;
}

I2CDevice& I2CDevice::operator=(const I2CDevice& other)
{
	if (this == &other)
	{
		return *this;
	}
	_address = other._address;
	_hi2c = other._hi2c;
	_speed = other._speed;
	return *this;
}

I2CDevice& I2CDevice::operator=(I2CDevice&& other)
{
	if (this == &other)
	{
		return *this;
	}
	_address = other._address;
	_hi2c = other._hi2c;
	_speed = other._speed;
	return *this;
}
HAL_StatusTypeDef I2CDevice::isReady(uint8_t force)
{
	ASSERT_I2C_HAVE();
	while (innerIsReady() || force) { }
	return HAL_OK;
}

HAL_StatusTypeDef I2CDevice::innerIsReady()
{
	LOG_I2C_ADDRESS();
	logText(": ");
	HAL_StatusTypeDef status = logStatus(
			HAL_I2C_IsDeviceReady(_hi2c, _address, 1, 1000)
	);
	logText("\n");
	return status;
}


HAL_StatusTypeDef I2CDevice::read(uint8_t* Data, uint8_t Nbytes)
{
	ASSERT_I2C_HAVE();
	LOG_I2C_ADDRESS();
	logText(" read ");
	logNumber(Nbytes);
	logText("bytes > ");

	HAL_StatusTypeDef status = logStatus(
			HAL_I2C_Master_Receive(_hi2c, _address, Data, Nbytes, 1000)
	);
	if (status == HAL_OK) { LOG_I2C_BUFFER(", ", Data, Nbytes); } else {
		// #ifdef ARDUINO
		// 	Wire.end();
		// 	HAL_Delay(300);
		// 	Serial.println("I2C reset");
		// 	Wire.begin();
		// #endif
		I2C_ClearBusyFlagErratum(_hi2c, 300);
	}

	logText("\n");
	return status;

}
HAL_StatusTypeDef I2CDevice::read(uint8_t Register, uint8_t* Data, uint8_t Nbytes)
{
	ASSERT_I2C_HAVE();
	LOG_I2C_ADDRESS();
	logText(" read from memory ");
	logHEX(Register);
	logText(" ");
	logNumber(Nbytes);
	logText("bytes > ");
	HAL_StatusTypeDef status = logStatus(
		HAL_I2C_Mem_Read(
			_hi2c,
			_address,
			Register,
			I2C_MEMADD_SIZE_8BIT,
			Data,
			Nbytes,
			1000
		)
	);

	if (status == HAL_OK) { LOG_I2C_BUFFER(", ", Data, Nbytes); }else {
		// #ifdef ARDUINO
		// 	Wire.end();
		// 	HAL_Delay(300);
		// 	Serial.println("I2C reset");
		// 	Wire.begin();
		// #endif
		I2C_ClearBusyFlagErratum(_hi2c, 300);
	}

	logText("\n");
	return status;
}
HAL_StatusTypeDef I2CDevice::write(uint8_t* Data, uint8_t Nbytes)
{
	ASSERT_I2C_HAVE();
	LOG_I2C_ADDRESS();
	logText(" write ");
	logNumber(Nbytes);
	logText(" bytes ");
	LOG_I2C_BUFFER(", ", Data, Nbytes);
	logText(" > ");
	HAL_StatusTypeDef status = logStatus(
			HAL_I2C_Master_Transmit(_hi2c, _address, Data, Nbytes, 1000)
	);

	if (status == HAL_OK) { LOG_I2C_BUFFER(", ", Data, Nbytes); }else {
		// #ifdef ARDUINO
		// 	Wire.end();
		// 	HAL_Delay(300);
		// 	Serial.println("I2C reset");
		// 	Wire.begin();
		// #endif
		I2C_ClearBusyFlagErratum(_hi2c, 300);
	}

	logText("\n");
	return status;
}
HAL_StatusTypeDef I2CDevice::write(uint8_t Register, uint8_t* Data, uint8_t Nbytes)
{
	ASSERT_I2C_HAVE();
	LOG_I2C_ADDRESS();
	logText(" write from memory ");
	logHEX(Register);
	logText(" ");
	logNumber(Nbytes);
	logText(" bytes ");
	LOG_I2C_BUFFER(", ", Data, Nbytes);
	logText(" > ");
	HAL_StatusTypeDef status = logStatus(
		HAL_I2C_Mem_Write(
				_hi2c,
				_address,
				Register,
				I2C_MEMADD_SIZE_8BIT,
				Data,
				Nbytes,
				1000)
	);
	if (status == HAL_OK) { LOG_I2C_BUFFER(", ", Data, Nbytes); } else {
		// #ifdef ARDUINO
		// 	Wire.end();
		// 	HAL_Delay(300);
		// 	Serial.println("I2C reset");
		// 	Wire.begin();
		// #endif
		I2C_ClearBusyFlagErratum(_hi2c, 300);
	}
	logText("\n");
	return status;
}

I2CDevice::~I2CDevice()
{
}

} /* namespace IntroSatLib */
