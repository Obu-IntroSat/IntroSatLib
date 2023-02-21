#ifndef DEVICES_THREEDIMENSIONALDEVICE_H_
#define DEVICES_THREEDIMENSIONALDEVICE_H_

#include <stdint.h>

#include "../Base/Device.h"

using namespace IntroSatLib::Base;

namespace IntroSatLib
{
	namespace Devices
	{

		template<typename T, int BufferSize>
		class ThreeDimensionalDevice: public Device
		{
			private:
				int _position = 0;
				T _bufferX[BufferSize] = {0};
				T _averageX = 0;
				T _bufferY[BufferSize] = {0};
				T _averageY = 0;
				T _bufferZ[BufferSize] = {0};
				T _averageZ = 0;

			protected:
				ThreeDimensionalDevice(std::unique_ptr<Interface> interface): Device(std::move(interface)) { };

				virtual T ConvertValue(int32_t value) const;

				virtual int32_t RawX(Status *status) const;
				virtual int32_t RawY(Status *status) const;
				virtual int32_t RawZ(Status *status) const;

			public:
				Status UpdateData() override final;
				T X() const;
				T Y() const;
				T Z() const;
		};

	}
}

#endif /* DEVICES_THREEDIMENSIONALDEVICE_H_ */
