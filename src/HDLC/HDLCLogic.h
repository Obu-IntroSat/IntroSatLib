#ifndef HDLCLOGIC_H_
#define HDLCLOGIC_H_

#include "../Base/GetTypeId.h"
#include "../Base/InstanceOf.h"
#include "HDLCPhysicsReciver.h"
#include "HDLCPhysicsTransmitter.h"
#include "Base/HDLCHolder.h"


namespace IntroSatLib {
namespace HDLC {

class HDLCLogic
{
private:
	template<typename Base, typename Derived>
	using InstanceOf = IntroSatLib::Base::InstanceOf<Base, Derived>;

	using RequestStatus = Base::HDLCHolder::RequestStatus;
	using HDLCPhysicsIterator = Base::HDLCPhysics::iterator;

public:
	template<class TService, typename InstanceOf<Base::HDLCHolder, TService>::type* = nullptr>
	using Reference = std::shared_ptr<TService>;

	using ServiceReference = Reference<Base::HDLCHolder>;

private:
	HDLCPhysicsTransmitter _transmitter;
	HDLCPhysicsReciver _reciver;
	uint8_t _address;

	std::map<uint32_t, ServiceReference> _tree;
	std::vector<ServiceReference> _list;

public:
	HDLCLogic(
		UART_HandleTypeDef *usart,
		GPIO_TypeDef *gpio,
		uint16_t gpioPin,
		uint8_t address
	): _transmitter(usart, gpio, gpioPin), _reciver(usart), _address(address)
	{
		_reciver.OnSuccess += [this](uint8_t address, HDLCPhysicsIterator cpStart, HDLCPhysicsIterator cpStop)
		{
			if (OnRecive(address, cpStart, cpStop))
			{
				_reciver.NewRequest();
			}
		};
		_reciver.OnZeroSize += [this]()
		{
			_reciver.NewRequest();
		};
		_reciver.OnReciveError += [this](uint8_t errors, HDLCPhysicsIterator cpStart, HDLCPhysicsIterator cpStop)
		{
			_reciver.NewRequest();
		};
		_reciver.OnSizeError += [this](uint8_t size, HDLCPhysicsIterator cpStart, HDLCPhysicsIterator cpStop)
		{
			_reciver.NewRequest();
		};

	}

	void Start()
	{
		_reciver.NewRequest();
	}

private:
	uint8_t OnRecive(uint8_t address, HDLCPhysicsIterator cpStart, HDLCPhysicsIterator cpStop)
	{
		uint8_t broadcast = address == 0xFF ? 1 : 0;

		if (address != _address && !broadcast) { return 1; }

		for (ServiceReference &holder : _list)
		{
			if (holder->IsCurrent(cpStart, cpStop) == 0) { continue; }

			RequestStatus resultCode = holder->Request(cpStart, cpStop);
			RequestStatus hasError = resultCode & RequestStatus::ErrorCode;
			RequestStatus canResponce = resultCode & RequestStatus::CanResponce;
			RequestStatus cantNext = resultCode & RequestStatus::CantNextCode;

			if (broadcast && canResponce != RequestStatus::CanResponce) { continue; }

			std::vector<uint8_t> responce;
			responce.push_back(_address);

			(hasError == RequestStatus::ErrorCode) ?
				holder->Error(cpStart, cpStop, responce) :
				holder->Responce(cpStart, cpStop, responce);

			_transmitter.NewResponse(responce);
			return cantNext == RequestStatus::CantNextCode;
		}
		return 1;
	}
public:
	template<
		class TService,
		typename... Args,
		typename InstanceOf<Base::HDLCHolder, TService>::type* = nullptr
	>
	void Register(Args&&... args)
	{
		Reference<TService> service = std::make_shared<TService>(std::forward<Args>(args)...);
		ServiceReference baseService = std::static_pointer_cast<Base::HDLCHolder>(service);
		_tree[IntroSatLib::Base::GetTypeId<TService>()] = baseService;
		_list.push_back(baseService);
	}

	template<
		class TService,
		typename InstanceOf<Base::HDLCHolder, TService>::type* = nullptr
	>
	Reference<TService> Get()
	{
		auto search = _tree.find(IntroSatLib::Base::GetTypeId<TService>());
		if (search != _tree.end())
		{
			return std::static_pointer_cast<TService>(search->second);
		}
	}

	static void USARTReciveCallback(UART_HandleTypeDef *usart)
	{
		HDLCPhysicsReciver::USARTCallback(usart);
	}
	static void USARTTransmitCallback(UART_HandleTypeDef *usart)
	{
		HDLCPhysicsTransmitter::USARTCallback(usart);
	}

	static void TimeoutCallback()
	{
		HDLCPhysicsTransmitter::TimeoutCallback();
	}
};

} /* namespace HDLC */
} /* namespace IntroSatLib */

#endif /* HDLCLOGIC_H_ */
