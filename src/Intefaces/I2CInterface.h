#ifndef INTEFACES_I2CINTERFACE_H_
#define INTEFACES_I2CINTERFACE_H_

#include "../Base/Interface.h"
#include "../Includes/I2C.h"

using namespace IntroSatLib::Base;

namespace IntroSatLib
{
namespace Interfaces
{

#ifdef INTROSAT_HAVE_I2C

class I2CInterface: public Interface
{

#ifdef INTROSAT_USE_HAL
private:
	I2C_HandleTypeDef *_i2c;
public:
	I2CInterface(I2C_HandleTypeDef *i2c, uint8_t address);
#endif /* INTROSAT_USE_HAL */

#ifdef INTROSAT_USE_ARDUINO
			private:
				TwoWire &_i2c;
			public:
				I2CInterface(TwoWire &i2c, uint8_t address);
#endif /* INTROSAT_USE_ARDUINO */

private:
	uint8_t _address;

public:
	uint8_t Address();
	virtual DeviceID ID() const override;
	virtual Status IsReady() override;
	virtual Status Read(uint8_t *Data, uint8_t Nbytes) override;
	virtual Status Read(uint8_t Register, uint8_t *Data, uint8_t Nbytes)
			override;
	virtual Status Write(uint8_t *Data, uint8_t Nbytes) override;
	virtual Status Write(uint8_t Register, uint8_t *Data, uint8_t Nbytes)
			override;
};
}

}

#endif /* INTROSAT_HAVE_I2C */

#endif /* INTEFACES_I2CINTERFACE_H_ */
