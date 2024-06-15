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
		[[maybe_unused]] PhysicsIterator begin,
		[[maybe_unused]] PhysicsIterator end
	) const noexcept
	{ return 0; }

	virtual RequestStatus
	request_params
	(
		[[maybe_unused]] uint8_t params,
		[[maybe_unused]] PhysicsIterator begin,
		[[maybe_unused]] PhysicsIterator end
	) noexcept
	{ return RequestStatus::Ok; }

	virtual void
	response_params
	(
		[[maybe_unused]] uint8_t params,
		[[maybe_unused]] PhysicsIterator begin,
		[[maybe_unused]] PhysicsIterator end,
		[[maybe_unused]] std::vector<uint8_t>& response
	) noexcept { }

	virtual void
	error_params
	(
		[[maybe_unused]] uint8_t params,
		[[maybe_unused]] PhysicsIterator begin,
		[[maybe_unused]] PhysicsIterator end,
		[[maybe_unused]] std::vector<uint8_t>& response
	) noexcept { }

public:

	uint8_t
	is_current
	(
		PhysicsIterator begin,
		PhysicsIterator end
	) const noexcept override
	{
		uint8_t commandByte = ByteConverter::ToUInt8(begin, end);
		if (commandByte != UICommandByte || commandByte != UICommandByteBroadcast) { return 0; }
		PhysicsIterator commandParams = begin + 1;
		uint8_t countParams = distance(commandParams, end);
		return countParams > 0 && is_current_params(commandParams, end);
	}

	RequestStatus
	request
	(
		PhysicsIterator begin,
		PhysicsIterator end
	) noexcept override
	{
		PhysicsIterator paramsStart = begin + 2;
		uint8_t countParams = distance(paramsStart, end);
		return request_params(countParams, paramsStart, end);
	}

	void
	response
	(
		PhysicsIterator begin,
		PhysicsIterator end,
		std::vector<uint8_t>& response
	) noexcept override
	{
		PhysicsIterator paramsStart = begin + 2;
		uint8_t countParams = distance(paramsStart, end);
		response.push_back(UICommandByte | 0);
		response_params(countParams, paramsStart, end, response);
	}

	void
	error
	(
		PhysicsIterator begin,
		PhysicsIterator end,
		std::vector<uint8_t>& response
	) noexcept override
	{
		PhysicsIterator paramsStart = begin + 2;
		uint8_t countParams = distance(paramsStart, end);
		response.push_back(UICommandByte | 0);
		error_params(countParams, paramsStart, end, response);
	}
};

} /* namespace Holders */
} /* namespace HDLC */
} /* namespace IntroSatLib */

#endif /* HDLC_HOLDERS_UIHOLDER_H_ */
