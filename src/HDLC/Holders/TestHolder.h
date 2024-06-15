#ifndef HDLC_HOLDERS_TESTHOLDER_H_
#define HDLC_HOLDERS_TESTHOLDER_H_

#include "../Base/Holder.h"

namespace IntroSatLib {
namespace HDLC {
namespace Holders {

class TestHolder: public IntroSatLib::HDLC::Base::Holder
{

private:

	static const uint8_t TestCommandByte = 0xF3;

protected:

	uint8_t is_current
	(
		PhysicsIterator begin,
		PhysicsIterator end
	) const noexcept override
	{ return ByteConverter::ToUInt8(begin, end) == TestCommandByte; }

	RequestStatus request
	(
		[[maybe_unused]] PhysicsIterator begin,
		[[maybe_unused]] PhysicsIterator end
	) noexcept override
	{ return RequestStatus::CanResponce; }

	void response
	(
		PhysicsIterator begin,
		PhysicsIterator end,
		std::vector<uint8_t>& response
	) noexcept override
	{
		for(PhysicsIterator it = begin; it != end; it++)
		{
			response.push_back(*it);
		}
	}
};

} /* namespace Holders */
} /* namespace HDLC */
} /* namespace IntroSatLib */

#endif /* HDLC_HOLDERS_TESTHOLDER_H_ */
