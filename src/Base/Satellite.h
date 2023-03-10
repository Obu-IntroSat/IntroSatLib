#ifndef BASE_SATELLITE_H_
#define BASE_SATELLITE_H_

#include <stdint.h>
#include <memory>
#include <map>

#include "./Device.h"

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
					while(_devices.count(id) == 1)
					{
						id++;
					}
					_devices[id] =  device;
					return id;
				}
			protected:
				std::map<DeviceID, std::shared_ptr<Device>> _devices;

			public:
				virtual DeviceID AddDevice(std::shared_ptr<Device> device)
				{
					AddToMap(device, MaskDeviceID::Unknown);
					AddToMap(device, MaskDeviceID::Product);
					AddToMap(device, MaskDeviceID::Type);
					AddToMap(device, MaskDeviceID::Type | MaskDeviceID::Product);
					AddToMap(device, MaskDeviceID::Interface);
					AddToMap(device, MaskDeviceID::Interface | MaskDeviceID::Product);
					AddToMap(device, MaskDeviceID::Interface | MaskDeviceID::Type);
					return AddToMap(device, MaskDeviceID::Interface | MaskDeviceID::Type | MaskDeviceID::Product);
				};
		};

	}
}

#endif /* BASE_SATELLITE_H_ */
