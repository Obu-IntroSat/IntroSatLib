/*
 * Gyroscope.h
 *
 *  Created on: Mar 17, 2025
 *      Author: unflesh
 */

#ifndef BASE_GYROSCOPE_H_
#define BASE_GYROSCOPE_H_

#include "Device/Device.h"

namespace IntroSatLib {

class BaseGyroscope {

public:
	virtual int16_t RawGX() = 0;
	virtual int16_t RawGY() = 0;
	virtual int16_t RawGZ() = 0;

	virtual float GX() = 0;
	virtual float GY() = 0;
	virtual float GZ() = 0;
};

} /* namespace IntroStratLib */


#endif /* BASE_GYROSCOPE_H_ */
