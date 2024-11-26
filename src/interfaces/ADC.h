#ifndef INTERFACES_ADC_H_
#define INTERFACES_ADC_H_


#include "stm32g4xx_hal.h"

#ifdef HAL_ADC_MODULE_ENABLED

#include "stm32g4xx_hal_adc.h"


#include <cinttypes>

#include "../exception/base.h"
#include "../../third_party/etl/include/etl/enum_type.h"

namespace is {

namespace exception {

struct ADC_start_exception: public base {
	ADC_start_exception(string_type file, numeric_type line): base("start adc", file, line) { }
};

struct ADC_start_busy_exception: public base {
	ADC_start_busy_exception(string_type file, numeric_type line): base("start adc with busy", file, line) { }
};

struct ADC_start_timeout_exception: public base {
	ADC_start_timeout_exception(string_type file, numeric_type line): base("start adc with timeout", file, line) { }
};

struct ADC_conversion_exception: public base {
	ADC_conversion_exception(string_type file, numeric_type line): base("conversion adc", file, line) { }
};

struct ADC_conversion_busy_exception: public base {
	ADC_conversion_busy_exception(string_type file, numeric_type line): base("conversion adc with busy", file, line) { }
};

struct ADC_conversion_timeout_exception: public base {
	ADC_conversion_timeout_exception(string_type file, numeric_type line): base("conversion adc with timeout", file, line) { }
};

struct ADC_stop_exception: public base {
	ADC_stop_exception(string_type file, numeric_type line): base("stop adc", file, line) { }
};

struct ADC_stop_busy_exception: public base {
	ADC_stop_busy_exception(string_type file, numeric_type line): base("stop adc with busy", file, line) { }
};

struct ADC_stop_timeout_exception: public base {
	ADC_stop_timeout_exception(string_type file, numeric_type line): base("stop adc with timeout", file, line) { }
};


} /* namespace exception */

namespace interfaces {


class ADC final
{
private:
	ADC_HandleTypeDef* _adc;

public:
	constexpr
	ADC(ADC_HandleTypeDef* adc): _adc(adc) { }

	exception::result_type<std::uint16_t>
	value() const noexcept
	{
		switch(HAL_ADC_Start(_adc))
		{
		case HAL_ERROR:
			return exception::error_type(exception::create<exception::ADC_start_exception>(__FILE__, __LINE__));
		case HAL_BUSY:
			return exception::error_type(exception::create<exception::ADC_start_busy_exception>(__FILE__, __LINE__));
		case HAL_TIMEOUT:
			return exception::error_type(exception::create<exception::ADC_start_timeout_exception>(__FILE__, __LINE__));
		default: break;
		}
		switch(HAL_ADC_PollForConversion(_adc, 100))
		{
		case HAL_ERROR:
			return exception::error_type(exception::create<exception::ADC_conversion_exception>(__FILE__, __LINE__));
		case HAL_BUSY:
			return exception::error_type(exception::create<exception::ADC_conversion_busy_exception>(__FILE__, __LINE__));
		case HAL_TIMEOUT:
			return exception::error_type(exception::create<exception::ADC_conversion_timeout_exception>(__FILE__, __LINE__));
		default: break;
		}

		std::uint16_t result = HAL_ADC_GetValue(_adc);

		switch(ADC_ConversionStop(_adc, ADC_REGULAR_GROUP))
		{
		case HAL_ERROR:
			return exception::error_type(exception::create<exception::ADC_stop_exception>(__FILE__, __LINE__));
		case HAL_BUSY:
			return exception::error_type(exception::create<exception::ADC_stop_busy_exception>(__FILE__, __LINE__));
		case HAL_TIMEOUT:
			return exception::error_type(exception::create<exception::ADC_stop_timeout_exception>(__FILE__, __LINE__));
		default: break;
		}
		return exception::result_type<std::uint16_t>(result);
	}
};

} /* namespace interfaces */
} /* namespace is */

#endif


#endif /* INTERFACES_ADC_H_ */
