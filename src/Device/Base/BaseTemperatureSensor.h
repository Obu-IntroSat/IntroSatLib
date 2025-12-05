/*
 * TemperatureSensor.h
 *
 *  Created on: Mar 4, 2025
 *      Author: Aleksey <TeaCupMe> Gilenko
 */

#ifndef BASE_TEMPERATURESENSOR_H_
#define BASE_TEMPERATURESENSOR_H_

#include "Device/Device.h"

namespace IntroSatLib {

class BaseTemperatureSensor {

private:

	float temperature;
public:

	virtual float GetTemperature() {
		return temperature;
	}
	virtual float GetTemperatureC() {
		return GetTemperature();
	}
	virtual float GetTemperatureF()
	{
	//	return GetTemperature() * 9 / 5 + 32; // TODO: Faster algorithm? Maybe optimize for LM75A using LM75A::GetTemperatureTimes8
		return GetTemperature() * 2 + 31;
	}
};
}




#endif /* BASE_TEMPERATURESENSOR_H_ */
