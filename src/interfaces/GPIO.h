#ifndef INTERFACES_GPIO_H_
#define INTERFACES_GPIO_H_

#include "stm32g4xx_hal.h"

#ifdef HAL_GPIO_MODULE_ENABLED

#include "stm32g4xx_hal_gpio.h"


#include <cinttypes>

#include "../exception/base.h"
#include "../../third_party/etl/include/etl/enum_type.h"

namespace is {

namespace exception {

struct GPIO_lock_exception: public base {
	GPIO_lock_exception(string_type file, numeric_type line): base("Error of locking pin", file, line) { }
};

} /* namespace exception */

namespace interfaces {

struct GPIOPinState
{
	enum enum_type
	{
		RESET = 0,
		SET = 1
	};

	ETL_DECLARE_ENUM_TYPE(GPIOPinState, uint8_t)
	ETL_ENUM_TYPE(RESET	, "Reset")
	ETL_ENUM_TYPE(SET	, "Set"  )
	ETL_END_ENUM_TYPE
};

struct GPIOPinEvent
{
	enum enum_type
	{
		NONE = 0,
		RISING = 1,
		FALLING = 2,
		CHANGE = 3
	};

	ETL_DECLARE_ENUM_TYPE(GPIOPinEvent, uint8_t)
	ETL_ENUM_TYPE(NONE		, "None"	)
	ETL_ENUM_TYPE(RISING	, "Rising"	)
	ETL_ENUM_TYPE(FALLING	, "Falling"	)
	ETL_ENUM_TYPE(CHANGE	, "Change"	)
	ETL_END_ENUM_TYPE
};

class GPIO final
{
private:
	GPIO_TypeDef* _GPIO;
	std::uint16_t _pin;
	GPIOPinState  _lastState = GPIOPinState::RESET;

public:
	constexpr
	GPIO(GPIO_TypeDef* GPIO, std::uint16_t pin): _GPIO(GPIO), _pin(pin) { }

	exception::result_type<void>
	lock() const noexcept
	{
		if (HAL_GPIO_LockPin(_GPIO, _pin) == HAL_ERROR)
		{
			return exception::error_type(exception::create<exception::GPIO_lock_exception>(__FILE__, __LINE__));
		}
		return exception::result_type<void>();
	}

	exception::result_type<GPIOPinState>
	get() const noexcept
	{
		return exception::result_type<GPIOPinState>(GPIOPinState(HAL_GPIO_ReadPin(_GPIO, _pin)));
	}
	exception::result_type<GPIOPinEvent>
	event() noexcept
	{
		GPIOPinState current = get().value_or(GPIOPinState::RESET);
		GPIOPinEvent result = GPIOPinEvent::NONE;
		if (_lastState.get_value() != current.get_value())
		{
			result = current.get_value() ? GPIOPinEvent::RISING : GPIOPinEvent::FALLING;
		}
		_lastState = current;
		return result;
	}

	exception::result_type<void>
	set(GPIOPinState value) const noexcept
	{
		HAL_GPIO_WritePin(_GPIO, _pin, static_cast<GPIO_PinState>(value.get_value()));
		return exception::result_type<void>();
	}

	exception::result_type<void>
	togle() const noexcept
	{
		HAL_GPIO_TogglePin(_GPIO, _pin);
		return exception::result_type<void>();
	}
};

} /* namespace interfaces */
} /* namespace is */

#endif

#endif /* INTERFACES_GPIO_H_ */
