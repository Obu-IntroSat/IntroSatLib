#include "LightSensor.h"

namespace IntroSatLib
{
namespace Devices
{
template<int BufferSize>
Status LightSensor_<BufferSize>::UpdateData()
{
	Status result = Status::Ok;
	_position = (_position + 1) % BufferSize;

	_buffer[_position] = ConvertValue(RawLight(&result));

	_average = 0;

	for (int i = 0; i < BufferSize; i++)
	{
		_average += _buffer[i] / BufferSize;
	}

	return result;
}

template<int BufferSize>
IntroSatLib::Units::Luminance LightSensor_<BufferSize>::Light() const
{
	return _average;
}
}
}
