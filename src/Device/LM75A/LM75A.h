/*
 * LM75A.h
 *
 *  Created on: Mar 4, 2025
 *      Author: Aleksey <TeaCupMe> Gilenko
 */

#ifndef LM75A_H_
#define LM75A_H_
#include "Device/Base/BaseTemperatureSensor.h"
#include "Device/I2CDevice.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"
//#include <functional>


namespace IntroSatLib {



class LM75A: public BaseTemperatureSensor, protected I2CDevice {
private:
	enum RegisterMap: uint8_t {
			TEMPERATURE = 0x00,
			CONFIGURATION = 0x01,
			T_HYST = 0x02,
			T_OS = 0x03
		};

	static const uint8_t BASE_ADDRESS = 0x48;
//	void setRegisterPointer(RegisterMap reg);

public:

	LM75A(const interfaces::I2C &i2c, uint8_t address = BASE_ADDRESS);

//	LM75A(const LM75A &other);
//	LM75A(LM75A &&other);
//	LM75A& operator=(const LM75A &other);
//	LM75A& operator=(LM75A &&other);

	ISL_StatusTypeDef Init();

	int16_t GetRawTemperature(); // TODO: make private
	int16_t GetTemperatureTimes8();

	float GetTemperature() override;
	uint8_t GetConfig();

	void PowerDown(bool shutdown);

};


}




#endif /* LM75A_H_ */
