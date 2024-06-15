#ifndef HDLC_HOLDERS_DISCHOLDER_H_
#define HDLC_HOLDERS_DISCHOLDER_H_

#include "../Base/Holder.h"

namespace IntroSatLib {
namespace HDLC {
namespace Holders {

class DISCHolder: public IntroSatLib::HDLC::Base::Holder
{
private:
	static const uint8_t DISCCommandByte = 0x53;
	static const uint8_t DISCResponseByte = 0x73;

private:
	uint8_t _needStop = 0;

public:

	constexpr uint8_t
	need_stop() const noexcept
	{ return _needStop; }

public:

	uint8_t
	is_current
	(
		HolderIterator begin,
		HolderIterator end
	) const noexcept override
	{ return ByteConverter::ToUInt8(begin, end) == DISCCommandByte; }

	RequestStatus
	request
	(
		[[maybe_unused]] HolderIterator begin,
		[[maybe_unused]] HolderIterator end
	) noexcept override
	{
		_needStop = 1;
		return RequestStatus::CantNextCode;
	}

	void
	response
	(
		[[maybe_unused]] HolderIterator begin,
		[[maybe_unused]] HolderIterator end,
						 HolderBuffer& response
	) noexcept override
	{
		uint8_t value = DISCResponseByte;
		response.push_back(value);
	}
};

} /* namespace Holders */
} /* namespace HDLC */
} /* namespace IntroSatLib */

#endif /* HDLC_HOLDERS_DISCHOLDER_H_ */
