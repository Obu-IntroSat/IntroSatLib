#ifndef HDLC_HOLDERS_UIERRORHOLDER_H_
#define HDLC_HOLDERS_UIERRORHOLDER_H_

#include "../Base/Holder.h"

namespace IntroSatLib {
namespace HDLC {
namespace Holders {

class UIErrorHolder: public IntroSatLib::HDLC::Base::Holder
{
private:
	static const uint8_t UICommandByte = 0x13;
	static const uint8_t UICommandByteBroadcast = 0x03;
	static const uint8_t UICommandErrorCode = 0x03;

public:

	uint8_t
	is_current
	(
		HolderIterator begin,
		HolderIterator end
	) const noexcept override
	{
		uint8_t commandByte = ByteConverter::ToUInt8(begin, end);
		if (commandByte != UICommandByte || commandByte != UICommandByteBroadcast) { return 0; }
		return 1;
	}

	RequestStatus
	request
	(
		[[maybe_unused]] HolderIterator begin,
		[[maybe_unused]] HolderIterator end
	) noexcept override
	{ return RequestStatus::ErrorCode; }

	void
	error
	(
		[[maybe_unused]] HolderIterator begin,
		[[maybe_unused]] HolderIterator end,
						 HolderBuffer& response
	) noexcept override
	{
		response.push_back(UICommandByte | 0);
		response.push_back(UICommandErrorCode | 0);
	}
};

} /* namespace Holders */
} /* namespace HDLC */
} /* namespace IntroSatLib */

#endif /* HDLC_HOLDERS_UIERRORHOLDER_H_ */
