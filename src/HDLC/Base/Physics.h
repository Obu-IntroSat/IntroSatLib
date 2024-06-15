#ifndef HDLC_BASE_Physics_H_
#define HDLC_BASE_Physics_H_

#include <stdint.h>
#include <map>
#include "stm32f1xx_hal_uart.h"
#include "../../Base/Memory/static_ptr.h"
#include "../../Base/Collections/BufferArray.h"

namespace IntroSatLib {
namespace HDLC {
namespace Base {

class Physics
{
private:
	static const uint8_t MaxBufferLength = 255;

private:
	uint32_t _lastTime;
	IntroSatLib::Base::Memory::BufferArray<uint8_t, MaxBufferLength> _buffer;

protected:

	template<typename T>
	using static_ptr = IntroSatLib::Base::Memory::static_ptr<T>;

	static const uint8_t StartOrStopByte = 0x7E;
	static const uint8_t AddedByte = 0x7D;
	static const uint8_t ReplaceAddedByte = 0x5D;
	static const uint8_t ReplaceStartOrStopByte = 0x5E;

	static_ptr<UART_HandleTypeDef> _usart;
	const uint32_t _usartReference;

	uint8_t _prevAddedByte = 0;
	uint8_t _bufferByte = 0;

public:
	typedef decltype(_buffer)::const_iterator iterator;
	typedef decltype(_buffer)::value_type value_type;
	typedef decltype(_buffer)::size_type size_type;

protected:
	static constexpr uint32_t UartReferenceToValue(const USART_TypeDef *usart)
	{
		return reinterpret_cast<uint32_t>(usart);
	}
	static constexpr uint32_t UartReferenceToValue(const UART_HandleTypeDef *usart)
	{
		return UartReferenceToValue(usart->Instance);
	}

	Physics(UART_HandleTypeDef *usart):
		_usart(usart),
		_usartReference(UartReferenceToValue(usart))
	{ set_last_time(); }

	void
	set_last_time() noexcept
	{ _lastTime = HAL_GetTick(); }

	constexpr uint32_t
	get_last_time() const noexcept
	{ return _lastTime; }

	uint32_t
	get_delta_time() const noexcept
	{ return HAL_GetTick() - get_last_time(); }

	constexpr void
	push(value_type x) noexcept
	{ _buffer.push_back(x); }

	constexpr uint8_t
	size() noexcept
	{ return static_cast<uint8_t>(_buffer.size()); }

	constexpr iterator
	cbegin() const noexcept
	{ return _buffer.cbegin(); }

	constexpr iterator
	cend() const noexcept
	{ return _buffer.cend(); }

	constexpr void
	clear() noexcept
	{ _buffer.clear(); }

	virtual ~Physics() = default;
};

} /* namespace Base */
} /* namespace HDLC */
} /* namespace IntroSatLib */
#endif /* HDLC_BASE_Physics_H_ */
