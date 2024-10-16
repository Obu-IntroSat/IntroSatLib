#ifndef INTERFACES_GPIO_H_
#define INTERFACES_GPIO_H_

#include "stm32g4xx_hal_gpio.h"

#include <cinttypes>
#include "../../third_party/etl/include/etl/expected.h"
#include "../../third_party/etl/include/etl/exception.h"
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
	std::uint8_t _pin;

public:
	constexpr GPIO(GPIO_TypeDef& GPIO, std::uint8_t pin): _GPIO(GPIO), _pin(pin) { }

	etl::expected<void, etl::exception> lock() noexcept
	{
		if (HAL_GPIO_LockPin(&_GPIO, _pin) == HAL_ERROR)
		{
			return etl::unexpected<etl::exception>(etl::exception("Error of locking pin", __FILE__, __LINE__));
		}
		return etl::expected<void, etl::exception>();
	}

	etl::expected<GPIOPinState, etl::exception> get() noexcept
	{
		return etl::expected<GPIOPinState, etl::exception>(GPIOPinState(HAL_GPIO_ReadPin(&_GPIO, _pin)));
	}

	etl::expected<void, etl::exception> set(GPIOPinState value) noexcept
	{
		HAL_GPIO_WritePin(&_GPIO, _pin, value);
		return etl::expected<void, etl::exception>();
	}

	etl::expected<void, etl::exception> togle() noexcept
	{
		HAL_GPIO_TogglePin(&_GPIO, _pin);
		return etl::expected<void, etl::exception>();
	}
};

} /* namespace interfaces */
} /* namespace is */

#endif /* INTERFACES_GPIO_H_ */
