#ifndef _HAL_I2CSERVICE_H_
#define _HAL_I2CSERVICE_H_

#include "../Interfaces/I2CService.h"
#include "../Holders/Base/Types.h"
#include <cinttypes>

#include "stm32f1xx_hal.h"
#ifdef HAL_I2C_MODULE_ENABLED
	#include "stm32f1xx_hal_i2c.h"
#endif

namespace IntroSatLib
{
	namespace HAL
	{
		using Holders::Base::Error;
		class I2CService: public Interfaces::I2CService
		{
		private:
			I2C_HandleTypeDef *_hi2c = 0;
		public:
			I2CService(I2C_HandleTypeDef *i2c): _hi2c(i2c) { }

			Error ForceWrite(std::uint8_t addr, std::uint8_t* data, std::uint8_t len) override
			{
				if(!_hi2c) return HAL_StatusTypeDef::HAL_ERROR;
				return HAL_I2C_Master_Receive(_hi2c, addr, data, len, 1000);
			}

			Error ForceRead(std::uint8_t addr, std::uint8_t* data, std::uint8_t len) override
			{
				if(!_hi2c) return HAL_StatusTypeDef::HAL_ERROR;
				return HAL_I2C_Master_Transmit(_hi2c, addr, data, len, 1000);
			}

			Error ForceWriteMem(std::uint8_t addr, std::uint8_t reg, std::uint8_t* data, std::uint8_t len) override
			{
				if(!_hi2c) return HAL_StatusTypeDef::HAL_ERROR;
				return HAL_I2C_Mem_Write(_hi2c, addr, reg, I2C_MEMADD_SIZE_8BIT, data, len, 1000);
			}

			Error ForceReadMem(std::uint8_t addr, std::uint8_t reg, std::uint8_t* data, std::uint8_t len) override
			{
				if(!_hi2c) return HAL_StatusTypeDef::HAL_ERROR;
				return HAL_I2C_Mem_Read(_hi2c, addr, reg, I2C_MEMADD_SIZE_8BIT, data, len, 1000);
			}

		};
	} /* namespace HAL */
} /* namespace IntroSatLib */

#endif /* _HAL_I2CSERVICE_H_ */
