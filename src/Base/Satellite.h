#ifndef BASE_SATELLITE_H_
#define BASE_SATELLITE_H_

#include <stdint.h>
#include <memory>
#include <map>

#include "Device.h"

namespace IntroSatLib
{
namespace Base
{
class Satellite
{
private:
	DeviceID AddToMap(std::shared_ptr<Device> device, MaskDeviceID mask)
	{
		DeviceID id = device->ID() & mask;
		while (_devices.count(id) == 1)
		{
			id++;
		}
		_devices[id] = device;
		return id;
	}
protected:
	std::map<DeviceID, std::shared_ptr<Device>> _devices;

public:
	virtual DeviceID AddDevice(std::shared_ptr<Device> device)
	{
	}
	;

	Status UpdateData()
	{
		Status result = Status::Ok;
		DeviceID id = DeviceID::All;
		while (_devices.count(id) == 1 && result == Status::Ok)
		{
			result = _devices[id]->UpdateData();
			id++;
		}
		return result;
	}
};

}
}

#endif /* BASE_SATELLITE_H_ */
