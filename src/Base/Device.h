#ifndef DEVICES_DEVICE_H_
#define DEVICES_DEVICE_H_

#include <memory>

#include "Interface.h"

namespace IntroSatLib
{
	namespace Base
	{
		class Device
		{
			protected:
				std::unique_ptr<Interface> _interface;
				Device(std::unique_ptr<Interface> interface): _interface(std::move(interface)) { };

			public:
				virtual Status Init();
				virtual Status UpdateData();
		};

	}
}

#endif /* DEVICES_DEVICE_H_ */
