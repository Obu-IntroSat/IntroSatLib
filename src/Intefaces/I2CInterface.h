#ifndef INTEFACES_I2CINTERFACE_H_
#define INTEFACES_I2CINTERFACE_H_

#include "../Base/Interface.h"

#ifdef ARDUINO
	#include "Arduino.h"
	#include "Wire.h"
#endif

#include "stm32f1xx_hal.h"
#ifdef HAL_I2C_MODULE_ENABLED
	#include "stm32f1xx_hal_i2c.h"
#endif

using namespace IntroSatLib::Base;

namespace IntroSatLib
{
	namespace Interfaces
	{

#if defined(ARDUINO) || defined(HAL_I2C_MODULE_ENABLED)

		class I2CInterface: public Interface
		{

#ifndef ARDUINO
			private:
				I2C_HandleTypeDef *_i2c;
			public:
				I2CInterface(I2C_HandleTypeDef *i2c, uint8_t address);
#else
			private:
				TwoWire &_i2c;
			public:
				I2CInterface(TwoWire &i2c, uint8_t address);
#endif

			private:
				uint8_t _address;

			public:
				uint8_t Address();
				Status IsReady();
				Status Read(uint8_t* Data, uint8_t Nbytes);
				Status Read(uint8_t Register, uint8_t* Data, uint8_t Nbytes);
				Status Write(uint8_t* Data, uint8_t Nbytes);
				Status Write(uint8_t Register, uint8_t* Data, uint8_t Nbytes);
		};
	}

}

#endif /* defined(ARDUINO) || defined(HAL_I2C_MODULE_ENABLED) */

#endif /* INTEFACES_I2CINTERFACE_H_ */
