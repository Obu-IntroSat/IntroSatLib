#ifndef EXCEPTION_BASE_H_
#define EXCEPTION_BASE_H_


#include <cinttypes>
#include <type_traits>
#include "../memory/shared_prt.h"
#include "../../third_party/etl/include/etl/expected.h"
#include "../../third_party/etl/include/etl/generic_pool.h"


namespace is {
namespace exception {

class base
{
public:
	typedef const char*					string_type;
	typedef std::uint32_t				numeric_type;
	typedef memory::shared_ptr<base>	exception_ref;

private:
	static inline etl::generic_pool<32, 8, 20> _pool;

private:
	string_type		_text = 0;
	string_type		_file = 0;
	numeric_type	_line = 0;
	exception_ref	_ref;

	template<typename TException>
	friend exception_ref create(base::string_type file, base::numeric_type line);

	template<typename TException>
	friend exception_ref next(exception_ref prev, string_type file, numeric_type line);

public:
	base(string_type text, string_type file, numeric_type line):
		_text(text), _file(file), _line(line) { }

	constexpr string_type text() const noexcept { return _text; }

	constexpr string_type file() const noexcept { return _file; }

	constexpr numeric_type line() const noexcept { return _line; }
};

template<typename TException>
base::exception_ref create(base::string_type file, base::numeric_type line)
{
	return is::memory::make_shared<TException, base::exception_ref>(&base::_pool, file, line);
}

template<typename TException>
base::exception_ref next(base::exception_ref prev, base::string_type file, base::numeric_type line)
{
	base::exception_ref result = create<TException>(file, line);
	result->_ref = prev;
	return result;
}

template<typename TResult>
using result_type = etl::expected<TResult, base::exception_ref>;

using error_type = etl::unexpected<base::exception_ref>;

} /* namespace exception */
} /* namespace is */

#endif /* EXCEPTION_BASE_H_ */
