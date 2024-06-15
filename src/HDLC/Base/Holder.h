#ifndef HDLC_BASE_HOLDER_H_
#define HDLC_BASE_HOLDER_H_

#include <inttypes.h>
#include <vector>
#include "./Physics.h"
#include "../../Base/ByteConverter.h"

namespace IntroSatLib {
namespace HDLC {
namespace Base {

class Holder
{
public:
	enum class RequestStatus: uint8_t
	{
		Ok = 0,
		ErrorCode = 1,
		CanResponce = 0x40,
		CantNextCode = 0x80
	};
protected:
	using PhysicsIterator = Physics::iterator;
	using ByteConverter = IntroSatLib::Base::ByteConverterLittleEndian;

	constexpr static uint8_t
	distance
	(
		PhysicsIterator begin,
		PhysicsIterator end
	) noexcept
	{ return static_cast<uint8_t>(std::distance(begin, end)); }

public:
	virtual uint8_t
	is_current
	(
		[[maybe_unused]] PhysicsIterator begin,
		[[maybe_unused]] PhysicsIterator end
	) const noexcept
	{
		return 0;
	}

	virtual RequestStatus
	request
	(
		[[maybe_unused]] PhysicsIterator begin,
		[[maybe_unused]] PhysicsIterator end
	) noexcept { return RequestStatus::Ok; }

	virtual void
	response
	(
		[[maybe_unused]] PhysicsIterator begin,
		[[maybe_unused]] PhysicsIterator end,
		[[maybe_unused]] std::vector<uint8_t>& response
	) noexcept { }

	virtual void
	error
	(
		[[maybe_unused]] PhysicsIterator begin,
		[[maybe_unused]] PhysicsIterator end,
		[[maybe_unused]] std::vector<uint8_t>& response
	) noexcept { }

	virtual ~Holder() = default;
};

constexpr Holder::RequestStatus
operator &
(
	Holder::RequestStatus left,
	Holder::RequestStatus rigth
) noexcept
{ return static_cast<Holder::RequestStatus>(static_cast<uint8_t>(left) & static_cast<uint8_t>(rigth)); }

constexpr Holder::RequestStatus
operator |
(
	Holder::RequestStatus left,
	Holder::RequestStatus rigth
) noexcept
{ return static_cast<Holder::RequestStatus>(static_cast<uint8_t>(left) | static_cast<uint8_t>(rigth)); }

} /* namespace Base */
} /* namespace HDLC */
} /* namespace IntroSatLib */

#endif /* HDLC_BASE_HOLDER_H_ */
