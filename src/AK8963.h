#ifndef AK8963_H_
#define AK8963_H_

#include "Device/I2CDevice.h"
//#include "../Device.h"

namespace IntroSatLib {

class AK8963: private I2CDevice {
private:
	static const uint8_t BASE_ADDRESS = 0x0C;
	static constexpr float _rawmt = 8190.0f / 4912.0f; //microTesla

	uint16_t _x = 0;
	uint16_t _y = 0;
	uint16_t _z = 0;
	uint8_t _calX = 0;
	uint8_t _calY = 0;
	uint8_t _calZ = 0;

	ISL_StatusTypeDef ReadCal();

public:
	AK8963(const interfaces::I2C &i2c, uint8_t address = BASE_ADDRESS);

	AK8963(const AK8963& other);
	AK8963& operator=(const AK8963& other);
	AK8963(AK8963&& other);
	AK8963& operator=(AK8963&& other);

	ISL_StatusTypeDef Init();

	ISL_StatusTypeDef Read();

	int16_t RawX();
	int16_t RawY();
	int16_t RawZ();

	float X();
	float Y();
	float Z();

	~AK8963() override;
};

} /* namespace IntroSatLib */

#endif /* AK8963_H_ */
