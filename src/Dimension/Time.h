#ifndef DIMENSION_TIME_H_
#define DIMENSION_TIME_H_

#include <stdint.h>

namespace IntroSatLib {

class Time {

private:
	int64_t _value;

public:
	explicit Time(int64_t value): _value(value) {};

	inline const float Value() const
	{
		return _value / 1000.0;
	}

	friend inline const Time operator+ (const Time& a);
	friend inline const Time operator- (const Time& a);

	friend inline const Time operator+ (const Time& a, const Time& b);
	friend inline const Time operator- (const Time& a, const Time& b);

	friend inline const Time operator* (const Time& a, const float& b);
	friend inline const Time operator/ (const Time& a, const float& b);

	friend inline const float operator/ (const Time& a, const Time& b);
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wliteral-suffix"

inline const Time operator""ms (unsigned long long int a)
{
	return Time(a);
}

inline const Time operator""s (long double a)
{
	return Time(a * 1000);
}

inline const Time operator""s (unsigned long long int a)
{
	return Time(a * 1000);
}

#pragma GCC diagnostic pop

const Time operator+ (const Time& a)
{
	return Time(a._value);
}
const Time operator- (const Time& a)
{
	return Time(-a._value);
}

const Time operator+ (const Time& a, const Time& b)
{
	return Time(a._value + b._value);
}
const Time operator- (const Time& a, const Time& b)
{
	return Time(a._value - b._value);
}

const Time operator* (const Time& a, const float& b)
{
	return Time(a._value * b);
}
const Time operator/ (const Time& a, const float& b)
{
	return Time(a._value / b);
}

const float operator/ (const Time& a, const Time& b)
{
	return a._value / b._value;
}

}

#endif /* DIMENSION_TIME_H_ */
