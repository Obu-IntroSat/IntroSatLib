#ifndef FILTERS_SCALABLEFILTER_H_
#define FILTERS_SCALABLEFILTER_H_

#include "../Base/BaseFilter.h"
#include <type_traits>
#include <limits>

namespace IntroSatLib
{
namespace Filters
{

template<typename T>
using ScalableFilterValue = typename std::conditional<std::is_same<int8_t, T>::value,
int16_t, std::conditional<std::is_same<int16_t, T>::value,
int32_t, std::conditional<std::is_same<int32_t, T>::value,
int64_t, std::conditional<std::is_same<uint8_t, T>::value,
uint16_t, std::conditional<std::is_same<uint16_t, T>::value,
uint32_t, std::conditional<std::is_same<uint32_t, T>::value,
uint64_t, void>>>>>>::type;
template<typename T>
class ScalableFilter: public IntroSatLib::Base::BaseFilter<T>
{
	using T1 = ScalableFilterValue<T>;

private:
	IntroSatLib::Base::BaseFilter<T1> &_innerFilter;
	int8_t _minScale;
	int8_t _maxScale;
	int8_t _nowScale = 0;
	uint8_t _needUpScale = 0;
	uint8_t _canDownScale = 0;

public:
	ScalableFilter(IntroSatLib::Base::BaseFilter<T1> &innerFilter,
			int8_t maxScale) :
			_innerFilter(innerFilter)
	{
		_minScale = 0;
		_maxScale = maxScale;
	}

	ScalableFilter(IntroSatLib::Base::BaseFilter<T1> &innerFilter,
			int8_t minScale, int8_t maxScale) :
			_innerFilter(innerFilter)
	{
		_minScale = minScale;
		_maxScale = maxScale;
	}

	uint8_t Size() const override;

	uint8_t NeedUpScale() const;
	uint8_t CanDownScale() const;

	void UpScale();
	void DownScale();

	T1 ScaledValue() const;

	T operator[](uint8_t index) override;

	T AddValue(T newValue) override
	{
		T1 calcValue = newValue << (_nowScale - _minScale);

		_innerFilter.AddValue(calcValue);

		T1 TMaxValue = std::numeric_limits<T>::max();

		_needUpScale = 0;
		if ((_nowScale < _maxScale) && (abs(newValue) > (TMaxValue >> 1)))
		{
			_needUpScale = 1;
			return newValue;
		}

		_canDownScale = 0;
		if (_nowScale > _minScale)
		{
			uint8_t size = _innerFilter.Size();
			_canDownScale = 1;
			T1 MaxAbsValue = TMaxValue << ((_nowScale - _minScale) - 1);
			for (uint8_t i = 0; i < size; i++)
			{
				if (MaxAbsValue < abs(_innerFilter[i]))
				{
					_canDownScale = 0;
					break;
				}
			}
		}

		return newValue;
	}

};

}
}

#endif /* FILTERS_SCALABLEFILTER_H_ */
