/*
 * LM75A.cpp
 *
 *  Created on: Mar 6, 2025
 *      Author: Aleksey <TeaCupMe> Gilenko
 */

#include "LM75A.h"

namespace IntroSatLib {

ISL_StatusTypeDef LM75A::Init() {
	return IsReady();
}

void LM75A::PowerDown(bool shutdown) {
	UNUSED(shutdown);
	//TODO implement
}

LM75A::LM75A(const interfaces::I2C &i2c, uint8_t address) :
		I2CDevice(new interfaces::I2C(i2c), address) {
}

int16_t LM75A::GetRawTemperature() {
	uint8_t raw[2];
	ReadRegisterI2C(RegisterMap::TEMPERATURE, raw, 2);
	int16_t data = raw[0] << 8 | raw[1];
	return data;
}

float LM75A::GetTemperature() {
	return (GetRawTemperature()) / 256.0;
}

int16_t LM75A::GetTemperatureTimes8() {
	return (GetRawTemperature()) / 32;
}

uint8_t LM75A::GetConfig() {
	uint8_t data = 0;
	ReadRegisterI2C(RegisterMap::CONFIGURATION, &data, 1);
	return data;
}

}
