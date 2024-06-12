#ifndef HDLCPHYSICSTRANSMITTER_H_
#define HDLCPHYSICSTRANSMITTER_H_

#include "Base/HDLCPhysics.h"

#include <cmath>

namespace IntroSatLib {
namespace HDLC {

class HDLCPhysicsTransmitter: public Base::HDLCPhysics
{
private:
	int16_t _count = -2;
	static inline std::map<uint32_t, HDLCPhysicsTransmitter*> _transmitCallbacks;
	GPIO_TypeDef *_gpio;
	uint16_t _gpioPin;

public:
	HDLCPhysicsTransmitter(UART_HandleTypeDef *usart, GPIO_TypeDef *gpio, uint16_t gpioPin):
		HDLCPhysics(usart),
		_gpio(gpio),
		_gpioPin(gpioPin)
	{
		_transmitCallbacks[_usartReference] = this;
	}

	void NewResponse(const std::vector<uint8_t> &buffer)
	{
		NewResponse(buffer.cbegin(), buffer.cend());
	}

	template<class iterator>
	void NewResponse(const iterator &cpStart, const iterator &cpStop)
	{
		SetBuffer(cpStart, cpStop);
		_prevAddedByte = 0;
	}

private:
	void ClearBuffer()
	{
		_count = -2;
		_buffer.clear();
	}

	template<class iterator>
	void SetBuffer(const iterator &cpStart, const iterator &cpStop)
	{
		ClearBuffer();
		uint16_t size = cpStop - cpStart + 3;
		_buffer.reserve(size);
		_buffer.push_back(size);
		for (iterator it = cpStart; it != cpStop; it++)
		{
			_buffer.push_back(*it);
		}
		uint16_t crc = IntroSatLib::Base::CRC_CCITT::CRC16(0xFFFF, _buffer) ^ 0xFFFF;
		_buffer.push_back(uint8_t(crc));
		_buffer.push_back(uint8_t(crc >> 8));
		_count = -1;
	}

	void NewResponseByte(uint8_t byte)
	{
		_bufferByte = byte;
		HAL_UART_Transmit_IT(_usart, &_bufferByte, 1);
	}

	void StartTransmite()
	{
		if (_count == -2) { return; }
		if (HAL_GPIO_ReadPin(_gpio, _gpioPin) == GPIO_PIN_RESET) { return; }
		if (GetLastTime() > 50)
		{
			ClearBuffer();
			return;
		}

		ProcessByte();
	}

	uint8_t StartOrStopBitTransmite()
	{
		if (_count < -1 || _count > static_cast<int16_t>(_buffer.size()))
		{
			ClearBuffer();
			return 1;
		}
		if (_count == -1 || _count == static_cast<int16_t>(_buffer.size()))
		{
			NewResponseByte(StartOrStopByte);
			SetLastTime();
			return 1;
		}
		return 0;

	}

	uint8_t ByteReplacer(uint8_t byte)
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

	void ProcessByte()
	{
		if (StartOrStopBitTransmite()) { return; }

		uint8_t byte = ByteReplacer(_buffer[_count]);
		_count++;

		NewResponseByte(byte);

	}

	void TransmitCallbackHandler()
	{
		ProcessByte();
	}

	void TimeoutCallbackHandler()
	{
		StartTransmite();
	}

public:
	virtual ~HDLCPhysicsTransmitter()
	{
		_transmitCallbacks.erase(_usartReference);
	}
	static void USARTCallback(UART_HandleTypeDef *usart)
	{
		auto transmitterIterator = _transmitCallbacks.find(UartReferenceToValue(usart));
		if (transmitterIterator != _transmitCallbacks.end())
		{
			transmitterIterator->second->TransmitCallbackHandler();
		}
	}

	static void TimeoutCallback()
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
