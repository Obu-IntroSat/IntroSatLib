/*
 * CoilFlyWheel.h
 *
 *  Created on: 15 окт. 2022 г.
 *      Author: Almaz
 */

#ifndef COILFLYWHEEL_H_
#define COILFLYWHEEL_H_

#include <BaseFlyWheel.h>

namespace IntroSatLib {

class CoilFlyWheel: public BaseFlyWheel {
protected:
	uint8_t _channel = 2;
};

} /* namespace IntroSatLib */

#endif /* COILFLYWHEEL_H_ */
