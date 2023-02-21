#ifndef DEVICES_ACCELEROMETER_H_
#define DEVICES_ACCELEROMETER_H_

#include "Devices/ThreeDimensionalDevice.h"

#include "Measurement.h"

namespace IntroSatLib
{
	namespace Devices
	{
		template<int BufferSize>
		class Accelerometer_: public ThreeDimensionalDevice<IntroSatLib::Units::DistanceAcceleration, BufferSize>
		{
			protected:
				Accelerometer_(std::unique_ptr<Interface> interface): ThreeDimensionalDevice(std::move(interface)) { };
		};
	}
}

#endif /* DEVICES_ACCELEROMETER_H_ */
