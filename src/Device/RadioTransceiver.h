/*
 * RadioTransmitter.h
 *
 *  Created on: Mar 11, 2025
 *      Author: User
 */

#ifndef RADIOTRANSCEIVER_H_
#define RADIOTRANSCEIVER_H_

#include "./Device.h"

namespace IntroSatLib {

class RadioTransceiver: public Device {
public:
	virtual void Send(uint8_t* data, uint16_t len);
};
}



#endif /* RADIOTRANSMITTER_H_ */
