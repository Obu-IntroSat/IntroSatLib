#ifndef BASE_MEMORY_STATIC_PTR_H_
#define BASE_MEMORY_STATIC_PTR_H_

#include <stdint.h>
#include <vector>

namespace IntroSatLib {
namespace Base {
namespace Memory {

template<typename T>
class static_ptr
{

private:
	T* _ref;

public:
	constexpr
	static_ptr(T* ref) noexcept : _ref(ref) { }

	constexpr
	static_ptr(const static_ptr<T>& ptr) noexcept
	{ _ref = ptr._ref; }

	constexpr static_ptr<T>&
	operator=(const static_ptr<T>& other) noexcept
	{
		if (this != &other) { _ref = other._ref; }
		return *this;
	}

	constexpr
	static_ptr(static_ptr<T>&& ptr) noexcept
	{
		_ref = ptr._ref;
	}

	static_ptr<T>&
	operator=(static_ptr<T>&& other) noexcept
	{
		if (this != &other) { _ref = other._ref; }
		return *this;
	}

	constexpr T*
	operator->() const noexcept
	{ return get(); }

	constexpr T*
	get() const noexcept
	{ return _ref; }

	~static_ptr() { }

};

} /* namespace Memory */
} /* namespace Base */
} /* namespace IntroSatLib */

#endif /* BASE_MEMORY_STATIC_PTR_H_ */
