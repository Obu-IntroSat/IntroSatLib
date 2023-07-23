#ifndef BASE_REALDEVICE_H_
#define BASE_REALDEVICE_H_

#include <array>
#include "../Filters/ScalableFilter.h"
#include "../Base/Device.h"

namespace IntroSatLib
{
namespace Base
{

template<uint8_t InputCount, typename InputType, uint8_t OutputCount,
		typename OutputType>
class RealDevice: public Device
{
private:
	std::array<IntroSatLib::Filters::ScalableFilter<InputType>, InputCount> _inputs;
	std::array<OutputType, OutputCount> _outputs;

protected:

	RealDevice(std::unique_ptr<Interface> interface, DeviceID id) :
			Device(std::move(interface), id)
	{
		id |= GenerateDeviceID(InputCount << 16);
		id |= GenerateDeviceID(OutputCount << 20);
	}

public:
	IntroSatLib::Filters::ScalableFilterValue<InputType> operator[](
			uint8_t index)
	{
		return _inputs[index % InputCount]->ScaledValue();
	}

//				OutputType& operator[](uint8_t index, OutputType value)
//				{
//					return _inputs[index % OutputCount];
//				}
};

}
}

#endif /* BASE_REALDEVICE_H_ */
