#ifndef _SERVICES_GYROSCOPE_H_
#define _SERVICES_GYROSCOPE_H_

#include "./Base/AxisService.h"
#include <cinttypes>
#include <vector>
#include <functional>

namespace IntroSatLib
{
	namespace Services
	{
		using Holders::Base::Error;

		template<std::size_t NInputs>
		class Gyroscope: public Base::AxisService<NInputs>
		{
		};
	} /* namespace Services */
} /* namespace IntroSatLib */

#endif /* _SERVICES_GYROSCOPE_H_ */
