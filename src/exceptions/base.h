#ifndef EXCEPTIONS_BASE_H_
#define EXCEPTIONS_BASE_H_


#include <cinttypes>
#include <type_traits>
#include "../memory/shared_prt.h"
#include "../../third_party/etl/include/etl/generic_pool.h"

namespace is {
namespace exceptions {

class base
{
public:
	typedef const char*					string_type;
	typedef std::uint32_t				numeric_type;

	class exception_ref: public is::memory::shared_ptr<base>
	{

	public:
		exception_ref(): is::memory::shared_ptr<base>() { };

		exception_ref
		(
			is::memory::shared_object<base>* object,
			etl::ipool* pool
		): shared_ptr<base>(object, pool) { };

		constexpr string_type
		text() const noexcept
		{ return get()->text(); }

		constexpr string_type
		file() const noexcept
		{ return get()->file(); }

		constexpr numeric_type
		line() const noexcept
		{ return get()->line(); }

		exception_ref next(string_type text, string_type file, numeric_type line)
		{
			exception_ref result = is::memory::make_shared<base, exception_ref>(&pool, text, file, line);
			result->_ref = *this;
			return result;
		}
		~exception_ref() { }

	};
private:
	static inline etl::generic_pool<32, 8, 20> pool;

public:
	string_type		_text = 0;
private:
	string_type		_file = 0;
	numeric_type	_line = 0;
	exception_ref	_ref;

public:
	base(string_type text, string_type file, numeric_type line):
		_text(text), _file(file), _line(line) { }

public:
	constexpr string_type text() const noexcept { return _text; }

	constexpr string_type file() const noexcept { return _file; }

	constexpr numeric_type line() const noexcept { return _line; }

	static exception_ref create(string_type text, string_type file, numeric_type line)
	{
		return is::memory::make_shared<base, exception_ref>(&pool, text, file, line);
	}

	~base() { _ref = exception_ref(); }
};

} /* namespace exceptions */
} /* namespace is */

#endif /* EXCEPTIONS_BASE_H_ */
