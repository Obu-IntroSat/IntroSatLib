#ifndef _DEVICES_LSM6DS3_H_
#define _DEVICES_LSM6DS3_H_

#include "../Services/Gyroscope.h"
#include <cinttypes>
#include <vector>
#include <functional>

namespace IntroSatLib
{
	namespace Devices
	{
		using Holders::Base::Error;

		class LSM6DS3: public IntroSatLib::Services::Gyroscope<3>
		{
		public:
			static constexpr std::uint8_t BaseAddress = 0x6A << 1;
		private:
			static constexpr float _rawdps = 8.75f / 1000.f;
		private:
			Holders::Base::Reference<Interfaces::I2CService> _i2c;
		private:
			Error SetDataRate()
			{
				char res[80] = {0};
				sprintf(res, "SetDataRate %s\n", GetId().data());
				HAL_UART_Transmit(&huart1, (uint8_t *)res, strlen(res), 1000);
				uint8_t data = 0;
				_i2c->ForceReadMem(BaseAddress, 0x11, &data, 1);
				data &= 0x0F;
				data |= (4 << 4);
				_i2c->ForceWriteMem(BaseAddress, 0x11, &data, 1);
				sprintf(res, "SetDataRate end %s\n", GetId().data());
				HAL_UART_Transmit(&huart1, (uint8_t *)res, strlen(res), 1000);
				return 0;
			}
		private:
			Error Init(const Holders::Base::IHolder& holder) override
			{
				_i2c = holder.Get<Interfaces::I2CService>();
				char result[80] = {0};
				sprintf(result, "i2c set %s\n", _i2c->GetId().data());
				HAL_UART_Transmit(&huart1, (uint8_t *)result, strlen(result), 1000);
				SetDataRate();
				sprintf(result, "Ready %s\n", GetId().data());
				HAL_UART_Transmit(&huart1, (uint8_t *)result, strlen(result), 1000);
				return 0;
			}

			void ReadAxi(std::uint8_t index) override
			{
				_i2c->ReadMem(BaseAddress, 0x22 + (index << 1), 2, [this, index](const std::vector<std::uint8_t>& data) -> void
				{
					int16_t result = static_cast<int16_t>(data[1] << 8) | data[0];
					result *= 1 << GetScale();
					char res[80] = {0};
					sprintf(res, "Raw value %s %d %d\n", GetId().data(), index, result);
					HAL_UART_Transmit(&huart1, (uint8_t *)res, strlen(res), 1000);
					this->SetData(result * _rawdps * 3.14 / 180, index);
				});
			}

			Error SetScaleReal(std::int8_t scale) override
			{
				uint8_t data = 0;
				char res[80] = {0};
				sprintf(res, "Set scale start %s\n", GetId().data());
				HAL_UART_Transmit(&huart1, (uint8_t *)res, strlen(res), 1000);
				Error result = 0;
				result |= _i2c->ForceReadMem(BaseAddress, 0x11, &data, 1);
				data &= 0xFF ^ (3 << 2);
				data |= (scale << 2);
				result |= _i2c->ForceWriteMem(BaseAddress, 0x11, &data, 1);
				sprintf(res, "Set scale end %s\n", GetId().data());
				HAL_UART_Transmit(&huart1, (uint8_t *)res, strlen(res), 1000);
				return result;
			}
		};
	} /* namespace Services */
} /* namespace IntroSatLib */

#endif /* _DEVICES_LSM6DS3_H_ */
