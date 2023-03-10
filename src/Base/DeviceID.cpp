#include "./DeviceID.h"

namespace IntroSatLib
{
	namespace Base
	{
		DeviceID operator| (const DeviceID& a, const DeviceID& b)
		{
			return (DeviceID)((uint32_t&)a | (uint32_t&)b);
		};

		DeviceID operator|= (DeviceID& a, const DeviceID& b)
		{
			return (DeviceID)((uint32_t&)a |= (uint32_t&)b);
		};

		MaskDeviceID operator| (const MaskDeviceID& a, const MaskDeviceID& b)
		{
			return (MaskDeviceID)((uint32_t&)a | (uint32_t&)b);
		};

		MaskDeviceID operator|= (MaskDeviceID& a, const MaskDeviceID& b)
		{
			return (MaskDeviceID)((uint32_t&)a |= (uint32_t&)b);
		};

		DeviceID operator& (const DeviceID& a, const MaskDeviceID& b)
		{
			return (DeviceID)((uint32_t&)a & (uint32_t&)b);
		};

		DeviceID operator&= (DeviceID& a, const MaskDeviceID& b)
		{
			return (DeviceID)((uint32_t&)a &= (uint32_t&)b);
		};

		DeviceID operator++(DeviceID& a, int)
		{
			return (DeviceID)((uint32_t&)a)++;
		};
	}
}
