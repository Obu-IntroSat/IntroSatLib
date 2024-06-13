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
	uint32_t GetUNIX() const
	{
		return _fullTime >> UNIXFractionShift;
	}

	uint32_t GetFractionalUNIX() const
	{
		return static_cast<uint32_t>(_fullTime);
	}

	uint64_t GetFullTime() const
	{
		return _fullTime;
	}

private:
	static uint64_t ConvertTime(
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop
	) {
		uint64_t time = 0;
		time |= static_cast<uint64_t>(ByteConverter::ToUInt32(cpStart, cpStop)) << ByteConverter::Int32Shift;
		time |= static_cast<uint64_t>(ByteConverter::ToUInt32(cpStart + ByteConverter::Int32ByteCount, cpStop));
		return time;
	}

protected:
	uint8_t IsCurrentParams(
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop
	) const override { return ByteConverter::ToUInt8(cpStart, cpStop) == TimeCommandByte; }

	RequestStatus RequestParams(
		uint16_t params,
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop
	) override
	{
		if (params != TimeCommandParams) { return RequestStatus::ErrorCode; }

		uint64_t time = ConvertTime(cpStart, cpStop);

		if (time < StartTime) { return RequestStatus::ErrorCode; }

		_fullTime = time;

		return RequestStatus::Ok;
	}

	void ResponceParams(
		uint16_t params,
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop,
		std::vector<uint8_t>& responce
	) override
	{
		responce.push_back(0);
	}

	void ErrorParams(
		uint16_t params,
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop,
		std::vector<uint8_t>& responce
	) override
	{
		params != TimeCommandParams ?
			responce.push_back(1) :
			responce.push_back(2);
	}
};

} /* namespace Holders */
} /* namespace HDLC */
} /* namespace IntroSatLib */



#endif /* HDLC_HOLDERS_TIMEHOLDER_H_ */
