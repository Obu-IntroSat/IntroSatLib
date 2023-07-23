#ifndef DIMENSION_DIVIDE_H_
#define DIMENSION_DIVIDE_H_

#include "Dimension/Multiply.h"

namespace IntroSatLib
{

template<typename T11, typename T12>
class Divide
{

private:
	float _value;

public:

	Divide<T11, T12>(float value) :
			_value(value)
	{
	}
	;
	Divide<T11, T12>(const Divide<T12, T11> &value) :
			_value(1 / value._value)
	{
	}
	;

	inline const float Value() const
	{
		return _value;
	}

	template<typename T1, typename T2>
	friend inline const Divide<T1, T2> operator+(const Divide<T1, T2> &a);
	template<typename T1, typename T2>
	friend inline const Divide<T1, T2> operator-(const Divide<T1, T2> &a);

	template<typename T1, typename T2>
	friend inline const Divide<T1, T2> operator+(const Divide<T1, T2> &a,
			Divide<T1, T2> &b);
	template<typename T1, typename T2>
	friend inline const Divide<T1, T2> operator-(const Divide<T1, T2> &a,
			Divide<T1, T2> &b);

	template<typename T1, typename T2>
	friend inline const Divide<T1, T2> operator*(const Divide<T1, T2> &a,
			const float &b);
	template<typename T1, typename T2>
	friend inline const Divide<T1, T2> operator*(const float &a,
			const Divide<T1, T2> &b);

	template<typename T1, typename T2>
	friend inline const Divide<T1, T2> operator/(const Divide<T1, T2> &a,
			const float &b);
	template<typename T1, typename T2>
	friend inline const Divide<T2, T1> operator/(const float &a,
			const Divide<T1, T2> &b);

	template<typename T1, typename T2>
	friend inline const Divide<T1, T2> operator/(const T1 &a, const T2 &b);

	template<typename T1, typename T2>
	friend inline const Multiply<Divide<T1, T2>, T1> operator*(
			const Divide<T1, T2> &a, const T1 &b);
	template<typename T1, typename T2>
	friend inline const T1 operator*(const Divide<T1, T2> &a, const T2 &b);

	template<typename T1, typename T2>
	friend inline const Multiply<Divide<T1, T2>, Divide<T1, T2>> operator*(
			const Divide<T1, T2> &a, const Divide<T1, T2> &b);
	template<typename T1, typename T2>
	friend inline const float operator/(const Divide<T1, T2> &a,
			const Divide<T1, T2> &b);

//	template<typename T1, typename T2>
//	friend inline const float operator* (const Divide<T1, T2>& a, const Divide<T2, T1>& b);
	template<typename T1, typename T2>
	friend inline const Multiply<Divide<T1, T2>, Divide<T1, T2>> operator/(
			const Divide<T1, T2> &a, const Divide<T2, T1> &b);

};

template<typename T1, typename T2>
inline const Divide<T1, T2> operator+(const Divide<T1, T2> &a)
{
	return Divide<T1, T2>(a.Value());
}
template<typename T1, typename T2>
inline const Divide<T1, T2> operator-(const Divide<T1, T2> &a)
{
	return Divide<T1, T2>(-a.Value());
}

template<typename T1, typename T2>
inline const Divide<T1, T2> operator+(const Divide<T1, T2> &a,
		Divide<T1, T2> &b)
{
	return Divide<T1, T2>(a.Value() + b.Value());
}
template<typename T1, typename T2>
inline const Divide<T1, T2> operator-(const Divide<T1, T2> &a,
		Divide<T1, T2> &b)
{
	return Divide<T1, T2>(a.Value() - b.Value());
}

template<typename T1, typename T2>
inline const Divide<T1, T2> operator*(const Divide<T1, T2> &a, const float &b)
{
	return Divide<T1, T2>(a.Value() * b);
}
template<typename T1, typename T2>
inline const Divide<T1, T2> operator/(const Divide<T1, T2> &a, const float &b)
{
	return Divide<T1, T2>(a.Value() / b);
}

template<typename T1, typename T2>
inline const Divide<T1, T2> operator/(const T1 &a, const T2 &b)
{
	return Divide<T1, T2>(a.Value() / b.Value());
}

template<typename T1, typename T2>
inline const T1 operator*(const Divide<T1, T2> &a, const T2 &b)
{
	return T1(a.Value() * b.Value());
}

template<typename T1, typename T2>
inline const float operator/(const Divide<T1, T2> &a, const Divide<T1, T2> &b)
{
	return a.Value() / b.Value();
}

template<typename T1, typename T2>
inline const Multiply<Divide<T1, T2>, Divide<T1, T2>> operator/(
		const Divide<T1, T2> &a, const Divide<T2, T1> &b)
{
	return Multiply<Divide<T1, T2>, Divide<T1, T2>>(a.Value() / b.Value());
}

}

#endif /* DIMENSION_DIVIDE_H_ */
