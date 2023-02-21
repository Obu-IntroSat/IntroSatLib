#ifndef DEVICES_LIGHTSENSOR_H_
#define DEVICES_LIGHTSENSOR_H_

#include "../Base/Device.h"
#include "../Measurement.h"

using namespace IntroSatLib::Base;

namespace IntroSatLib
{
	namespace Devices
	{

		template<int BufferSize>
		class LightSensor_: public Device
		{
			private:
				int _position = 0;
				IntroSatLib::Units::Luminance _buffer[BufferSize];
				IntroSatLib::Units::Luminance _average;

			protected:
				LightSensor_(std::unique_ptr<Interface> interface): Device(std::move(interface)) { };

				virtual IntroSatLib::Units::Luminance ConvertValue(int32_t value) const;

				virtual int32_t RawLight(Status *status) const;

			public:
				Status UpdateData() override final;

				IntroSatLib::Units::Luminance Light() const;

		};

	}
}

#endif /* DEVICES_LIGHTSENSOR_H_ */
