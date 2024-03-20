#ifndef MagnetometerV2_H_
#define MagnetometerV2_H_

#include "I2CDevice.h"
#include "BaseDevice.h"
#include "Quaternion/Quaternion.h"
#include <cmath>
#include <array>

namespace IntroSatLib {

class MagnetometerV2: public BaseDevice {
private:
	static const uint8_t BASE_ADDRESS = 0x1E;

public:
#ifndef ARDUINO
	MagnetometerV2(I2C_HandleTypeDef *hi2c, uint8_t address = BASE_ADDRESS);
#else
	MagnetometerV2(TwoWire &hi2c, uint8_t address = BASE_ADDRESS);
	MagnetometerV2(uint8_t address = BASE_ADDRESS);
#endif

	MagnetometerV2(const MagnetometerV2& other);
	MagnetometerV2& operator=(const MagnetometerV2& other);
	MagnetometerV2(MagnetometerV2&& other);
	MagnetometerV2& operator=(MagnetometerV2&& other);

	uint8_t Init();
public:
	int16_t RawX();
	int16_t RawY();
	int16_t RawZ();
	Quaternion<float> GetQuaternion();

	virtual ~MagnetometerV2();
};

} /* namespace IntroSatLib */

#endif /* MagnetometerV2_H_ */
