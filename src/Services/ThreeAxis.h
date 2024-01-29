#ifndef _SERVICES_THREEAXIS_H_
#define _SERVICES_THREEAXIS_H_

#include "../Base/FloatType.h"
#include "../Holders/Base/Types.h"
#include "../Holders/Base/IHolder.h"
#include "../Interfaces/I2CService.h"
#include "./Base/AxisService.h"
#include <cinttypes>
#include <vector>
#include <functional>

namespace IntroSatLib
{
	namespace Services
	{
		using Holders::Base::Error;

		class ThreeAxis: public Base::AxisService<3>
		{
		private:
			Base::Float _data[3] = {0};
			std::int8_t _scale = 0;
			std::int8_t _nextScale = 0;
		protected:
			std::int8_t _minScale = 0;
			std::int8_t _mavScale = 0;
			std::uint8_t _addr = 0;
			std::uint8_t _dataSize = 0;
			std::uint8_t _addressesData[3] = {0};

			Holders::Base::Reference<Interfaces::I2CService> _i2c;
		protected:
			Error Init(const Holders::Base::IHolder& holder) override
			{
				_i2c = holder.Get<Interfaces::I2CService>();
				return 0;
			}

			void ReadAxi(std::uint8_t i)
			{
				_i2c->ReadMem(_addr, _addressesData[i], _dataSize,
					[this, &i](const std::vector<std::uint8_t>& data) -> void
					{
						this->_data[i] = this->ConvertValue(data);
					}
				);
			}

			virtual Base::Float ConvertValue(const std::vector<std::uint8_t>& data)
			{
				return 0;
			}

		public:

			constexpr Base::Float X() const
			{
				return _data[0];
			}

			constexpr Base::Float Y() const
			{
				return _data[1];
			}

			constexpr Base::Float Z() const
			{
				return _data[2];
			}

		};
	} /* namespace Services */
} /* namespace IntroSatLib */

#endif /* _SERVICES_THREEAXIS_H_ */
