#ifndef HDLC_HOLDERS_TIMEHOLDER_H_
#define HDLC_HOLDERS_TIMEHOLDER_H_

#include "UIHolder.h"

namespace IntroSatLib {
namespace HDLC {
namespace Holders {

class TimeHolder: public UIHolder
{
private:
	static constexpr uint64_t StartTime = uint64_t(1704056400) << 32;
	static const uint8_t TimeCommandByte = 0;
	static const uint8_t TimeCommandParams = 8;
	static const uint8_t UNIXFractionShift = 32;
	static const uint8_t UNIXBitInByte = 8;

	uint64_t _fullTime = 0;

public:

	constexpr uint32_t
	UNIX() const noexcept
	{ return static_cast<uint32_t>(_fullTime >> UNIXFractionShift); }

	constexpr uint32_t
	fractional_UNIX() const noexcept
	{ return static_cast<uint32_t>(_fullTime); }

	constexpr uint64_t
	full_time() const noexcept
	{ return _fullTime; }

private:

	constexpr static uint64_t
	convert_time
	(
		PhysicsIterator begin,
		PhysicsIterator end
	) noexcept
	{
		uint64_t time = 0;
		time |= static_cast<uint64_t>(ByteConverter::ToUInt32(begin, end)) << ByteConverter::Int32Shift;
		time |= static_cast<uint64_t>(ByteConverter::ToUInt32(begin + ByteConverter::Int32ByteCount, end));
		return time;
	}

protected:
	uint8_t
	is_current
	(
		PhysicsIterator begin,
		PhysicsIterator end
	) const noexcept override
	{ return ByteConverter::ToUInt8(begin, end) == TimeCommandByte; }

	RequestStatus
	request_params
	(
		uint8_t params,
		PhysicsIterator begin,
		PhysicsIterator end
	) noexcept override
	{
		if (params != TimeCommandParams) { return RequestStatus::ErrorCode; }

		uint64_t time = convert_time(begin, end);

		if (time < StartTime) { return RequestStatus::ErrorCode; }

		_fullTime = time;

		return RequestStatus::Ok;
	}

	void
	response_params
	(
		[[maybe_unused]] uint8_t params,
		[[maybe_unused]] PhysicsIterator begin,
		[[maybe_unused]] PhysicsIterator end,
		std::vector<uint8_t>& response
	) noexcept override
	{ response.push_back(0); }

	void
	error_params
	(
		uint8_t params,
		[[maybe_unused]] PhysicsIterator begin,
		[[maybe_unused]] PhysicsIterator end,
		std::vector<uint8_t>& response
	) noexcept override
	{
		params != TimeCommandParams ?
			response.push_back(1) :
			response.push_back(2);
	}
};

} /* namespace Holders */
} /* namespace HDLC */
} /* namespace IntroSatLib */



#endif /* HDLC_HOLDERS_TIMEHOLDER_H_ */
