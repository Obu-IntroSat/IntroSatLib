/*
 * Barometer.h
 *
 *  Created on: Mar 7, 2025
 *      Author: unflesh
 */

#ifndef BASE_BAROMETER_H_
#define BASE_BAROMETER_H_

#include "Device/Device.h"

namespace IntroSatLib {

class BaseBarometer {
	float pressure;
public:
	virtual float GetPressure() = 0;
};

} /* namespace IntroStratLib */


#endif /* BASE_BAROMETER_H_ */
