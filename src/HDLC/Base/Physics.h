#ifndef HDLC_BASE_Physics_H_
#define HDLC_BASE_Physics_H_

#include <stdint.h>
#include <map>
#include "../../Includes/UART.h"
#include "../../Base/Memory/static_ptr.h"
#include "./Typing.h"

namespace IntroSatLib {
namespace HDLC {
namespace Base {

class Physics
{
private:
	uint32_t _lastTime;
	BufferType _buffer;

	const static_ptr<UART_HandleTypeDef> _usart;

	uint8_t _bufferByte = 0;

	bool _prevAddedByte = 0;

protected:
	static const uint8_t StartOrStopByte = 0x7E;
	static const uint8_t AddedByte = 0x7D;
	static const uint8_t ReplaceAddedByte = 0x5D;
	static const uint8_t ReplaceStartOrStopByte = 0x5E;

	const uint32_t _usartReference;

protected:

	static constexpr uint32_t
	UartReferenceToValue(const UART_HandleTypeDef *usart) noexcept
	{ return reinterpret_cast<uint32_t>(usart); }

	Physics(UART_HandleTypeDef *usart) noexcept :
		_usart(usart),
		_usartReference(UartReferenceToValue(usart))
	{ set_last_time(); }

	constexpr UART_HandleTypeDef*
	get_UART() const noexcept
	{ return _usart.get(); }

	constexpr uint8_t*
	get_buffer_reference() noexcept
	{ return &_bufferByte; }

	constexpr uint8_t
	get_buffer_byte() const noexcept
	{ return _bufferByte; }

	constexpr uint8_t*
	set_buffer_byte(uint8_t byte) noexcept
	{ _bufferByte = byte; return &_bufferByte; }

	constexpr bool
	get_added_byte() const noexcept
	{ return _prevAddedByte; }

	constexpr void
	set_added_byte(bool x) noexcept
	{ _prevAddedByte = x; }

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
