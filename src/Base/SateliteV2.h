#ifndef BASE_SATELITEV2_H_
#define BASE_SATELITEV2_H_

#include <map>
#include <vector>
#include <memory>
#include <string>
#include "DeviceID.h"
#include "Status.h"
#include "StackTrace.h"

namespace IntroSatLib
{
namespace Base
{

class SateliteV2
{
public:
	class DeviceV2
	{
		friend SateliteV2;
	private:
		DeviceID _id;
		void ForceChangeID(uint8_t newID);

	protected:

		virtual Status InnerPreInit(const SateliteV2 &satelite) = 0;
		virtual Status InnerInit(const SateliteV2 &satelite) = 0;
		virtual Status InnerPostInit(const SateliteV2 &satelite) = 0;

		virtual Status InnerPreUpdate(const SateliteV2 &satelite) = 0;
		virtual Status InnerUpdate(const SateliteV2 &satelite) = 0;
		virtual Status InnerPostUpdate(const SateliteV2 &satelite) = 0;

	public:
		DeviceID ID();
		Status PreInit(const SateliteV2 &satelite);
		Status Init(const SateliteV2 &satelite);
		Status PostInit(const SateliteV2 &satelite);
		Status PreUpdate(const SateliteV2 &satelite);
		Status Update(const SateliteV2 &satelite);
		Status PostUpdate(const SateliteV2 &satelite);

	};

private:
	std::map<DeviceID, std::shared_ptr<DeviceV2>> _treeDevices;
	std::vector<std::shared_ptr<DeviceV2>> _listDevices;

protected:
	Status AddDeviceToList(std::shared_ptr<DeviceV2> device, uint8_t index)
	{
		auto iterator = _listDevices.begin();
		if (index >= _listDevices.size())
		{
			index = _listDevices.size();
			_listDevices.push_back(device);
		}
		else
		{
			iterator += index;
			_listDevices.insert(iterator, device);
		}
		for (uint8_t newIndex = index; newIndex < _listDevices.size(); newIndex++)
		{
			(*iterator)->ForceChangeID(newIndex);
		}
		return Status::Ok;
	}

public:
	Status Init() const
	{
		uint8_t size = _listDevices.size();
		std::vector<Status> results(size);

		for (uint8_t i = 0; i < size; i++)
		{
			results[i] |= (_listDevices[i])->PreInit(*this);
		}
		for (uint8_t i = 0; i < size; i++)
		{
			results[i] |= _listDevices[i]->Init(*this);
		}
		for (uint8_t i = 0; i < size; i++)
		{
			results[i] |= _listDevices[i]->PostUpdate(*this);
		}


		return Status::Ok;
	}

	Status Update() const
	{
		uint8_t size = _listDevices.size();
		std::vector<Status> results(size);

		for (uint8_t i = 0; i < size; i++)
		{
			results[i] |= (_listDevices[i])->PreUpdate(*this);
		}
		for (uint8_t i = 0; i < size; i++)
		{
			results[i] |= _listDevices[i]->Update(*this);
		}
		for (uint8_t i = 0; i < size; i++)
		{
			results[i] |= _listDevices[i]->PostInit(*this);
		}
		return Status::Ok;
	}
};

}
}

#endif /* BASE_SATELITEV2_H_ */
