#ifndef HDLCPHYSICSTRANSMITTER_H_
#define HDLCPHYSICSTRANSMITTER_H_

#include "Base/HDLCPhysics.h"

#include <cmath>

namespace IntroSatLib {
namespace HDLC {

class HDLCPhysicsTransmitter: public Base::HDLCPhysics
{
private:
	int16_t _count = -1;

	static inline std::map<uint32_t, HDLCPhysicsTransmitter*> _transmitCallbacks;

public:
	HDLCPhysicsTransmitter(UART_HandleTypeDef *usart): HDLCPhysics(usart)
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
		if (GetDeltaTime() > 50)
		{
			NewResponseByte(StartOrStopByte);
		}
		_count = 0;
		_prevAddedByte = 0;
	}

private:
	template<class iterator>
	void SetBuffer(const iterator &cpStart, const iterator &cpStop)
	{
		_buffer.clear();
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
	}

	void NewResponseByte(uint8_t byte)
	{
		_bufferByte = byte;
		HAL_UART_Transmit_IT(_usart, &_bufferByte, 1);
	}

	uint8_t StopBitFind()
	{
		if (_count == static_cast<int16_t>(_buffer.size()))
		{
			NewResponseByte(StartOrStopByte);
			SetLastTime();
			return 1;
		}
		return 0;

	}
	void ProcessByte()
	{
		if (StopBitFind()) { return; }

		uint8_t byte = _buffer[_count];

		if (_prevAddedByte)
		{
			switch (byte)
			{
			case AddedByte:
				byte = ReplaceAddedByte;
				break;
			case StartOrStopByte:
				byte = ReplaceStartOrStopByte;
				break;
			default:
				break;
			}
			_count++;
			_prevAddedByte = 0;
		}
		else
		{
			switch (byte)
			{
			case AddedByte:
			case StartOrStopByte:
				byte = ReplaceStartOrStopByte;
				_prevAddedByte = 1;
				break;
			default:
				_count++;
				break;
			}
		}
		NewResponseByte(byte);

	}
	void TransmitCallback()
	{
		ProcessByte();
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
			transmitterIterator->second->TransmitCallback();
		}
	}
};

} /* namespace HDLC */
} /* namespace IntroSatLib */


#endif /* HDLCPHYSICSTRANSMITTER_H_ */
