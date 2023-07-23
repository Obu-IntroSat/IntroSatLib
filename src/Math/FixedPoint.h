#ifndef MATH_FIXEDPOINT_H_
#define MATH_FIXEDPOINT_H_

#include <cstdint>
#include <cmath>
#include <climits>
#include <type_traits>

namespace IntroSatLib
{
namespace Math
{

template<typename Type, typename = typename std::enable_if<std::is_integral<Type>::value>::type>
class FixedPoint
{
private:

	const static uint8_t _bitCount = CHAR_BIT * sizeof(Type);
	const static uint8_t _bitHalfCount = _bitCount >> 1;

	const static Type _halfTypeScaler = 1 << _bitHalfCount;
	const static Type _halfLowMask = _halfTypeScaler - 1;
	const static Type _halfHighMask = ~_halfLowMask;

	const static uint8_t _bitWise = _bitCount >> 2;
	const static Type _scaler = 1 << _bitWise;

	const static Type _roundMask = _scaler >> 1;

	const static Type _fractionalPart = _scaler - 1;
	const static Type _integerPart = ~_fractionalPart;

	Type _realValue = 0;

public:

	constexpr FixedPoint()
	{
		_realValue = 0;
	}

	template<typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
	constexpr FixedPoint(T value)
	{
		_realValue = static_cast<Type>(value) << _bitWise;
	}

	constexpr FixedPoint(float value)
	{
		_realValue = static_cast<Type>(llroundf(value * _scaler));
	}

	constexpr FixedPoint(double value)
	{
		_realValue = static_cast<Type>(llround(value * _scaler));
	}

	constexpr FixedPoint(long double value)
	{
		_realValue = static_cast<Type>(llroundl(value * _scaler));
	}

	template<typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
	constexpr FixedPoint(FixedPoint<T> value)
	{
		if constexpr (value._bitWise < _bitWise)
		{
			_realValue = static_cast<Type>(value._realValue) << (_bitWise - value._bitWise);
		}
		else
		{
			_realValue = static_cast<Type>(value._realValue >> (value._bitWise - _bitWise));
		}
	}

	template<typename = Type>
	constexpr FixedPoint(FixedPoint<Type> value)
	{
		_realValue = value._realValue;
	}

	constexpr FixedPoint& operator+() const
	{
	   FixedPoint result;
	   result._realValue = _realValue;
	   return result;
	}
	constexpr FixedPoint& operator-() const
	{
	   FixedPoint result;
	   result._realValue = -_realValue;
	   return result;
	}

	constexpr FixedPoint& operator++()
	{
		_realValue++;
	    return *this;
	}

	constexpr FixedPoint& operator++(int) const
	{
		FixedPoint result(this);
		_realValue++;
	    return result;
	}

	constexpr FixedPoint& operator--()
	{
		_realValue--;
	    return *this;
	}

	constexpr FixedPoint operator--(int) const
	{
		FixedPoint result(this);
	    _realValue--;
	    return result;
	}

	constexpr FixedPoint operator+(const FixedPoint& other) const
	{
	    FixedPoint result;
	    result._realValue = _realValue + other._realValue;
	    return result;
	}

	constexpr FixedPoint& operator+=(const FixedPoint& other)
	{
		_realValue += other._realValue;
		return *this;
	}

	constexpr FixedPoint operator-(const FixedPoint& other) const
	{
	    FixedPoint result;
	    result._realValue = _realValue - other._realValue;
	    return result;
	}

	constexpr FixedPoint& operator-=(const FixedPoint& other)
	{
		_realValue -= other._realValue;
		return *this;
	}

	constexpr FixedPoint operator*(const FixedPoint& other) const
	{
		FixedPoint result;
		if constexpr (std::is_signed<Type>::value)
		{
			bool expLeft = _realValue > 0;
			bool expRight = other._realValue > 0;

			Type left = expLeft ? _realValue : -_realValue;
			Type right = expRight ? other._realValue : -other._realValue;

			Type parts[4] = {
				static_cast<Type>((left & _halfLowMask) * (right & other._halfLowMask)),
				static_cast<Type>((left >> _bitHalfCount) * (right & other._halfLowMask)),
				static_cast<Type>((left & _halfLowMask) * (right >> other._bitHalfCount)),
				static_cast<Type>((left >> _bitHalfCount) * (right >> other._bitHalfCount))
			};
			Type centerPart = parts[1] + parts[2];
			Type endPart = parts[0] + (centerPart << _bitHalfCount);
			Type startPart = parts[3] + (centerPart >> _bitHalfCount);

			if (endPart & _roundMask)
			{
				endPart += _roundMask;
			}

			Type resultValue = (endPart >> _bitWise) + (startPart << (_bitHalfCount - _bitWise));

			result._realValue = expLeft ^ expRight ? -resultValue : resultValue;
		}
		else
		{

			Type left = _realValue;
			Type right = other._realValue;

			Type parts[4] = {
				static_cast<Type>((left & _halfLowMask) * (right & other._halfLowMask)),
				static_cast<Type>((left >> _bitHalfCount) * (right & other._halfLowMask)),
				static_cast<Type>((left & _halfLowMask) * (right >> other._bitHalfCount)),
				static_cast<Type>((left >> _bitHalfCount) * (right >> other._bitHalfCount))
			};
			Type centerPart = parts[1] + parts[2];
			Type endPart = parts[0] + (centerPart << _bitHalfCount);
			Type startPart = parts[3] + (centerPart >> _bitHalfCount);

			if (endPart & _roundMask)
			{
				endPart += _roundMask;
			}

			Type resultValue = (endPart >> _bitWise) + (startPart << (_bitHalfCount - _bitWise));

			result._realValue = resultValue;
		}
		return result;
	}

	constexpr FixedPoint& operator*=(const FixedPoint& other)
	{
		return *this;
	}
	constexpr FixedPoint operator/(const FixedPoint& other) const
	{
	    FixedPoint result;
	    return result;
	}

	constexpr FixedPoint& operator/=(const FixedPoint& other)
	{
		return *this;
	}

	constexpr float ToFloat()
	{
		return (float)_realValue / (float)_scaler;
	}

};

} /* namespace Base */
} /* namespace IntroSatLib */

#endif /* MATH_FIXEDPOINT_H_ */
