#ifndef BASE_GETTYPEID_H_
#define BASE_GETTYPEID_H_

#include <cinttypes>

namespace IntroSatLib {
namespace Base {

	using TypeId = std::uint32_t;

	template<class T>
	constexpr TypeId
	GetTypeId() noexcept
	{ return reinterpret_cast<TypeId>(&GetTypeId<T>); }

} /* namespace Base */
} /* namespace IntroSatLib */

#endif /* BASE_GETTYPEID_H_ */
