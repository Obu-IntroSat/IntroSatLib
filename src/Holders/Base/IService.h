#ifndef _HOLDERS_BASE_ISERVICE_H_
#define _HOLDERS_BASE_ISERVICE_H_

#include "./Types.h"
#include "./IHolder.h"
#include <cinttypes>

namespace IntroSatLib
{
	namespace Holders
	{
		namespace Base
		{
			class IHolder;

			using Error = std::uint32_t;
			class IService
			{
			public:
				virtual Error Start(const IHolder& holder) = 0;
				virtual Error PreUpdate() = 0;
				virtual Error Update() = 0;
				virtual Error PostUpdate() = 0;
			private:
				ServiceID _id;
				ServiceStage _stage;
			public:
				constexpr void SetId(const ServiceID& id)
				{
					_id = id;
				}

				constexpr void SetStage(ServiceStage& stage)
				{
					_stage = stage;
				}

				constexpr const ServiceID& GetId() const
				{
					return _id;
				}

				constexpr const ServiceStage& GetStage() const
				{
					return _stage;
				}
			};
		} /* namespace Base */
	} /* namespace Holders */
} /* namespace IntroSatLib */

#endif /* _HOLDERS_BASE_ISERVICE_H_ */
