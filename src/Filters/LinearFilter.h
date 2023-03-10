#ifndef FILTERS_LINEARFILTER_H_
#define FILTERS_LINEARFILTER_H_

#include "../Base/BaseFilter.h"

#include <vector>
#include <type_traits>

namespace IntroSatLib
{
	namespace Filters
	{
		template<typename T>
		class LinearFilter: IntroSatLib::Base::BaseFilter<T>
		{
			using T1 = typename std::conditional<std::is_same<int8_t, T>::value,
				int16_t, std::conditional<std::is_same<int16_t, T>::value,
				int32_t, std::conditional<std::is_same<int32_t, T>::value,
				int64_t, std::conditional<std::is_same<uint8_t, T>::value,
				uint16_t, std::conditional<std::is_same<uint16_t, T>::value,
				uint32_t, std::conditional<std::is_same<uint32_t, T>::value,
				uint64_t, void>>>>>>::type;

			private:
				std::vector<T> _buffer;
				uint8_t _position;
				uint8_t _size;

				T _average = 0;
				T1 _sum = 0;

			public:
				uint8_t Size() const override
				{
					return _size;
				}


				T AddValue(T newValue) override
				{
					_sum += newValue - _buffer[_position];
					_buffer[_position] = newValue;

					_average = _sum / _size;
					_position = (_position + 1) % _size;
					return _average;
				};

				T operator[](uint8_t index) const override
				{
					return _buffer[(index + _position) % _size];
				}
		};

	}
}

#endif /* FILTERS_LINEARFILTER_H_ */
