#ifndef _HOLDERS_HOLDER_H_
#define _HOLDERS_HOLDER_H_

#include <vector>
#include <set>
#include <map>
#include <memory>
#include <functional>

#include "./Base/IHolder.h"
#include "./Base/IService.h"

namespace IntroSatLib
{
	namespace Holders
	{
		class Holder: public Base::IHolder
		{
		private:
			struct IServiceCompare : public std::binary_function<const Base::ServiceReference, const Base::ServiceReference, bool>
			{
				bool operator()(const Base::ServiceReference &lhs, const Base::ServiceReference &rhs) const
				{
					return lhs->GetStage() < rhs->GetStage();
				}
			};
		private:
			std::multiset<Base::ServiceReference, IServiceCompare> _list;
			std::map<Base::TypeId, std::vector<Base::ServiceReference>> _tree;
		protected:
			void RegisterByInteraface(Base::TypeId typeId, Base::ServiceReference service) override
			{
				const auto &toPlace = _tree.emplace(typeId, std::vector<Base::ServiceReference>());
				if (!toPlace.second)
				{
					for (const Base::ServiceReference& placedDervice : toPlace.first->second)
					{
						if (service->GetId().compare(placedDervice->GetId()))
						{
							return;
						}
					}
				}
				toPlace.first->second.push_back(service);

			}
			void RegisterByInstance(Base::TypeId typeId, Base::ServiceReference service) override
			{
				_list.insert(service);
				RegisterByInteraface(typeId, service);
			}

		private:

			auto GetIteratorByClass(Base::TypeId typeId) const
			{
				return _tree.find(typeId);
			}

		protected:
			Base::ServiceReference GetByClass(Base::TypeId typeId) const override
			{
				auto find = GetIteratorByClass(typeId);
				if (find == _tree.end())
				{
					return Base::ServiceReference(nullptr);
				}
				return find->second[0];
			}
			Base::ServiceReference GetById(Base::TypeId typeId, Base::ServiceID id) const override
			{
				auto find = GetIteratorByClass(typeId);
				if (find == _tree.end())
				{
					return Base::ServiceReference(nullptr);
				}
				for (const Base::ServiceReference &service : find->second)
				{
					if (service->GetId().compare(id))
					{
						return service;
					}
				}
				return Base::ServiceReference(nullptr);
			}

		public:
			Base::Error Start() override
			{
				for(auto& service : _list)
				{
					auto result = service->Start(*this);
					if(result.has_value()) { continue; }
				}
				return 0;
			}
			Base::Error Update() override
			{
				for(auto& service : _list)
				{
					auto result = service->PreUpdate();
					if(result.has_value()) { continue; }
				}
				for(auto& service : _list)
				{
					auto result = service->Update();
					if(result.has_value()) { continue; }
				}
				for(auto& service : _list)
				{
					auto result = service->PostUpdate();
					if(result.has_value()) { continue; }
				}
				return 0;
			}
		};
	} /* namespace Holders */
} /* namespace IntroSatLib */
#endif /* _HOLDERS_HOLDER_H_ */
