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
		HolderIterator begin,
		HolderIterator end
	) const noexcept override
	{ return ByteConverter::ToUInt8(begin, end) == TestCommandByte; }

	RequestStatus request
	(
		[[maybe_unused]] HolderIterator begin,
		[[maybe_unused]] HolderIterator end
	) noexcept override
	{ return RequestStatus::CanResponce; }

	void response
	(
		HolderIterator begin,
		HolderIterator end,
		HolderBuffer& response
	) noexcept override
	{
		for(HolderIterator it = begin; it != end; it++)
		{
			response.push_back(*it);
		}
	}
};

} /* namespace Holders */
} /* namespace HDLC */
} /* namespace IntroSatLib */

#endif /* HDLC_HOLDERS_TESTHOLDER_H_ */
