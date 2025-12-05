#ifndef LIGHTSENSOR_H_
#define LIGHTSENSOR_H_

#include "Device/I2CDevice.h"
//#include "../BaseDevice.h"

namespace IntroSatLib {

class LightSensor: private I2CDevice {
private:

	static const uint8_t BASE_ADDRESS = 0x50;

public:
//#ifndef ARDUINO
	/**
	 * @brief Создание объекта датчика освещённости
	 * @note Только в STM32CubeIDE
	 * 
	 * @param hi2c объект @b I2C_HandleTypeDef
	 * @param address адрес датчика освещённости на шине I2C
	 */
	LightSensor(const interfaces::I2C &i2c, uint8_t address = BASE_ADDRESS);
//#else
//	/**
//	 * @brief Создание объекта датчика освещённости
//	 * @note Только в Arduino IDE
//	 *
//	 * @param hi2c объект @b TwoWire или @b Wire
//	 * @param address адрес датчика освещённости на шине I2C
//	 */
//	LightSensor(TwoWire &hi2c, uint8_t address = BASE_ADDRESS);
//
//	/**
//	 * @brief Создание объекта датчика освещённости на @b I2C1
//	 * @note Только в Arduino IDE
//	 *
//	 * @param address адрес датчика освещённости на шине I2C
//	 */
//	LightSensor(uint8_t address = BASE_ADDRESS);
//#endif
	/**
	 * @brief Создание объекта датчика освещённости как копии другого объекта датчика освещённости
	 * 
	 * @param other исходный объект для копирования
	 */
	LightSensor(const LightSensor &other);
	LightSensor(LightSensor &&other);
	LightSensor& operator=(const LightSensor &other);
	LightSensor& operator=(LightSensor &&other);

	/**
	 * @brief Инициализация датчика освещённости
	 * 
	 * @returns 0, если инициализация прошла успешно
	 */
	ISL_StatusTypeDef Init() override;

	/**
	 * @brief Получение значения освещённости
	 * 
	 * @return Значение освещённости в условных единицах, где 0 - максимальная освещённость, 2048 - минимальная освещённость
	 */
	int16_t GetLight();
	~LightSensor() override;
};

} /* namespace IntroSatLib */

#endif /* LIGHTSENSOR_H_ */
