#ifndef HDLCPHYSICSTRANSMITTER_H_
#define HDLCPHYSICSTRANSMITTER_H_

#include "stm32f1xx_hal_gpio.h"
#include "Base/Physics.h"

namespace IntroSatLib {
namespace HDLC {

class PhysicsTransmitter: public Base::Physics
{
private:
	static inline std::map<uint32_t, PhysicsTransmitter*> _transmitCallbacks;

	Base::Physics::static_ptr<GPIO_TypeDef> _gpio;
	const uint16_t _gpioPin;
	int16_t _count = -2;

public:
	PhysicsTransmitter
	(
		UART_HandleTypeDef *usart,
		GPIO_TypeDef *gpio,
		uint16_t gpioPin
	) noexcept :
		Physics(usart),
		_gpio(gpio),
		_gpioPin(gpioPin)
	{ _transmitCallbacks[_usartReference] = this; }

	template<typename Iterator>
	constexpr void
	new_response
	(
		const Iterator &cpStart,
		const Iterator &cpStop
	) noexcept
	{
		set_last_time();
		_prevAddedByte = 0;
		set_buffer(cpStart, cpStop);
	}

private:

	constexpr uint8_t
	get_current_byte() const noexcept
	{ return cbegin()[_count]; }

	constexpr void
	clear_buffer() noexcept
	{
		_count = -2;
		clear();
	}

	template<class iterator>
	constexpr void
	set_buffer
	(
		const iterator &begin,
		const iterator &end
	) noexcept
	{
		clear_buffer();

		uint8_t size = static_cast<uint8_t>(end - begin) + 3;
		push(size);

		for (iterator it = begin; it != end; it++) { push(*it); }

		uint16_t crc = IntroSatLib::Base::CRC_CCITT::CRC16(0xFFFF, cbegin(), cend()) ^ 0xFFFF;
		push(uint8_t(crc));
		push(uint8_t(crc >> 8));

		_count = -1;
	}

	void
	transmite_byte(uint8_t byte) noexcept
	{
		_bufferByte = byte;
		HAL_UART_Transmit_IT(_usart.get(), &_bufferByte, 1);
	}

	void
	start_transmite() noexcept
	{
		if (_count == -2) { return; }
		if (HAL_GPIO_ReadPin(_gpio.get(), _gpioPin) == GPIO_PIN_RESET) { return; }

		if (get_last_time() < 50) { clear_buffer(); }
		else { transmite_next_byte(); }
	}

	uint8_t
	start_or_stop_transmite() noexcept
	{
		if (_count < -1 || _count > static_cast<int16_t>(size()))
		{
			clear_buffer();
			return 1;
		}
		if (_count == -1 || _count == static_cast<int16_t>(size()))
		{
			transmite_byte(StartOrStopByte);
			set_last_time();
			return 1;
		}
		return 0;

	}

	constexpr uint8_t
	byte_replacer(uint8_t byte) noexcept
	{
		if (_prevAddedByte)
		{
			_prevAddedByte = 0;
			switch (byte)
			{
			case AddedByte:
				return ReplaceAddedByte;
			case StartOrStopByte:
				return ReplaceStartOrStopByte;
			default:
				return byte;
			}
		}
		else
		{
			switch (byte)
			{
			case AddedByte:
			case StartOrStopByte:
				_count--;
				_prevAddedByte = 1;
				return ReplaceStartOrStopByte;
			default:
				return byte;
			}
		}
	}

	void
	transmite_next_byte() noexcept
	{
		if (start_or_stop_transmite()) { return; }

		uint8_t byte = byte_replacer(get_current_byte());
		_count++;

		transmite_byte(byte);

	}

	void
	TransmitCallbackHandler() noexcept
	{ transmite_next_byte(); }

	void
	TimeoutCallbackHandler() noexcept
	{ start_transmite(); }

public:

	virtual
	~PhysicsTransmitter() noexcept
	{ _transmitCallbacks.erase(_usartReference); }

	static void
	USARTCallback(UART_HandleTypeDef *usart) noexcept
	{
		auto transmitterIterator = _transmitCallbacks.find(UartReferenceToValue(usart));
		if (transmitterIterator != _transmitCallbacks.end())
		{
			transmitterIterator->second->TransmitCallbackHandler();
		}
	}

	static void
	TimeoutCallback() noexcept
	{
		for (auto &transmitCallback : _transmitCallbacks)
		{
			transmitCallback.second->TimeoutCallbackHandler();
		}
	}
};

} /* namespace HDLC */
} /* namespace IntroSatLib */


#endif /* HDLCPHYSICSTRANSMITTER_H_ */
