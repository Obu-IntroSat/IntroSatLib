#ifndef MOTORFLYWHEEL_H_
#define MOTORFLYWHEEL_H_

#include "BaseFlyWheel.h"

namespace IntroSatLib {

class MotorFlyWheel: public BaseFlyWheel {
public:
#ifndef ARDUINO
	MotorFlyWheel(I2C_HandleTypeDef *hi2c, uint8_t address = BASE_ADDRESS);
#else
	MotorFlyWheel(TwoWire &hi2c, uint8_t address = BASE_ADDRESS);
	MotorFlyWheel(uint8_t address = BASE_ADDRESS);
#endif

	MotorFlyWheel(const MotorFlyWheel &other);
	MotorFlyWheel(MotorFlyWheel &&other);
	MotorFlyWheel& operator=(const MotorFlyWheel &other);
	MotorFlyWheel& operator=(MotorFlyWheel &&other);
	~MotorFlyWheel() override;
};

} /* namespace IntroSatLib */

#endif /* MOTORFLYWHEEL_H_ */
