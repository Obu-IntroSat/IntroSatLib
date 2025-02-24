#ifndef GYROSCOPE_V2_H_
#define GYROSCOPE_V2_H_

#include "I2CDevice.h"
#include "BaseDevice.h"

#ifndef ARDUINO
#include "Quaternion/Quaternion.h"
#endif

namespace IntroSatLib {

class GyroscopeV2: public BaseDevice {
private:

	static const uint8_t BASE_ADDRESS = 0x6B;
	static constexpr float _rawdps = (8.75f / 1000.f) * 3.1415926f / 180.0f;

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

	static float cutMin(float value, float cut);

	uint8_t _sensitivity = 0;
	uint8_t _dataRate = 0;

	uint32_t _lastXTime = 0;
	uint32_t _lastYTime = 0;
	uint32_t _lastZTime = 0;

	float _cutX = 0.0872665;
	float _cutY = 0.0872665;
	float _cutZ = 0.0872665;

	float _lastX = 0;
	float _lastY = 0;
	float _lastZ = 0;

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

	uint8_t Init() override;
	uint8_t Init(Scale sensitivity);
	uint8_t Init(Scale sensitivity, DataRate dataRate);

	void SetScale(Scale sensitivity);
	void SetDataRate(DataRate dataRate);
public:
	int16_t RawX();
	int16_t RawY();
	int16_t RawZ();

	float X();
	float Y();
	float Z();

	float integrationX();
	float integrationY();
	float integrationZ();

	void SetMinCutX(float x);
	void SetMinCutY(float y);
	void SetMinCutZ(float z);
#ifndef ARDUINO
	Quaternion<float> GetQuaternion();
#endif
	~GyroscopeV2() override;
};

} /* namespace IntroSatLib */

#endif /* GYROSCOPE_V2_H_ */
