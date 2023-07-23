#ifndef DIMENSION_DISTANCE_H_
#define DIMENSION_DISTANCE_H_

#include <stdint.h>

namespace IntroSatLib
{

class Distance
{

private:
	int64_t _value;

public:
	explicit Distance(int64_t value) :
			_value(value * 1000000)
	{
	}
	;
	explicit Distance(float value) :
			_value(value * 1000000)
	{
	}
	;

	inline const float Value() const
	{
		return _value / 1000000.0;
	}

	friend inline const Distance operator+(const Distance &a);
	friend inline const Distance operator-(const Distance &a);

	friend inline const Distance operator+(const Distance &a,
			const Distance &b);
	friend inline const Distance operator-(const Distance &a,
			const Distance &b);

	friend inline const Distance operator*(const Distance &a, const float &b);
	friend inline const Distance operator/(const Distance &a, const float &b);

	friend inline const float operator/(const Distance &a, const Distance &b);
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wliteral-suffix"

const inline Distance operator""mm(unsigned long long int a)
{
	return Distance((int64_t) a);
}
const inline Distance operator""m(long double a)
{
	return Distance((float) a * 1000);
}
const inline Distance operator""m(unsigned long long int a)
{
	return Distance((int64_t) a * 1000);
}

#pragma GCC diagnostic pop

const Distance operator+(const Distance &a)
{
	return Distance(a._value);
}
const Distance operator-(const Distance &a)
{
	return Distance(-a._value);
}

const Distance operator+(const Distance &a, const Distance &b)
{
	return Distance(a._value + b._value);
}
const Distance operator-(const Distance &a, const Distance &b)
{
	return Distance(a._value - b._value);
}

const Distance operator*(const Distance &a, const float &b)
{
	return Distance(a._value * b);
}
const Distance operator/(const Distance &a, const float &b)
{
	return Distance(a._value / b);
}

const float operator/(const Distance &a, const Distance &b)
{
	return a._value / b._value;
}

}

#endif /* DIMENSION_DISTANCE_H_ */
