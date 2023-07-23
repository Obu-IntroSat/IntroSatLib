#ifndef BASE_INTERFACE_H_
#define BASE_INTERFACE_H_

#include <stdint.h>
#include "Status.h"
#include "Endian.h"
#include "DeviceID.h"

namespace IntroSatLib
{
namespace Base
{
class Interface
{
public:
	virtual Status IsReady();

	virtual DeviceID ID() const;

	virtual Status Read(uint8_t *data, uint8_t Nbytes);
	virtual Status Read(uint8_t reg, uint8_t *data, uint8_t Nbytes);
	Status Read(uint8_t reg, uint8_t &data, Endian end = Endian::LE);
	Status Read(uint8_t reg, uint16_t &data, Endian end = Endian::LE);
	Status Read(uint8_t reg, uint32_t &data, Endian end = Endian::LE);
	Status Read(uint8_t reg, uint64_t &data, Endian end = Endian::LE);

	virtual Status Write(uint8_t *data, uint8_t Nbytes);
	virtual Status Write(uint8_t reg, uint8_t *data, uint8_t Nbytes);
	Status Write(uint8_t reg, uint8_t data, Endian end = Endian::LE);
	Status Write(uint8_t reg, uint16_t data, Endian end = Endian::LE);
	Status Write(uint8_t reg, uint32_t data, Endian end = Endian::LE);
	Status Write(uint8_t reg, uint64_t data, Endian end = Endian::LE);
};
}
}

#endif /* BASE_INTERFACE_H_ */
