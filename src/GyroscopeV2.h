#ifndef GYROSCOPE_H_
#define GYROSCOPE_H_

#include "I2CDevice.h"
#include "BaseDevice.h"

namespace IntroSatLib {

class GyroscopeV2: public BaseDevice {
private:

	static const uint8_t BASE_ADDRESS = 0x68;
	static constexpr float _rawdps = 8.75f / 1000.f;

	enum RegisterMap
	{
		GYRO_CONFIG = 0X11,
		GYRO_XOUT_H = 0X22,
		GYRO_XOUT_L,
		GYRO_YOUT_H,
		GYRO_YOUT_L,
		GYRO_ZOUT_H,
		GYRO_ZOUT_L,
	};


	uint8_t _sensitivity = 0;
	uint8_t _dataRate = 0;

public:
	enum DataRate
	{
		OFF,
		F_13_Hz,
		F_26_Hz,
		F_52_Hz,
		F_104_Hz,
		F_208_Hz,
		F_416_Hz,
		F_833_Hz,
		F_1666_Hz,
	};
	enum Scale
	{
		DPS0250 = 0,
		DPS0500,
		DPS1000,
		DPS2000
	};

#ifndef ARDUINO
	GyroscopeV2(I2C_HandleTypeDef *hi2c, uint8_t address = BASE_ADDRESS);
#else
	GyroscopeV2(TwoWire &hi2c, uint8_t address = BASE_ADDRESS);
	GyroscopeV2(uint8_t address = BASE_ADDRESS);
#endif

	GyroscopeV2(const GyroscopeV2 &other);
	GyroscopeV2(GyroscopeV2 &&other);
	GyroscopeV2& operator=(const GyroscopeV2 &other);
	GyroscopeV2& operator=(GyroscopeV2 &&other);

	uint8_t Init();
	uint8_t Init(Scale sensitivity);
	uint8_t Init(Scale sensitivity, DataRate dataRate);

	void SetScale(Scale sensitivity);
	void SetDataRate(DataRate dataRate);

	int16_t RawX();
	int16_t RawY();
	int16_t RawZ();

	float X();
	float Y();
	float Z();

	virtual ~GyroscopeV2();
};

} /* namespace IntroSatLib */

#endif /* GYROSCOPE_H_ */
