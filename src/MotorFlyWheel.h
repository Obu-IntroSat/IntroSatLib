#ifndef MOTORFLYWHEEL_H_
#define MOTORFLYWHEEL_H_

#include "BaseFlyWheel.h"

namespace IntroSatLib {

class MotorFlyWheel: public BaseFlyWheel {
public:
	/**
	 * @note Только в STM32CubeIDE
	 * @brief Создание объекта маховика. 
	 * 
	 * @param hi2c объект @b I2C_HandleTypeDef
	 * @param address адрес маховика на шине I2C
	 */
	MotorFlyWheel(const interfaces::I2C &i2c, uint8_t address = BASE_ADDRESS);

	/**
	 * @brief Создание объекта маховика как копии другого объекта маховика
	 * 
	 * @param other исходный объект для копирования
	 */
	MotorFlyWheel(const MotorFlyWheel &other);
	MotorFlyWheel(MotorFlyWheel &&other);
	MotorFlyWheel& operator=(const MotorFlyWheel &other);
	MotorFlyWheel& operator=(MotorFlyWheel &&other);
	~MotorFlyWheel() override;
};

} /* namespace IntroSatLib */

#endif /* MOTORFLYWHEEL_H_ */
