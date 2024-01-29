#ifndef _SERVICES_USARTSERVICE_H_
#define _SERVICES_USARTSERVICE_H_

#include "../Holders/Base/Types.h"
#include "../Holders/Base/IHolder.h"

namespace IntroSatLib
{
	namespace Interfaces
	{
		class USARTService: public Holders::Base::IService
		{
		public:
			Holders::Base::Error Start(const Holders::Base::IHolder& holder) override
			{
				return 0;
			}
			Holders::Base::Error PreUpdate() override
			{
				return 0;
			}
			Holders::Base::Error Update() override
			{
				return 0;
			}
			Holders::Base::Error PostUpdate() override
			{
				return 0;
			}
		};
	} /* namespace Services */
} /* namespace IntroSatLib */

#endif /* _SERVICES_USARTSERVICE_H_ */
