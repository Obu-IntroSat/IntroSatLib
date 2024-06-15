#ifndef BASE_BYTECONVERTER_H_
#define BASE_BYTECONVERTER_H_

#include <stdint.h>
#include <limits.h>
#include <iterator>

namespace IntroSatLib {

namespace Base {
class ByteConverterLittleEndian
{
public:
	static const uint8_t BitInByte = 8;
	static const uint8_t Int8ByteCount = 1;
	static const uint8_t Int16ByteCount = 2;
	static const uint8_t Int32ByteCount = 4;
	static const uint8_t Int64ByteCount = 8;

	static constexpr uint8_t Int8Shift = 1 * BitInByte;
	static constexpr uint8_t Int16Shift = 2 * BitInByte;
	static constexpr uint8_t Int32Shift = 4 * BitInByte;
	static constexpr uint8_t Int64Shift = 8 * BitInByte;

	template<class iterator>
	constexpr static uint8_t
	ToUInt8
	(
		const iterator &begin,
		const iterator &end
	) noexcept
	{
		if (end - begin < Int8ByteCount) { return 0; }

		return static_cast<uint8_t>(*begin);
	}

	template<class iterator>
	constexpr static uint16_t
	ToUInt16
	(
		const iterator &begin,
		const iterator &end
	) noexcept
	{
		if (end - begin < Int16ByteCount) { return 0; }

		uint16_t result = 0;
		for (uint8_t i = 0; i < Int16ByteCount; i++)
		{
			result |= static_cast<uint16_t>(begin[i]) << (i * BitInByte);
		}
		return result;
	}

	template<class iterator>
	constexpr static uint32_t
	ToUInt32
	(
		const iterator &begin,
		const iterator &end
	) noexcept
	{
		if (end - begin < Int32ByteCount) { return 0; }

		uint32_t result = 0;
		for (uint8_t i = 0; i < Int32ByteCount; i++)
		{
			result |= static_cast<uint32_t>(begin[i]) << (i * BitInByte);
		}
		return result;
	}

	template<class iterator>
	constexpr static uint64_t ToUInt64
	(
		const iterator &begin,
		const iterator &end
	) noexcept
	{
		if (end - begin < Int64ByteCount) { return 0; }

		uint64_t result = 0;
		for (uint8_t i = 0; i < Int64ByteCount; i++)
		{
			result |= static_cast<uint32_t>(begin[i]) << (i * BitInByte);
		}
		return result;
	}

	template<class iterator>
	constexpr static int8_t
	ToInt8
	(
		const iterator &begin,
		const iterator &end
	) noexcept
	{
		return static_cast<int8_t>(ToUInt8(begin, end));
	}

	template<class iterator>
	constexpr static int16_t
	ToInt16
	(
		const iterator &begin,
		const iterator &end
	) noexcept
	{
		return static_cast<int16_t>(ToUInt16(begin, end));
	}

	template<class iterator>
	constexpr static int32_t
	ToInt32
	(
		const iterator &begin,
		const iterator &end
	) noexcept
	{
		return static_cast<int32_t>(ToUInt32(begin, end));
	}

	template<class iterator>
	constexpr static int64_t
	ToInt64
	(
		const iterator &begin,
		const iterator &end
	) noexcept
	{
		return static_cast<int64_t>(ToUInt64(begin, end));
	}
};

} /* namespace Base */
} /* namespace IntroSatLib */

#endif /* BASE_BYTECONVERTER_H_ */
