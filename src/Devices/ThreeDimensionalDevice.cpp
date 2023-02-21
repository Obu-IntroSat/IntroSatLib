#include "ThreeDimensionalDevice.h"

namespace IntroSatLib
{
	namespace Devices
	{
		template<typename T, int BufferSize>
		Status ThreeDimensionalDevice<T, BufferSize>::UpdateData()
		{
			Status result = Status::Ok;
			_position = (_position + 1) % BufferSize;

			_bufferX[_position] = ConvertValue(RawX(&result));
			_bufferY[_position] = ConvertValue(RawY(&result));
			_bufferZ[_position] = ConvertValue(RawZ(&result));

			_averageX = 0;
			_averageY = 0;
			_averageZ = 0;

			for (int i = 0; i < BufferSize; i++)
			{
				_averageX += _bufferX[i] / BufferSize;
				_averageY += _bufferY[i] / BufferSize;
				_averageZ += _bufferZ[i] / BufferSize;
			}

			return result;

		}

		template<typename T, int BufferSize>
		T ThreeDimensionalDevice<T, BufferSize>::X() const
		{
			return _averageX;
		}

		template<typename T, int BufferSize>
		T ThreeDimensionalDevice<T, BufferSize>::Y() const
		{
			return _averageY;
		}

		template<typename T, int BufferSize>
		T ThreeDimensionalDevice<T, BufferSize>::Z() const
		{
			return _averageZ;
		}
	}
}
