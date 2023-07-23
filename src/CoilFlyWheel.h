/*
 * CoilFlyWheel.h
 *
 *  Created on: 15 окт. 2022 г.
 *      Author: Almaz
 */

#ifndef COILFLYWHEEL_H_
#define COILFLYWHEEL_H_

#include "BaseFlyWheel.h"

namespace IntroSatLib
{

class CoilFlyWheel: public BaseFlyWheel
{
public:
#ifndef ARDUINO
	CoilFlyWheel(I2C_HandleTypeDef *hi2c, uint8_t address = BASE_ADDRESS);
#else
	CoilFlyWheel(TwoWire &hi2c, uint8_t address = BASE_ADDRESS);
#endif

	CoilFlyWheel(const CoilFlyWheel &other);
	CoilFlyWheel(CoilFlyWheel &&other);
	CoilFlyWheel& operator=(const CoilFlyWheel &other);
	CoilFlyWheel& operator=(CoilFlyWheel &&other);
	virtual ~CoilFlyWheel();
};

} /* namespace IntroSatLib */

#endif /* COILFLYWHEEL_H_ */
