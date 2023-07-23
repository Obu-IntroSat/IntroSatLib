#ifndef BASE_DEVICEID_H_
#define BASE_DEVICEID_H_

#include "stdint.h"
#include "../Includes/I2C.h"
#include "../Includes/UART.h"

namespace IntroSatLib
{
namespace Base
{

enum class DeviceID : uint32_t
{
	Virtual = 0x80000000,
	Quaternion = 0x08000000,
	Direct = 0x04000000,
	Memory = 0x02000000,
	Channel = 0x01000000,
	Accelerometer = 0x00080000,
	Gyroscope = 0x00100000,
	Magnetometer = 0x00180000,
	Motor = 0x00200000,
	Coil = 0x00280000,
#ifdef INTROSAT_HAVE_UART
	UART = 0x00006000,
#endif
#ifdef INTROSAT_HAVE_SPI
	SPI			=	0x00005000,
#endif
#ifdef INTROSAT_HAVE_I2C
	I2C = 0x00004000,
#endif
	PWM = 0x00003000,
	TIM = 0x00002000,
	GPIO = 0x00001000,
	All = 0x00000000,
};
enum class MaskDeviceID : uint32_t
{
	Virtual = 0x80000000,

	Debug = 0x40000000,
	Direct = 0x04000000,
	Memory = 0x02000000,
	Channel = 0x01000000,

	DeviceType = 0x00F80000,
	DeviceID = 0x00070000,
	InterfaceID = 0x00000F00,
	DevicePK = 0x00000FFF,
};

DeviceID operator|(const DeviceID &a, const DeviceID &b);
DeviceID operator|=(DeviceID &a, const DeviceID &b);

DeviceID GenerateDeviceID(uint32_t id);

MaskDeviceID operator|(const MaskDeviceID &a, const MaskDeviceID &b);
MaskDeviceID operator|=(MaskDeviceID &a, const MaskDeviceID &b);

DeviceID operator&(const DeviceID &a, const MaskDeviceID &b);
DeviceID operator&=(DeviceID &a, const MaskDeviceID &b);

DeviceID operator++(DeviceID &a, int);

}
}

#endif /* BASE_DEVICEID_H_ */
