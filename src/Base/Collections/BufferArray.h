#ifndef BASE_COLLECTIONS_BUFFERARRAY_H_
#define BASE_COLLECTIONS_BUFFERARRAY_H_

#include <iterator>
#include <type_traits>

namespace IntroSatLib {
namespace Base {
namespace Collections {

template<typename _Tp, std::size_t _Nm>
struct __buffer_array_traits
{
	typedef _Tp _Type[_Nm];
	typedef std::is_swappable<_Tp> _Is_swappable;
	typedef std::is_nothrow_swappable<_Tp> _Is_nothrow_swappable;

	static constexpr _Tp&
	_S_ref(const _Type& __t, std::size_t __n) noexcept
	{ return const_cast<_Tp&>(__t[__n]); }

	static constexpr _Tp*
	_S_ptr(const _Type& __t) noexcept
	{ return const_cast<_Tp*>(__t); }
};

template<typename _Tp>
struct __buffer_array_traits<_Tp, 0>
{
	struct _Type { };
	typedef std::true_type _Is_swappable;
	typedef std::true_type _Is_nothrow_swappable;

	static constexpr _Tp&
	_S_ref(const _Type&, std::size_t) noexcept
	{ return *static_cast<_Tp*>(nullptr); }

	static constexpr _Tp*
	_S_ptr(const _Type&) noexcept
	{ return nullptr; }
};

template<typename _Tp, std::size_t _Nm>
class BufferArray
{

public:
	typedef _Tp										value_type;
	typedef value_type*								pointer;
	typedef const value_type*						const_pointer;
	typedef value_type&								reference;
	typedef const value_type&						const_reference;
	typedef value_type*								iterator;
	typedef const value_type*						const_iterator;
	typedef std::size_t								size_type;
	typedef std::ptrdiff_t							difference_type;
	typedef std::reverse_iterator<iterator>			reverse_iterator;
	typedef std::reverse_iterator<const_iterator>	const_reverse_iterator;

private:
	typedef __buffer_array_traits<_Tp, _Nm>			_AT_Type;
	typename _AT_Type::_Type						_M_elems;

	size_type _currentItem = 0;

public:
    constexpr iterator
    begin() noexcept
    { return iterator(data()); }

    constexpr const_iterator
    begin() const noexcept
    { return const_iterator(data()); }

    constexpr iterator
    end() noexcept
    { return iterator(data() + _currentItem); }

    constexpr const_iterator
    end() const noexcept
    { return const_iterator(data() + _currentItem); }

    constexpr reverse_iterator
    rbegin() noexcept
    { return reverse_iterator(end()); }

    constexpr const_reverse_iterator
    rbegin() const noexcept
    { return const_reverse_iterator(end()); }

    constexpr reverse_iterator
    rend() noexcept
    { return reverse_iterator(begin()); }

    constexpr const_reverse_iterator
    rend() const noexcept
    { return const_reverse_iterator(begin()); }

    constexpr const_iterator
    cbegin() const noexcept
    { return const_iterator(data()); }

    constexpr const_iterator
    cend() const noexcept
    { return const_iterator(data() + _currentItem); }

    constexpr const_reverse_iterator
    crbegin() const noexcept
    { return const_reverse_iterator(end()); }

    constexpr const_reverse_iterator
    crend() const noexcept
    { return const_reverse_iterator(begin()); }

    constexpr reference
    operator[](size_type __n) noexcept
    { return _AT_Type::_S_ref(_M_elems, __n); }

    constexpr const_reference
    operator[](size_type __n) const noexcept
    { return _AT_Type::_S_ref(_M_elems, __n); }

    constexpr size_type
    size() const noexcept { return _currentItem; }

    constexpr size_type
    max_size() const noexcept { return _Nm; }

    constexpr bool
    empty() const noexcept { return size() == 0; }

    constexpr void
	clear() noexcept
    { _currentItem = 0; }

    constexpr void
	push_back(const value_type& x)
    {
    	this->operator [](_currentItem) = x;
    	_currentItem++;
    }

    constexpr pointer
    data() noexcept
    { return _AT_Type::_S_ptr(_M_elems); }

    constexpr const_pointer
	data() const noexcept
	{ return _AT_Type::_S_ptr(_M_elems); }

};

} /* namespace Collections */
} /* namespace Base */
} /* namespace IntroSatLib */

#endif /* BASE_COLLECTIONS_BUFFERARRAY_H_ */
