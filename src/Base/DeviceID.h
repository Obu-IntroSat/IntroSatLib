#ifndef BASE_DEVICEID_H_
#define BASE_DEVICEID_H_

#include "stdint.h"

namespace IntroSatLib
{
	namespace Base
	{

		enum class DeviceID: uint32_t
		{

			Input = 0x00000000,
			Output = 0x80000000,

			OneDimensional = 0x01000000,
			TwoDimensional = 0x02000000,
			ThreeDimensional = 0x03000000,

			Accelerometer = 0x00010000,
			Gyroscope = 0x00020000,

			MPU9250ID = 0x00000100,

		};
		enum class MaskDeviceID: uint32_t
		{
			Unknown = 0x00000000,
			Number = 0x000000FF,
			Product = 0x0000FF00,
			Type = 0x00FF0000,
			Interface = 0xFF000000,

		};

		DeviceID operator| (const DeviceID& a, const DeviceID& b);
		DeviceID operator|= (DeviceID& a, const DeviceID& b);

		MaskDeviceID operator| (const MaskDeviceID& a, const MaskDeviceID& b);
		MaskDeviceID operator|= (MaskDeviceID& a, const MaskDeviceID& b);

		DeviceID operator& (const DeviceID& a, const MaskDeviceID& b);
		DeviceID operator&= (DeviceID& a, const MaskDeviceID& b);

		DeviceID operator++(DeviceID& a, int);

	}
}

#endif /* BASE_DEVICEID_H_ */
