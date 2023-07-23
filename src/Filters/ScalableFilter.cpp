#include "./ScalableFilter.h"

namespace IntroSatLib
{
namespace Filters
{
template<typename T>
uint8_t ScalableFilter<T>::Size() const
{
	return _innerFilter.Size();
}

template<typename T>
uint8_t ScalableFilter<T>::NeedUpScale() const
{
	return _needUpScale;
}

template<typename T>
uint8_t ScalableFilter<T>::CanDownScale() const
{
	return _canDownScale;
}

template<typename T>
void ScalableFilter<T>::UpScale()
{
	if (_nowScale < _maxScale)
	{
		_nowScale++;
	}
}

template<typename T>
void ScalableFilter<T>::DownScale()
{
	if (_nowScale > _minScale)
	{
		_nowScale--;
	}
}

template<typename T>
ScalableFilterValue<T> ScalableFilter<T>::ScaledValue() const
{
	return _innerFilter[0];
}

template<typename T>
T ScalableFilter<T>::operator[](uint8_t index)
{
	return _innerFilter[index] >> (_nowScale - _minScale);
}
}
}
