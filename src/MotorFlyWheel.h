/*
 * MotorFlyWheel.h
 *
 *  Created on: 15 окт. 2022 г.
 *      Author: Almaz
 */

#ifndef MOTORFLYWHEEL_H_
#define MOTORFLYWHEEL_H_

#include <BaseFlyWheel.h>

namespace IntroSatLib {

class MotorFlyWheel: public BaseFlyWheel {
protected:
	uint8_t _channel = 1;
};

} /* namespace IntroSatLib */

#endif /* MOTORFLYWHEEL_H_ */
