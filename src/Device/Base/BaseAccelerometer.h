/*
 * Accelerometer.h
 *
 *  Created on: Mar 18, 2025
 *      Author: unflesh
 */

#ifndef BASE_ACCELEROMETER_H_
#define BASE_ACCELEROMETER_H_

#include "Device/Device.h"

namespace IntroSatLib {

class BaseAccelerometer {

public:
	virtual int16_t RawAX() = 0;
	virtual int16_t RawAY() = 0;
	virtual int16_t RawAZ() = 0;

	virtual float AX() = 0;
	virtual float AY() = 0;
	virtual float AZ() = 0;
};

} /* namespace IntroStratLib */


#endif /* BASE_ACCELEROMETER_H_ */
