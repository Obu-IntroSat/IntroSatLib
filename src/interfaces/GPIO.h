#ifndef INTERFACES_GPIO_H_
#define INTERFACES_GPIO_H_

#include "stm32g4xx_hal_gpio.h"

#include <cinttypes>
#include "../exceptions/base.h"
#include "../../third_party/etl/include/etl/expected.h"
#include "../../third_party/etl/include/etl/enum_type.h"

namespace is {
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

	etl::expected<void, exceptions::base::exception_ref>
	lock() const noexcept
	{
		if (HAL_GPIO_LockPin(&_GPIO, _pin) == HAL_ERROR || 1)
		{
			return etl::unexpected<exceptions::base::exception_ref>
			(exceptions::base::create("text", __FILE__, __LINE__));
		}
		return etl::expected<void, exceptions::base::exception_ref>();
	}

	etl::expected<GPIOPinState, exceptions::base>
	get() const noexcept
	{
		return etl::expected<GPIOPinState, exceptions::base>(GPIOPinState(HAL_GPIO_ReadPin(&_GPIO, _pin)));
	}

	etl::expected<void, exceptions::base>
	set(GPIOPinState value) const noexcept
	{
		HAL_GPIO_WritePin(&_GPIO, _pin, value);
		return etl::expected<void, exceptions::base>();
	}

	etl::expected<void, exceptions::base>
	togle() const noexcept
	{
		HAL_GPIO_TogglePin(&_GPIO, _pin);
		return etl::expected<void, exceptions::base>();
	}
};

} /* namespace interfaces */
} /* namespace is */

#endif /* INTERFACES_GPIO_H_ */
