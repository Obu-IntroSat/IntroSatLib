#ifndef DIMENSION_DEGREE_H_
#define DIMENSION_DEGREE_H_

namespace IntroSatLib
{

class Degree
{

private:
	int64_t _value;

public:

	explicit Degree(int64_t value) :
			_value(value * 1000)
	{
	}
	;
	explicit Degree(float value) :
			_value(value * 1000)
	{
	}
	;

	inline const float Value() const
	{
		return _value;
	}

	friend inline const Degree operator+(const Degree &a);
	friend inline const Degree operator-(const Degree &a);

	friend inline const Degree operator+(const Degree &a, const Degree &b);
	friend inline const Degree operator-(const Degree &a, const Degree &b);

	friend inline const Degree operator*(const Degree &a, const float &b);
	friend inline const Degree operator/(const Degree &a, const float &b);

	friend inline const float operator/(const Degree &a, const Degree &b);
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wliteral-suffix"

const inline Degree operator""deg(unsigned long long int a)
{
	return Degree(a);
}
const inline Degree operator""deg(long double a)
{
	return Degree(a);
}

#pragma GCC diagnostic pop

const Degree operator+(const Degree &a)
{
	return Degree(a._value);
}
const Degree operator-(const Degree &a)
{
	return Degree(-a._value);
}

const Degree operator+(const Degree &a, const Degree &b)
{
	return Degree(a._value + b._value);
}
const Degree operator-(const Degree &a, const Degree &b)
{
	return Degree(a._value - b._value);
}

const Degree operator*(const Degree &a, const float &b)
{
	return Degree(a._value * b);
}
const Degree operator/(const Degree &a, const float &b)
{
	return Degree(a._value / b);
}

const float operator/(const Degree &a, const Degree &b)
{
	return a._value / b._value;
}

}

#endif /* DIMENSION_DEGREE_H_ */
