/*
 * BaseDevice.h
 *
 *  Created on: Jul 12, 2022
 *      Author: Almaz
 */

#ifndef BASEDEVICE_H_
#define BASEDEVICE_H_

namespace IntroSatLib {

class BaseDevice {
public:
	virtual void Init() = 0;
};

} /* namespace IntroSatLib */

#endif /* BASEDEVICE_H_ */
