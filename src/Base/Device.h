#ifndef DEVICES_DEVICE_H_
#define DEVICES_DEVICE_H_

#include <memory>

#include "DeviceID.h"
#include "Interface.h"

namespace IntroSatLib
{
namespace Base
{
class Device
{
protected:

	DeviceID _id;
	std::unique_ptr<Interface> _interface;

	Device(std::unique_ptr<Interface> interface, DeviceID id) :
			_id(id), _interface(std::move(interface))
	{
	}

public:
	DeviceID ID() const
	{
		return _id;
	}
	virtual Status Init();
	virtual Status UpdateData();

};

}
}

#endif /* DEVICES_DEVICE_H_ */
