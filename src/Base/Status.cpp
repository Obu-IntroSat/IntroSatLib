#include "./Status.h"

namespace IntroSatLib
{
	namespace Base
	{

		Status operator| (const Status& a, const Status& b)
		{
			return (Status)((uint32_t&)a | (uint32_t&)b);
		}

		Status operator|= (Status& a, const Status& b)
		{
			return (Status)((uint32_t&)a |= (uint32_t&)b);
		}

#ifdef INTROSAT_USE_HAL
		Status StatusConverter::Convert(HAL_StatusTypeDef status)
		{
			switch(status)
			{
				case HAL_OK:
					return Status::Ok;
				case HAL_ERROR:
					return Status::Error;
				case HAL_BUSY:
					return Status::Busy;
				case HAL_TIMEOUT:
					return Status::Timeout;
				default:
					return Status::Error;
			}
		}
#endif /* INTROSAT_USE_HAL */

#ifdef INTROSAT_USE_ARDUINO
		Status StatusConverter::Convert(int status)
		{
			switch(status)
			{
				case 0:
					return Status::Ok;
				case 1:
					return Status::Error;
				case 2:
					return Status::Busy;
				case 3:
					return Status::Timeout;
				default:
					return Status::Error;
			}
		}
#endif /*INTROSAT_USE_ARDUINO */
	}
}


