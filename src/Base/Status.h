#ifndef BASE_STATUS_H_
#define BASE_STATUS_H_

#include "../Includes/Base.h"

namespace IntroSatLib
{
namespace Base
{
enum class Status : uint32_t
{
	Ok = 0, Error = 1, Busy = 2, Timeout = 4, NotConnected = 8,
};

Status operator|(const Status &a, const Status &b);
Status operator|=(Status &a, const Status &b);

class StatusConverter
{

#ifdef INTROSAT_USE_HAL
public:
	static Status Convert(HAL_StatusTypeDef status);
#endif /* INTROSAT_USE_HAL */

#ifdef INTROSAT_USE_ARDUINO
			public:
				static Status Convert(int status);
#endif /* INTROSAT_USE_ARDUINO */

};
}

}

#endif /* BASE_STATUS_H_ */
