#ifndef COILFLYWHEEL_H_
#define COILFLYWHEEL_H_

#include "BaseFlyWheel.h"

namespace IntroSatLib {

class CoilFlyWheel: public BaseFlyWheel {
public:
	/**
	 * @brief Конструктор объекта для управления магнитными катушками 
	 * @note Только в STM32CubeIDE
	 * 
	 * @param hi2c объект @b I2C_HandleTypeDef
	 * @param address адрес магнитных катушек на шине I2C
	 */
	CoilFlyWheel(const interfaces::I2C &i2c, uint8_t address = BASE_ADDRESS);

	/**
	 * @brief Конструктор объекта для управления магнитными катушками как копии другого объекта
	 * 
	 * @param other исходный объект для копирования
	 */
	CoilFlyWheel(const CoilFlyWheel &other);
	CoilFlyWheel(CoilFlyWheel &&other);
	CoilFlyWheel& operator=(const CoilFlyWheel &other);
	CoilFlyWheel& operator=(CoilFlyWheel &&other);
	~CoilFlyWheel() override;
};

} /* namespace IntroSatLib */

#endif /* COILFLYWHEEL_H_ */
