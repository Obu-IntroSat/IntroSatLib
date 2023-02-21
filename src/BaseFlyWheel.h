/*
 * BaseFlyWheel.h
 *
 *  Created on: 6 окт. 2022 г.
 *      Author: Almaz
 */

#ifndef BASEFLYWHEEL_H_
#define BASEFLYWHEEL_H_

#include "BaseDevice.h"

namespace IntroSatLib {

class BaseFlyWheel: public BaseDevice {
private:

	enum RegisterMap
	{
		Config = 0,
		BaseAddress,
		Calibration,
		Clock_Setting,
		Need_F_ChannelSpeed = 4,
		Current_F_ChannelSpeed = 6,
		F_MaxAbsSpeed = 8,
		F_Min_Force = 10,
		F_P = 12,
		F_I = 16,
		F_D = 20,
		Need_S_ChannelSpeed = 24,
		Current_S_ChannelSpeed = 26,
		S_MaxAbsSpeed = 28,
		S_Min_Force = 30,
		S_P = 32,
		S_I = 36,
		S_D = 40,
		Reserve1 = 44,
		Reserve2 = 45,
		Version = 46,
		WriteToMemory = 47,
	};
protected:
	uint8_t _channel = 0;
	uint8_t _version = 0;
	static const uint8_t BASE_ADDRESS = 0x38;
	uint8_t CheckVersion();
public:
#ifndef ARDUINO
	BaseFlyWheel(I2C_HandleTypeDef *hi2c, uint8_t address = BASE_ADDRESS);
#else
	BaseFlyWheel(TwoWire &hi2c, uint8_t address = BASE_ADDRESS);
#endif

	uint8_t Init();

	void DirectMode(uint8_t directMode);
	uint8_t DirectMode();
	void SilentMode(uint8_t silentMode);
	uint8_t SilentMode();
	void MinForceMode(uint8_t minForceMode);
	uint8_t MinForceMode();
	void ReverseMode(uint8_t reverceMode);
	uint8_t ReverseMode();
	void NeedSpeed(int16_t needSpeed);
	int16_t NeedSpeed();
	int16_t CurrentSpeed();
	void MaxAbsSpeed(uint16_t maxAbsSpeed);
	uint16_t MaxAbsSpeed();
	void MinForce(uint16_t minForce);
	uint16_t MinForce();
	void PID_P(float p);
	float PID_P();
	void PID_I(float i);
	float PID_I();
	void PID_D(float d);
	float PID_D();

	BaseFlyWheel(const BaseFlyWheel &other);
	BaseFlyWheel(BaseFlyWheel &&other);
	BaseFlyWheel& operator=(const BaseFlyWheel &other);
	BaseFlyWheel& operator=(BaseFlyWheel &&other);
	virtual ~BaseFlyWheel();
};

} /* namespace IntroSatLib */

#endif /* BASEFLYWHEEL_H_ */
