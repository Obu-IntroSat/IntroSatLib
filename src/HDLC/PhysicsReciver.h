#ifndef HDLCPHYSICSRECIVER_H_
#define HDLCPHYSICSRECIVER_H_

#include "./Base/Physics.h"
#include "../Base/CRC_CCITT.h"
#include "../Base/ByteConverter.h"
#include "../Events/Event.h"

namespace IntroSatLib {
namespace HDLC {

class PhysicsReciver: public Base::Physics
{
private:
	static inline std::map<uint32_t, PhysicsReciver*> _reciveCallbacks;

	using ByteConverter = IntroSatLib::Base::ByteConverterLittleEndian;

	uint16_t _errors = 0;

public:

	event<> OnZeroSize;
	event<uint8_t, Base::iterator, Base::iterator> OnSuccess;
	event<uint16_t, Base::iterator, Base::iterator> OnReciveError;
	event<uint16_t, Base::iterator, Base::iterator> OnSizeError;


	PhysicsReciver(UART_HandleTypeDef *usart) noexcept : Physics(usart)
	{ _reciveCallbacks[_usartReference] = this; }

	void
	new_request() noexcept
	{
		new_byte_request();
		clear();
		_errors = 0;
		set_last_time();
		set_added_byte(false);
	}

private:

	constexpr uint8_t
	byte_replacer(uint8_t byte)
	{
		switch (byte)
		{
			case ReplaceAddedByte:
				return AddedByte;
			case ReplaceStartOrStopByte:
				return StartOrStopByte;
			default:
				_errors++;
				return byte;
		}
	}

	void
	new_byte_request() noexcept
	{ HAL_UART_Receive_IT(get_UART(), get_buffer_reference(), 1); }

	void
	end_packet() noexcept
	{
		const uint8_t sizeBuf = size();
		Base::iterator begin = cbegin();
		Base::iterator end = cend();

		if (sizeBuf == 0) { OnZeroSize(); }
		else if (sizeBuf < Base::MinBufferSize) { OnSizeError(sizeBuf, begin, end); }

		else
		{
			uint16_t errors = static_cast<uint16_t>(
					std::abs(sizeBuf - ByteConverter::ToUInt8(begin, end))
			);
			errors = errors + _errors;

			if (get_added_byte()) { errors++; }

			uint16_t crcResult = IntroSatLib::Base::CRC_CCITT::CRC16(0xFFFF, begin, end);
			if (crcResult != IntroSatLib::Base::CRC_CCITT::GOOD_FCS) { errors++; }

			if (errors == 0)
			{
				uint8_t addr = ByteConverter::ToUInt8(begin + 1, end);
				OnSuccess(addr, begin + 2, end - 2);
			}
			else { OnReciveError(errors, begin, end); }
		}
	}

	void
	next_byte(uint8_t byte) noexcept
	{
		if (byte == StartOrStopByte) { end_packet(); }
		else
		{
			new_byte_request();
			if (AddedByte == byte) { set_added_byte(true); }
			else
			{
				if (get_added_byte())
				{
					byte = byte_replacer(byte);
					set_added_byte(false);
				}
				push(byte);
			}

		}
	}

	void
	recive_callback() noexcept
	{ next_byte(get_buffer_byte()); }

public:

	virtual
	~PhysicsReciver() noexcept
	{ _reciveCallbacks.erase(_usartReference); }

	static void
	USARTCallback(UART_HandleTypeDef *usart) noexcept
	{
		auto reciverIterator = _reciveCallbacks.find(UartReferenceToValue(usart));
		if (reciverIterator != _reciveCallbacks.end())
		{
			reciverIterator->second->recive_callback();
		}
	}
};

} /* namespace HDLC */
} /* namespace IntroSatLib */
#endif /* HDLCPHYSICSRECIVER_H_ */
