#ifndef HDLCPhysics_H_
#define HDLCPhysics_H_

#include <vector>
#include <map>

namespace IntroSatLib {
namespace HDLC {
namespace Base {

class HDLCPhysics
{
private:
	static inline std::map<uint32_t, uint32_t> _lastTime;

protected:
	static const uint8_t StartOrStopByte = 0x7E;
	static const uint8_t AddedByte = 0x7D;
	static const uint8_t ReplaceAddedByte = 0x5D;
	static const uint8_t ReplaceStartOrStopByte = 0x5E;

	static const uint8_t MaxBufferLength = 255;

	UART_HandleTypeDef *_usart;
	const uint32_t _usartReference;

	std::vector<uint8_t> _buffer;
	uint8_t _prevAddedByte = 0;
	uint8_t _bufferByte = 0;

public:
	typedef decltype(_buffer)::const_iterator iterator;

protected:
	static constexpr uint32_t UartReferenceToValue(const USART_TypeDef *usart)
	{
		return reinterpret_cast<uint32_t>(usart);
	}
	static constexpr uint32_t UartReferenceToValue(const UART_HandleTypeDef *usart)
	{
		return UartReferenceToValue(usart->Instance);
	}


	HDLCPhysics(UART_HandleTypeDef *usart): _usart(usart), _usartReference(UartReferenceToValue(usart))
	{
		_buffer.reserve(MaxBufferLength);
		_lastTime[_usartReference] = 0;
	}

	void SetLastTime() const
	{
		_lastTime[_usartReference] = HAL_GetTick();
	}

	uint32_t GetLastTime() const
	{
		return _lastTime[_usartReference];
	}

	uint32_t GetDeltaTime() const
	{
		return HAL_GetTick() - GetLastTime();
	}

	virtual ~HDLCPhysics()
	{
		_buffer.clear();
		_lastTime.erase(_usartReference);
	}
};

}
}
}
#endif /* HDLCPhysics_H_ */
