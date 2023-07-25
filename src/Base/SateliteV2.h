#ifndef BASE_SATELITEV2_H_
#define BASE_SATELITEV2_H_

#include <map>
#include <vector>
#include <memory>
#include <string>

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
		std::string _name;
		void ForceChangeID(std::uint8_t newID);

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
	std::map<std::uint32_t, std::shared_ptr<DeviceV2>, std::greater<std::uint32_t>> _listDevices;

protected:
	Status AddDeviceToList(std::shared_ptr<DeviceV2> device, std::uint32_t index)
	{

	}

public:
	Status Init() const
	{
		std::uint8_t size = _listDevices.size();
		std::vector<Status> results(size);

		for (auto const& [key, device] : _listDevices)
		{
			device->PreInit(*this);
		}
		for (auto const& [key, device] : _listDevices)
		{
			device->Init(*this);
		}
		for (auto const& [key, device] : _listDevices)
		{
			device->PostInit(*this);
		}


		return Status::Ok;
	}

	Status Update() const
	{
		std::uint8_t size = _listDevices.size();
		std::vector<Status> results(size);

		for (std::uint8_t i = 0; i < size; i++)
		{
			results[i] |= (_listDevices[i])->PreUpdate(*this);
		}
		for (std::uint8_t i = 0; i < size; i++)
		{
			results[i] |= _listDevices[i]->Update(*this);
		}
		for (std::uint8_t i = 0; i < size; i++)
		{
			results[i] |= _listDevices[i]->PostInit(*this);
		}
		return Status::Ok;
	}
};

}
}

#endif /* BASE_SATELITEV2_H_ */
