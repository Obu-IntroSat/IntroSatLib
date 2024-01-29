#ifndef _SERVICES_L3G4200D_H_
#define _SERVICES_L3G4200D_H_

#include "../Base/FloatType.h"
#include "../Holders/Base/Types.h"
#include "../Holders/Base/IHolder.h"
#include "../Interfaces/I2CService.h"
#include "../Services/ThreeAxis.h"
#include <cinttypes>
#include <vector>


namespace IntroSatLib
{
	namespace TestSevice
	{
		using Holders::Base::Error;

		class L3G4200D: public Services::ThreeAxis
		{

		public:
			L3G4200D(): Services::ThreeAxis()
			{
				_addr = 0x68;
				_dataSize = 2;
				_addressesData[0] = 0x28;
				_addressesData[1] = 0x2A;
				_addressesData[2] = 0x2C;
			}
		protected:
			Error Init(const Holders::Base::IHolder& holder) override
			{
				this->ThreeAxis::Init(holder);
				std::uint8_t data = 0xF;
				_i2c->ForceWriteMem(_addr, 0x20, &data, 1);
				_i2c->ForceReadMem(_addr, 0x23, &data, 1);
				data |= 0x30;
				_i2c->ForceWriteMem(_addr, 0x23, &data, 1);
				return 0;
			}

			Base::Float ConvertValue(const std::vector<std::uint8_t>& data) override
			{
				return static_cast<std::int16_t>((static_cast<std::uint16_t>(data[1]) << 8) | data[0]) * 0.07;
			}

		};
	} /* namespace Services */
} /* namespace IntroSatLib */

#endif /* _SERVICES_L3G4200D_H_ */
