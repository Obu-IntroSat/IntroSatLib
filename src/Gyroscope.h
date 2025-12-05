#ifndef GYROSCOPE_H_
#define GYROSCOPE_H_

#include "Device/I2CDevice.h"
//#include "../BaseDevice.h"

namespace IntroSatLib {

class Gyroscope: private I2CDevice {
private:

	static const uint8_t BASE_ADDRESS = 0x68;
	static constexpr float _rawdps = 32768.0f / 250.0f;

	enum RegisterMap
	{
		CONFIG = 0x1A,
		GYRO_CONFIG,
		GYRO_XOUT_H = 0x43,
		GYRO_XOUT_L,
		GYRO_YOUT_H,
		GYRO_YOUT_L,
		GYRO_ZOUT_H,
		GYRO_ZOUT_L,
	};

	uint8_t _sensitivity = 0;

public:

	enum Scale
	{
		DPS0250 = 0,
		DPS0500,
		DPS1000,
		DPS2000
	};
	enum FilterBandwidth
	{
		F8800 = 8,
		F3600 = 16,
		F0250 = 0,
		F0184,
		F0092,
		F0041,
		F0020,
		F0010,
		F0005,
		F3600b
	};

	Gyroscope(const interfaces::I2C &i2c, uint8_t address = BASE_ADDRESS);

	Gyroscope(const Gyroscope &other);
	Gyroscope(Gyroscope &&other);
	Gyroscope& operator=(const Gyroscope &other);
	Gyroscope& operator=(Gyroscope &&other);

	ISL_StatusTypeDef Init() override;
	ISL_StatusTypeDef Init(Scale scale);
	ISL_StatusTypeDef Init(Scale scale, FilterBandwidth filter);

	ISL_StatusTypeDef SetScale(Scale scale);
	ISL_StatusTypeDef SetFilter(FilterBandwidth filter);

	int16_t RawX();
	int16_t RawY();
	int16_t RawZ();

	float X();
	float Y();
	float Z();

	~Gyroscope() override;
};

} /* namespace IntroSatLib */

#endif /* GYROSCOPE_H_ */
