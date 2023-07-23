#ifndef DEVICES_ACCELEROMETER_H_
#define DEVICES_ACCELEROMETER_H_

#include "./ThreeDimensionalDevice.h"

#include "../Measurement.h"

namespace IntroSatLib
{
namespace Devices
{
template<typename TRaw>
class Accelerometer: ThreeDimensionalDevice<DistanceAcceleration, TRaw>
{

protected:

	Accelerometer(std::unique_ptr<IntroSatLib::Base::Interface> interface,
			IntroSatLib::Base::DeviceID id, int8_t maxScale, uint8_t size) :
			ThreeDimensionalDevice(std::move(interface), id, maxScale, size)
	{
	}
	;

	Accelerometer(std::unique_ptr<IntroSatLib::Base::Interface> interface,
			IntroSatLib::Base::DeviceID id, int8_t minScale, int8_t maxScale,
			uint8_t size) :
			ThreeDimensionalDevice(std::move(interface), id, minScale, maxScale,
					size)
	{
	}
	;
};
}
}

#endif /* DEVICES_ACCELEROMETER_H_ */
