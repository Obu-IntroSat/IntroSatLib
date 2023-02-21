#ifndef BASE_STATUS_H_
#define BASE_STATUS_H_

#ifndef ARDUINO
#include "stm32f1xx_hal.h"
#else
#include "Arduino.h"
#endif

namespace IntroSatLib
{
	namespace Base
	{
		enum class Status
		{
			Ok = 0,
			Error = 1,
			Busy = 2,
			Timeout = 4,
			NotConnected = 8,
		};

		class StatusConverter
		{

#ifndef ARDUINO
			public:
				static Status Convert(HAL_StatusTypeDef status);
#else
			public:
				static Status Convert(int status);
#endif
		};
	}

}

#endif /* BASE_STATUS_H_ */
