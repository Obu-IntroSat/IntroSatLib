#ifndef _HOLDERS_BASE_IHOLDER_H_
#define _HOLDERS_BASE_IHOLDER_H_

#include "./Types.h"
#include "./IService.h"
#include "../../Base/GetTypeId.h"
#include <cinttypes>
#include <string>
#include <memory>
#include <type_traits>

namespace IntroSatLib
{
	namespace Holders
	{
		namespace Base
		{
			using TypeId = IntroSatLib::Base::TypeId;

			template<class TService, typename std::enable_if<std::is_base_of<IService, TService>::value>::type* = nullptr>
			using Reference = std::shared_ptr<TService>;

			using ServiceReference = Reference<IService>;

			using IntroSatLib::Base::GetTypeId;
			class IHolder
			{
			public:

				struct RegisterSetting
				{
				private:
					IHolder* _holder;
					ServiceReference _service;
				public:
					RegisterSetting(IHolder* holder, ServiceReference service):
						_holder(holder), _service(service) { }
					template<class TService>
					const RegisterSetting& As()
					{
						_holder->RegisterByInteraface(GetTypeId<TService>(), _service);
						return *this;
					}
				};

			protected:

				virtual void RegisterByInteraface(TypeId typeId, ServiceReference service) = 0;
				virtual void RegisterByInstance(TypeId typeId, ServiceReference service) = 0;

				virtual Base::ServiceReference GetByClass(TypeId typeId) const = 0;
				virtual Base::ServiceReference GetById(TypeId typeId, ServiceID id)  const = 0;

			public:
				template<class TService, typename... Args, typename std::enable_if<std::is_base_of<IService, TService>::value>::type* = nullptr>
				RegisterSetting Register(ServiceStage stage, ServiceID id, Args&&... args)
				{
					ServiceReference service = std::make_shared<TService>(std::forward<Args>(args)...);
					service->SetStage(stage);
					service->SetId(id);
					RegisterByInstance(GetTypeId<TService>(), service);
					return {this, service};
				}

				template<class TService, typename std::enable_if<std::is_base_of<IService, TService>::value>::type* = nullptr>
				std::shared_ptr<TService> Get() const
				{
					return std::static_pointer_cast<TService>(GetByClass(GetTypeId<TService>()));
				}

				template<class TService, typename std::enable_if<std::is_base_of<IService, TService>::value>::type* = nullptr>
				std::shared_ptr<TService> Get(ServiceID id) const
				{
					return std::static_pointer_cast<TService>(GetById(GetTypeId<TService>(), id));
				}

				virtual Error Start() = 0;
				virtual Error Update() = 0;
			};

		} /* namespace Base */
	} /* namespace Holders */
} /* namespace IntroSatLib */

#endif /* _HOLDERS_BASE_IHOLDER_H_ */
