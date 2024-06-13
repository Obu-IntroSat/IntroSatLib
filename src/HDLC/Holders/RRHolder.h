#ifndef HDLC_HOLDERS_RRHOLDER_H_
#define HDLC_HOLDERS_RRHOLDER_H_

#include "../Base/HDLCHolder.h"

namespace IntroSatLib {
namespace HDLC {
namespace Holders {

class RRHolder: public IntroSatLib::HDLC::Base::HDLCHolder
{
private:
	static const uint8_t RRCommandByte = 0x01;
	static const uint8_t RRCommandLength = 8;
	static const uint8_t RRCommandIndexShift = 33;
	static const uint8_t RRCommandError = 0x97;

	static const uint8_t RRCommandErrorFormat = 1;
	static const uint8_t RRCommandErrorIndex = 4;

	uint32_t _vi = 0;

public:
	uint8_t IsCurrent(
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop
	) const override
	{
		uint8_t firstByte = ByteConverter::ToUInt8(cpStart, cpStop);
		uint16_t countParams = std::distance(cpStart, cpStop);
		return countParams >= RRCommandLength && firstByte == RRCommandByte;
	}

	RequestStatus Request(
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop
	) override
	{
		uint16_t countParams = std::distance(cpStart, cpStop);
		if (countParams != RRCommandLength) { return RequestStatus::Error; }

		uint64_t command = ByteConverter::ToUInt64(cpStart, cpStop);

		uint32_t index = static_cast<uint32_t>(command >> RRCommandIndexShift);

		if (index != _vi) { return RequestStatus::ErrorCode; }

		return RequestStatus::Ok;
	}

	void Responce(
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop,
		std::vector<uint8_t>& responce
	) override
	{
		uint64_t resultCode = 0x100000000;
		resultCode |= (_vi << 1);
		for (uint8_t i = 0; i < ByteConverter::Int64ByteCount; i++)
		{
			responce.push_back(static_cast<uint8_t>(resultCode >> (i * ByteConverter::BitInByte)));
		}
		_vi++;
	}

	void Error(
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop,
		std::vector<uint8_t>& responce
	) override
	{
		responce.push_back(RRCommandError | 0);
		for (uint8_t i = 0; i < RRCommandLength; i++)
		{
			responce.push_back(cpStart[i]);
		}
		for (uint8_t i = 0; i < ByteConverter::Int32ByteCount; i++)
		{
			responce.push_back(static_cast<uint8_t>(_vi >> (i * ByteConverter::BitInByte)));
		}
		for (uint8_t i = 0; i < ByteConverter::Int32ByteCount; i++)
		{
			responce.push_back(0);
		}
		uint8_t errorCode = 0;

		if (std::distance(cpStart, cpStop) != RRCommandLength) { errorCode |= RRCommandErrorFormat; }

		uint32_t index = static_cast<uint32_t>(ByteConverter::ToUInt64(cpStart, cpStop) >> RRCommandIndexShift);

		if (index != _vi) { errorCode |= RRCommandErrorIndex; }

		responce.push_back(errorCode);
	}
};

} /* namespace Holders */
} /* namespace HDLC */
} /* namespace IntroSatLib */


#endif /* HDLC_HOLDERS_RRHOLDER_H_ */
