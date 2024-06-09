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
		uint64_t shift = 64;
		for (uint8_t i = 0; i < TimeCommandParams; i++)
		{
			shift -= UNIXBitInByte;
			time |= static_cast<uint64_t>(cpStart[i]) << shift;
		}
		return time;
	}

protected:
	uint8_t IsCurrentParams(
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop
	) const override { return *cpStart == TimeCommandByte; }

	uint8_t RequestParams(
		uint16_t params,
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop
	) override
	{
		if (params != TimeCommandParams) { return 1; }
		uint64_t time = ConvertTime(cpStart, cpStop);
		if (time < StartTime) { return 1; }
		_fullTime = time;
		return 0;
	}

	void ResponceParams(
		uint16_t params,
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop,
		std::vector<uint8_t>& responce
	) const override
	{
		responce.push_back(0);
	}

	void ErrorParams(
		uint16_t params,
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop,
		std::vector<uint8_t>& responce
	) const override
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
