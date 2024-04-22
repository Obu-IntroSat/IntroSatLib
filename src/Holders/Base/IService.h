#ifndef _HOLDERS_BASE_ISERVICE_H_
#define _HOLDERS_BASE_ISERVICE_H_

#include "./Types.h"
#include "./IHolder.h"
#include <cinttypes>
#include "../../Base/Expected.h"
#include "../../Base/Error.h"

namespace IntroSatLib
{
	namespace Holders
	{
		namespace Base
		{
			using IntroSatLib::Base::Error;
			class IHolder;

			class IService
			{
			public:
				virtual tl::expected<void, Error> Start(const IHolder& holder) = 0;
				virtual tl::expected<void, Error> PreUpdate() = 0;
				virtual tl::expected<void, Error> Update() = 0;
				virtual tl::expected<void, Error> PostUpdate() = 0;
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
