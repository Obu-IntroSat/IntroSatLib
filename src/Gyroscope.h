/*
 * Gyroscope.h
 *
 *  Created on: 13 июл. 2022 г.
 *      Author: Almaz
 */

#ifndef GYROSCOPE_H_
#define GYROSCOPE_H_

#include "I2CDevice.h"
#include "BaseDevice.h"

namespace IntroSatLib {

class Gyroscope: public BaseDevice {
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

#ifndef ARDUINO
	Gyroscope(I2C_HandleTypeDef *hi2c, uint8_t address = BASE_ADDRESS);
#else
	Gyroscope(TwoWire &hi2c, uint8_t address = BASE_ADDRESS);
#endif

	Gyroscope(const Gyroscope &other);
	Gyroscope(Gyroscope &&other);
	Gyroscope& operator=(const Gyroscope &other);
	Gyroscope& operator=(Gyroscope &&other);

	uint8_t Init();
	uint8_t Init(Scale sensitivity);
	uint8_t Init(Scale sensitivity, FilterBandwidth filter);

	void SetScale(Scale sensitivity);
	void SetFilter(FilterBandwidth filter);

	int16_t RawX();
	int16_t RawY();
	int16_t RawZ();

	float X();
	float Y();
	float Z();

	virtual ~Gyroscope();
};

} /* namespace IntroSatLib */

#endif /* GYROSCOPE_H_ */
