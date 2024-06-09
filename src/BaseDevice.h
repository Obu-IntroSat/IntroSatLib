#ifndef BASEDEVICE_H_
#define BASEDEVICE_H_

#include "I2CDevice.h"

namespace IntroSatLib {

class BaseDevice {

protected:
	I2CDevice &_i2c;

	virtual uint8_t GetRegister(uint8_t reg);
	virtual void SetRegister(uint8_t reg, uint8_t value);
	virtual void SetBitRegister(uint8_t reg, uint8_t bit);
	virtual void ResetBitRegister(uint8_t reg, uint8_t bit);
	virtual void BitRegister(uint8_t reg, uint8_t bit, uint8_t value);

public:
#ifndef ARDUINO
	BaseDevice(I2C_HandleTypeDef *hi2c, uint8_t address);
#else
	BaseDevice(TwoWire &hi2c, uint8_t address);
	BaseDevice(uint8_t address);
#endif
	BaseDevice(const BaseDevice& other);
	BaseDevice(BaseDevice&& other);
	BaseDevice& operator=(const BaseDevice& other);
	BaseDevice& operator=(BaseDevice&& other);
	virtual uint8_t Init();
	virtual ~BaseDevice();
};

} /* namespace IntroSatLib */

#endif /* BASEDEVICE_H_ */
