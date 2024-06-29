#ifndef HDLC_HOLDERS_RRHOLDER_H_
#define HDLC_HOLDERS_RRHOLDER_H_

#include <queue>
#include "../Base/Holder.h"
#include "../Base/Typing.h"

namespace IntroSatLib {
namespace HDLC {
namespace Holders {

class RRHolder: public IntroSatLib::HDLC::Base::Holder
{
private:
	static const uint8_t RRCommandByte = 0x01;
	static const uint8_t RRCommandLength = 8;
	static const uint8_t RRCommandIndexShift = 33;
	static const uint8_t RRCommandError = 0x97;

	static const uint8_t RRCommandErrorFormat = 1;
	static const uint8_t RRCommandErrorIndex = 4;
	static const uint32_t MaxValueIndex = 0x7FFFFFFF;
	static const uint32_t MaxBuffer = 5;

	uint32_t _vi = 0;

	uint8_t _first = 1;

	std::queue<Base::BufferType> _queue;

public:
	template<class iterator>
	constexpr void
	add
	(
		const iterator cpStart,
		const iterator cpStop
	) noexcept
	{
		Base::BufferType toSet;
		for (iterator it = cpStart; cpStart != cpStop; it++)
		{
			toSet.push_back(*it);
		}
		_queue.push(toSet);
	}

	template<class iterator>
	constexpr void
	add
	(
		const iterator cpStart,
		const uint8_t size
	) noexcept
	{ add(cpStart, cpStart + size); }

	bool
	can_add() const noexcept
	{ return _queue.size() < MaxBuffer; }

protected:
	uint8_t
	is_current
	(
		HolderIterator begin,
		HolderIterator end
	) const noexcept override
	{
		uint8_t firstByte = ByteConverter::ToUInt8(begin, end);
		uint8_t countParams = distance(begin, end);
		return countParams >= RRCommandLength && firstByte == RRCommandByte;
	}

	RequestStatus
	request
	(
		HolderIterator begin,
		HolderIterator end
	) noexcept override
	{
		uint8_t countParams = distance(begin, end);
		if (countParams != RRCommandLength) { return RequestStatus::ErrorCode; }

		uint64_t command = ByteConverter::ToUInt64(begin, end);

		uint32_t index = static_cast<uint32_t>(command >> RRCommandIndexShift);

		if (_first == 1)
		{
			_vi = index;
			_first = 0;
			return RequestStatus::Ok;
		}
		if (index != _vi && index != (_vi + 1)) { return RequestStatus::ErrorCode; }

		return RequestStatus::Ok;
	}

	void
	response
	(
		HolderIterator begin,
		HolderIterator end,
		HolderBuffer& response
	) noexcept override
	{
		uint64_t resultCode = 0x100000000;
		uint64_t command = ByteConverter::ToUInt64(begin, end);

		uint32_t index = static_cast<uint32_t>(command >> RRCommandIndexShift);

		if (index != _vi) { _vi = (_vi + 1) & MaxValueIndex; }

		resultCode |= (_vi << 1);

		for (uint8_t i = 0; i < ByteConverter::Int64ByteCount; i++)
		{
			response.push_back(static_cast<uint8_t>(resultCode >> (i * ByteConverter::BitInByte)));
		}

		if (_queue.empty()) { return; }

		Base::BufferType& buffer = _queue.front();

		for (const uint8_t& value : buffer)
		{
			response.push_back(value);
		}
		_queue.pop();
	}

	void
	error
	(
		HolderIterator begin,
		HolderIterator end,
		HolderBuffer& response
	) noexcept override
	{
		response.push_back(RRCommandError | 0);
		for (uint8_t i = 0; i < RRCommandLength; i++)
		{
			response.push_back(begin[i]);
		}
		for (uint8_t i = 0; i < ByteConverter::Int32ByteCount; i++)
		{
			response.push_back(static_cast<uint8_t>(_vi >> (i * ByteConverter::BitInByte)));
		}
		for (uint8_t i = 0; i < ByteConverter::Int32ByteCount; i++)
		{
			response.push_back(0);
		}
		uint8_t errorCode = 0;

		if (distance(begin, end) != RRCommandLength) { errorCode |= RRCommandErrorFormat; }

		uint32_t index = static_cast<uint32_t>(ByteConverter::ToUInt64(begin, end) >> RRCommandIndexShift);

		if (index != _vi) { errorCode |= RRCommandErrorIndex; }

		response.push_back(errorCode);
	}
};

} /* namespace Holders */
} /* namespace HDLC */
} /* namespace IntroSatLib */


#endif /* HDLC_HOLDERS_RRHOLDER_H_ */
