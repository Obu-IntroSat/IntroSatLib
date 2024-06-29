#ifndef HDLC_LOGIC_H_
#define HDLC_LOGIC_H_

#include "../Base/GetTypeId.h"
#include "../Base/InstanceOf.h"
#include "./PhysicsReciver.h"
#include "./PhysicsTransmitter.h"
#include "./Base/Holder.h"


namespace IntroSatLib {
namespace HDLC {

class Logic
{
private:
	template<typename Base, typename Derived>
	using InstanceOf = IntroSatLib::Base::InstanceOf<Base, Derived>;

	using RequestStatus = Base::Holder::RequestStatus;

public:
	template<class TService, typename InstanceOf<Base::Holder, TService>::type* = nullptr>
	using Reference = std::shared_ptr<TService>;

	using ServiceReference = Reference<Base::Holder>;

private:
	PhysicsTransmitter _transmitter;
	PhysicsReciver _reciver;
	uint8_t _address;

	std::map<uint32_t, ServiceReference> _tree;
	std::vector<ServiceReference> _list;

public:
	Logic
	(
		UART_HandleTypeDef *usart,
		GPIO_TypeDef *gpio,
		uint16_t gpioPin,
		uint8_t address
	) noexcept : _transmitter(usart, gpio, gpioPin), _reciver(usart), _address(address)
	{
		_reciver.OnSuccess += [this](
			uint8_t address,
			Base::iterator begin,
			Base::iterator end
		) { if (OnRecive(address, begin, end)) _reciver.new_request(); };

		_reciver.OnZeroSize += [this]() { _reciver.new_request(); };

		_reciver.OnReciveError += [this](
			[[maybe_unused]] uint8_t errors,
			[[maybe_unused]] Base::iterator begin,
			[[maybe_unused]] Base::iterator end
		) { _reciver.new_request(); };

		_reciver.OnSizeError += [this](
			[[maybe_unused]] uint8_t size,
			[[maybe_unused]] Base::iterator begin,
			[[maybe_unused]] Base::iterator end
		) { _reciver.new_request(); };

	}

	void
	start() noexcept
	{ _reciver.new_request(); }

private:
	uint8_t
	OnRecive
	(
		uint8_t address,
		Base::iterator begin,
		Base::iterator end
	) noexcept
	{
		uint8_t broadcast = address == 0xFF ? 1 : 0;

		if (address != _address && !broadcast) { return 1; }

		for (ServiceReference &holder : _list)
		{
			if (holder->is_current(begin, end) == 0) { continue; }

			RequestStatus resultCode = holder->request(begin, end);

			RequestStatus hasError = resultCode & RequestStatus::ErrorCode;
			RequestStatus canResponce = resultCode & RequestStatus::CanResponce;
			RequestStatus cantNext = resultCode & RequestStatus::CantNextCode;

			if (broadcast && canResponce != RequestStatus::CanResponce) { continue; }

			Base::BufferType response;
			response.push_back(_address);

			(hasError == RequestStatus::ErrorCode) ?
				holder->error(begin, end, response) :
				holder->response(begin, end, response);

			_transmitter.new_response(response.cbegin(), response.cend());
			return cantNext != RequestStatus::CantNextCode;
		}
		return 1;
	}
public:
	template<
		class TService,
		typename... Args,
		typename InstanceOf<Base::Holder, TService>::type* = nullptr
	>
	Reference<TService>
	Register(Args&&... args) noexcept
	{
		Reference<TService> service = std::make_shared<TService>(std::forward<Args>(args)...);
		ServiceReference baseService = std::static_pointer_cast<Base::Holder>(service);
		_tree[IntroSatLib::Base::GetTypeId<TService>()] = baseService;
		_list.push_back(baseService);
		return service;
	}

	template<
		class TService,
		typename InstanceOf<Base::Holder, TService>::type* = nullptr
	>
	Reference<TService>
	Get() const noexcept
	{
		auto search = _tree.find(IntroSatLib::Base::GetTypeId<TService>());
		if (search != _tree.end())
		{
			return std::static_pointer_cast<TService>(search->second);
		}
	}

	static void
	USARTReciveCallback(UART_HandleTypeDef *usart) noexcept
	{ PhysicsReciver::USARTCallback(usart); }

	static void
	USARTTransmitCallback(UART_HandleTypeDef *usart) noexcept
	{ PhysicsTransmitter::USARTCallback(usart); }

	static void
	TimeoutCallback() noexcept
	{ PhysicsTransmitter::TimeoutCallback(); }
};

} /* namespace HDLC */
} /* namespace IntroSatLib */

#endif /* HDLC_LOGIC_H_ */
