#ifndef _HOLDERS_BASE_TYPES_H_
#define _HOLDERS_BASE_TYPES_H_

#include <string_view>
#include <cinttypes>

namespace IntroSatLib
{
	namespace Holders
	{
		namespace Base
		{
			using ServiceStage = std::uint8_t;
			using ServiceID = std::string_view;
		} /* namespace Base */
	} /* namespace Holders */
} /* namespace IntroSatLib */

#endif /* _HOLDERS_BASE_TYPES_H_ */
