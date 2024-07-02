#ifndef HDLC_HOLDERS_UIHOLDER_H_
#define HDLC_HOLDERS_UIHOLDER_H_

#include "../Base/Holder.h"

namespace IntroSatLib {
namespace HDLC {
namespace Holders {

class UIHolder: public Base::Holder
{
private:
	static const uint8_t UICommandByte = 0x13;
	static const uint8_t UICommandByteBroadcast = 0x03;

	const uint8_t _address = 0x00;

protected:
	constexpr
	UIHolder(uint8_t address) noexcept:
		Base::Holder(),
		_address(address) { }

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
		if (countParams == 0) { return 0; }

		if (ByteConverter::ToUInt8(commandParams, end) == _address) { return 1; }

		return 0;
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
