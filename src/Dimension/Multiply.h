#ifndef DIMENSION_MULTIPLY_H_
#define DIMENSION_MULTIPLY_H_


namespace IntroSatLib {


template<typename T11, typename T12>
class Multiply
{
private:
	float _value;

public:
	Multiply<T11, T12>(float value): _value(value) { };
	Multiply<T11, T12>(const Multiply<T12, T11>& value): _value(value._value) { };

	inline const float Value() const
	{
		return _value;
	}

	template<typename T1, typename T2>
	friend inline const Multiply<T1, T2> operator+ (const Multiply<T1, T2>& a);
	template<typename T1, typename T2>
	friend inline const Multiply<T1, T2> operator- (const Multiply<T1, T2>& a);

	template<typename T1, typename T2>
	friend inline const Multiply<T1, T2> operator+ (const Multiply<T1, T2>& a, Multiply<T1, T2>& b);
	template<typename T1, typename T2>
	friend inline const Multiply<T1, T2> operator- (const Multiply<T1, T2>& a, Multiply<T1, T2>& b);

	template<typename T1, typename T2>
	friend inline const Multiply<T1, T2> operator* (const Multiply<T1, T2>& a, const float& b);
	template<typename T1, typename T2>
	friend inline const Multiply<T1, T2> operator* (const float& a, const Multiply<T1, T2>& b);

	template<typename T1, typename T2>
	friend inline const Multiply<T1, T2> operator/ (const Multiply<T1, T2>& a, const float& b);

	template<typename T1, typename T2>
	friend inline const Multiply<T1, T2> operator* (const T1& a, const T2& b);

	template<typename T1, typename T2>
	friend inline const T1 operator/ (const Multiply<T1, T2>& a, const T2& b);
	template<typename T1, typename T2>
	friend inline const T2 operator/ (const Multiply<T1, T2>& a, const T1& b);


	template<typename T1, typename T2>
	friend inline const Multiply<Multiply<T1, T2>, Multiply<T1, T2>> operator* (const Multiply<T1, T2>& a, const Multiply<T1, T2>& b);

	template<typename T1, typename T2>
	friend inline const float operator/ (const Multiply<T1, T2>& a, const Multiply<T1, T2>& b);
};

template<typename T1, typename T2>
const Multiply<T1, T2> operator+ (const Multiply<T1, T2>& a)
{
	return Multiply<T1, T2>(a.Value());
}
template<typename T1, typename T2>
const Multiply<T1, T2> operator- (const Multiply<T1, T2>& a)
{
	return Multiply<T1, T2>(-a.Value());
}

template<typename T1, typename T2>
const Multiply<T1, T2> operator+ (const Multiply<T1, T2>& a, Multiply<T1, T2>& b)
{
	return Multiply<T1, T2>(a.Value() + b.Value());
}
template<typename T1, typename T2>
const Multiply<T1, T2> operator- (const Multiply<T1, T2>& a, Multiply<T1, T2>& b)
{
	return Multiply<T1, T2>(a.Value() - b.Value());
}

template<typename T1, typename T2>
const Multiply<T1, T2> operator* (const Multiply<T1, T2>& a, const float& b)
{
	return Multiply<T1, T2>(a.Value() * b);
}
template<typename T1, typename T2>
const Multiply<T1, T2> operator/ (const Multiply<T1, T2>& a, const float& b)
{
	return Multiply<T1, T2>(a.Value() / b);
}

template<typename T1, typename T2>
const Multiply<T1, T2> operator* (const T1& a, const T2& b)
{
	return Multiply<T1, T2>(a.Value() * b.Value());
}

template<typename T1, typename T2>
inline const T1 operator/ (const Multiply<T1, T2>& a, const T2& b)
{
	return T1(a.Value() / b.Value());
}

template<typename T1, typename T2>
inline const T2 operator/ (const Multiply<T1, T2>& a, const T1& b)

{
	return T1(a.Value() / b.Value());
}

template<typename T1, typename T2>
inline const float operator/ (const Multiply<T1, T2>& a, const Multiply<T1, T2>& b)
{
	return a.Value() / b.Value();
}

}

#endif /* DIMENSION_MULTIPLY_H_ */
