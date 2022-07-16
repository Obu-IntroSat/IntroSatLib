/*
 * Accelerometer.h
 *
 *  Created on: Jul 12, 2022
 *      Author: Almaz
 */

#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#include "I2CDevice.h"
#include "BaseDevice.h"

namespace IntroSatLib {

const float _rawg = 16384.0f;

enum RegisterMap
{
	SELF_TEST_X_ACCEL = 0x0D,
	SELF_TEST_Y_ACCEL,
	SELF_TEST_Z_ACCEL,
	ACCEL_CONFIG = 0x1C,
	ACCEL_CONFIG_2,
	ACCEL_XOUT_H = 0x3B,
	ACCEL_XOUT_L,
	ACCEL_YOUT_H,
	ACCEL_YOUT_L,
	ACCEL_ZOUT_H,
	ACCEL_ZOUT_L,
};
enum MaxAccelScale
{
	twoG = 0,
	fourG,
	eightG,
	sixteenG
};

enum Bandwidth
{
	F1130 = 0,
	F0460 = 8,
	F0184,
	F0092,
	F0041,
	F0020,
	F0010,
	F0005,
	F0460b
};

class Accelerometer : virtual public BaseDevice {
private:
	static const uint8_t BASE_ADDRES = 0x68;
	I2CDevice _i2c;
	MaxAccelScale _sensivity = MaxAccelScale::twoG;

	uint8_t GetRegister(RegisterMap reg);
	void SetRegister(RegisterMap reg, uint8_t value);

public:
#ifndef ARDUINO
	Accelerometer(I2C_HandleTypeDef *hi2c, uint8_t addres = BASE_ADDRES);
#else
	Accelerometer(TwoWire &hi2c, uint8_t addres = BASE_ADDRES);
#endif
	Accelerometer(const Accelerometer& other);
	Accelerometer& operator=(const Accelerometer& other);
	Accelerometer(Accelerometer&& other);
	Accelerometer& operator=(Accelerometer&& other);

	void Init();
	void Init(MaxAccelScale sensivity);
	void Init(MaxAccelScale sensivity, Bandwidth filter);

	void SetScale(MaxAccelScale sensivity);
	void SetFilter(Bandwidth filter);

	int16_t RawX();
	int16_t RawY();
	int16_t RawZ();

	float X();
	float Y();
	float Z();

	~Accelerometer();
};

} /* namespace IntroSatLib */

#endif /* ACCELEROMETER_H_ */
