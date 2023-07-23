#ifndef LIGHTSENSOR_H_
#define LIGHTSENSOR_H_

#include "I2CDevice.h"
#include "BaseDevice.h"

namespace IntroSatLib
{

class LightSensor: public virtual BaseDevice
{
private:

	static const uint8_t BASE_ADDRESS = 0x50;

public:
#ifndef ARDUINO
	LightSensor(I2C_HandleTypeDef *hi2c, uint8_t address = BASE_ADDRESS);
#else
	LightSensor(TwoWire &hi2c, uint8_t address = BASE_ADDRESS);
#endif

	LightSensor(const LightSensor &other);
	LightSensor(LightSensor &&other);
	LightSensor& operator=(const LightSensor &other);
	LightSensor& operator=(LightSensor &&other);

	uint8_t Init();

	int16_t GetLight();
	~LightSensor();
};

} /* namespace IntroSatLib */

#endif /* LIGHTSENSOR_H_ */
