#ifndef BASE_RESULT_H_
#define BASE_RESULT_H_

#include <cstdint>
#include <type_traits>

namespace IntroSatLib
{
namespace Base
{
template <typename OutputType, typename = typename std::enable_if<!std::is_void<OutputType>::value>::type>
class Result
{
private:
	std::uint8_t _isResolved;
	union
	{
		OutputType& _result;
		uint16_t& _error;
	};
public:
	constexpr Result(uint16_t& error): _isResolved(0), _error(error) { }
	constexpr Result(OutputType& result): _isResolved(1), _result(result) { }
};


} /* namespace Base */
} /* namespace IntroSatLib */

#endif /* BASE_RESULT_H_ */
