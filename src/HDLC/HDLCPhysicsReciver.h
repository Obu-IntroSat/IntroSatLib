#ifndef HDLCPHYSICSRECIVER_H_
#define HDLCPHYSICSRECIVER_H_

#include "Base/HDLCPhysics.h"
#include "../Base/CRC_CCITT.h"
#include "../Events/Event.h"

#include <cmath>

namespace IntroSatLib {
namespace HDLC {

class HDLCPhysicsReciver: public Base::HDLCPhysics
{

private:
	uint16_t _errors = 0;

	static inline std::map<uint32_t, HDLCPhysicsReciver*> _reciveCallbacks;

public:

	event<> OnZeroSize;
	event<uint8_t, iterator, iterator> OnSuccess;
	event<uint8_t, iterator, iterator> OnReciveError;
	event<uint8_t, iterator, iterator> OnSizeError;


	HDLCPhysicsReciver(UART_HandleTypeDef *usart):
		HDLCPhysics(usart)
	{
		_reciveCallbacks[_usartReference] = this;
	}

	void NewRequest()
	{
		NewRequestByte();
		_buffer.clear();
		_buffer.reserve(MaxBufferLength);
		_errors = 0;
		_prevAddedByte = 0;
	}

private:

	void NewRequestByte()
	{
		HAL_UART_Receive_IT(_usart, &_bufferByte, 1);
	}

	void StartOrEndPacket()
	{
		SetLastTime();
		uint8_t size = _buffer.size();
		iterator start = _buffer.cbegin();
		iterator end = _buffer.cend();
		if (size == 0)
		{
			OnZeroSize();
		}
		else if (size < 5)
		{
			OnSizeError(size, start, end);
		}
		else
		{
			int16_t errorsSize = _buffer.size() - _buffer[0];
			errorsSize = std::abs(errorsSize) + _errors;

			if (_prevAddedByte) { _errors++; }

			uint16_t crcResult = IntroSatLib::Base::CRC_CCITT::CRC16(0xFFFF, _buffer);
			if (crcResult != IntroSatLib::Base::CRC_CCITT::GOOD_FCS)
			{
				_errors++;
			}

			if (_errors == 0)
			{
				uint8_t addr = _buffer[1];
				OnSuccess(addr, start + 2, end - 2);
			}
			else
			{
				OnReciveError(_errors, start, end);
			}
		}
	}

	void NextByte(uint8_t byte)
	{
		if (byte == StartOrStopByte)
		{
			StartOrEndPacket();
			return;
		}
		NewRequestByte();
		if (AddedByte == byte)
		{
			_prevAddedByte = 1;
			return;
		}

		if (_prevAddedByte)
		{
			switch (byte)
			{
			case ReplaceAddedByte:
				byte = AddedByte;
				break;
			case ReplaceStartOrStopByte:
				byte = StartOrStopByte;
				break;
			default:
				_errors++;
			}
			_prevAddedByte = 0;
		}

		_buffer.push_back(byte);
	}
	void ReciveCallback()
	{
		uint8_t byte = _bufferByte;
		NextByte(byte);
	}

public:
	virtual ~HDLCPhysicsReciver()
	{
		_reciveCallbacks.erase(_usartReference);
	}

	static void USARTCallback(UART_HandleTypeDef *usart)
	{
		auto reciverIterator = _reciveCallbacks.find(UartReferenceToValue(usart));
		if (reciverIterator != _reciveCallbacks.end())
		{
			reciverIterator->second->ReciveCallback();
		}
	}
};

}
}


#endif /* HDLCPHYSICSRECIVER_H_ */
