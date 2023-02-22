#ifndef AK8963_H_
#define AK8963_H_

#include "I2CDevice.h"
#include "BaseDevice.h"

namespace IntroSatLib {

class AK8963: public BaseDevice {
private:
	static const uint8_t BASE_ADDRESS = 0x0C;
	uint16_t _x = 0;
	uint16_t _y = 0;
	uint16_t _z = 0;
	uint8_t _calX = 0;
	uint8_t _calY = 0;
	uint8_t _calZ = 0;

public:
#ifndef ARDUINO
	AK8963(I2C_HandleTypeDef *hi2c, uint8_t address = BASE_ADDRESS);
#else
	AK8963(TwoWire &hi2c, uint8_t address = BASE_ADDRESS);
	AK8963(uint8_t address = BASE_ADDRESS);
#endif

	AK8963(const AK8963& other);
	AK8963& operator=(const AK8963& other);
	AK8963(AK8963&& other);
	AK8963& operator=(AK8963&& other);

	uint8_t Init();

	void Read();

	int16_t RawX();
	int16_t RawY();
	int16_t RawZ();

	virtual ~AK8963();
};

} /* namespace IntroSatLib */

#endif /* AK8963_H_ */
