#ifndef INTERFACES_GPIO_H_
#define INTERFACES_GPIO_H_

#include "stm32g4xx_hal.h"

#ifdef HAL_GPIO_MODULE_ENABLED

#include "stm32g4xx_hal_gpio.h"


#include <cinttypes>

#include "../exception/base.h"
#include "../../third_party/etl/include/etl/expected.h"
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

	ETL_DECLARE_ENUM_TYPE(GPIOPinState, GPIO_PinState)
	ETL_ENUM_TYPE(RESET	, "Reset")
	ETL_ENUM_TYPE(SET	, "Set"  )
	ETL_END_ENUM_TYPE
};

class GPIO final
{
private:
	GPIO_TypeDef& _GPIO;
	std::uint16_t _pin;

public:
	constexpr
	GPIO(GPIO_TypeDef& GPIO, std::uint16_t pin): _GPIO(GPIO), _pin(pin) { }

	etl::expected<void, exception::base::exception_ref>
	lock() const noexcept
	{
		if (HAL_GPIO_LockPin(&_GPIO, _pin) == HAL_ERROR || 1)
		{
			return etl::unexpected<exception::base::exception_ref>
			(exception::create<exception::GPIO_lock_exception>(__FILE__, __LINE__));
		}
		return etl::expected<void, exception::base::exception_ref>();
	}

	etl::expected<GPIOPinState, exception::base>
	get() const noexcept
	{
		return etl::expected<GPIOPinState, exception::base>(GPIOPinState(HAL_GPIO_ReadPin(&_GPIO, _pin)));
	}

	etl::expected<void, exception::base>
	set(GPIOPinState value) const noexcept
	{
		HAL_GPIO_WritePin(&_GPIO, _pin, value);
		return etl::expected<void, exception::base>();
	}

	etl::expected<void, exception::base>
	togle() const noexcept
	{
		HAL_GPIO_TogglePin(&_GPIO, _pin);
		return etl::expected<void, exception::base>();
	}
};

} /* namespace interfaces */
} /* namespace is */

#endif

#endif /* INTERFACES_GPIO_H_ */
