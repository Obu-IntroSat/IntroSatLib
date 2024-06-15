#ifndef HDLC_HOLDERS_UIHOLDER_H_
#define HDLC_HOLDERS_UIHOLDER_H_

#include "../Base/Holder.h"

namespace IntroSatLib {
namespace HDLC {
namespace Holders {

class UIHolder: public IntroSatLib::HDLC::Base::Holder
{
protected:
	static const uint8_t UICommandByte = 0x13;
	static const uint8_t UICommandByteBroadcast = 0x03;

	virtual uint8_t
	is_current_params
	(
		[[maybe_unused]] HolderIterator begin,
		[[maybe_unused]] HolderIterator end
	) const noexcept
	{ return 0; }

	virtual RequestStatus
	request_params
	(
		[[maybe_unused]] uint8_t params,
		[[maybe_unused]] HolderIterator begin,
		[[maybe_unused]] HolderIterator end
	) noexcept
	{ return RequestStatus::Ok; }

	virtual void
	response_params
	(
		[[maybe_unused]] uint8_t params,
		[[maybe_unused]] HolderIterator begin,
		[[maybe_unused]] HolderIterator end,
		[[maybe_unused]] HolderBuffer& response
	) noexcept { }

	virtual void
	error_params
	(
		[[maybe_unused]] uint8_t params,
		[[maybe_unused]] HolderIterator begin,
		[[maybe_unused]] HolderIterator end,
		[[maybe_unused]] HolderBuffer& response
	) noexcept { }

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
		HolderIterator commandParams = begin + 1;
		uint8_t countParams = distance(commandParams, end);
		return countParams > 0 && is_current_params(commandParams, end);
	}

	RequestStatus
	request
	(
		HolderIterator begin,
		HolderIterator end
	) noexcept override
	{
		HolderIterator paramsStart = begin + 2;
		uint8_t countParams = distance(paramsStart, end);
		return request_params(countParams, paramsStart, end);
	}

	void
	response
	(
		HolderIterator begin,
		HolderIterator end,
		HolderBuffer& response
	) noexcept override
	{
		HolderIterator paramsStart = begin + 2;
		uint8_t countParams = distance(paramsStart, end);
		response.push_back(UICommandByte | 0);
		response_params(countParams, paramsStart, end, response);
	}

	void
	error
	(
		HolderIterator begin,
		HolderIterator end,
		HolderBuffer& response
	) noexcept override
	{
		HolderIterator paramsStart = begin + 2;
		uint8_t countParams = distance(paramsStart, end);
		response.push_back(UICommandByte | 0);
		error_params(countParams, paramsStart, end, response);
	}
};

} /* namespace Holders */
} /* namespace HDLC */
} /* namespace IntroSatLib */

#endif /* HDLC_HOLDERS_UIHOLDER_H_ */
