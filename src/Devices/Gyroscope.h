#ifndef DEVICES_GYROSCOPE_H_
#define DEVICES_GYROSCOPE_H_

#include "Devices/ThreeDimensionalDevice.h"

#include "Measurement.h"

namespace IntroSatLib
{
	namespace Devices
	{

		template<int BufferSize>
		class Gyroscope_: public ThreeDimensionalDevice<IntroSatLib::Units::DegreeVelocity, BufferSize>
		{
			protected:
				Gyroscope_(std::unique_ptr<Interface> interface): ThreeDimensionalDevice(std::move(interface)) { };
		};

	}
}

#endif /* DEVICES_GYROSCOPE_H_ */
