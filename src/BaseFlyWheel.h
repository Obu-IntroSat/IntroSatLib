#ifndef BASEFLYWHEEL_H_
#define BASEFLYWHEEL_H_

//#include "../BaseDevice.h"
#include "Device/I2CDevice.h"

namespace IntroSatLib {

class BaseFlyWheel: private I2CDevice {
private:

	enum RegisterMap
	{
		Config = 0,
		BaseAddress,
		Calibration,
		Clock_Setting,
		Need_F_ChannelSpeed = 4,
		Current_F_ChannelSpeed = 6,
		F_MaxAbsSpeed = 8,
		F_Min_Force = 10,
		F_P = 12,
		F_I = 16,
		F_D = 20,
		Need_S_ChannelSpeed = 24,
		Current_S_ChannelSpeed = 26,
		S_MaxAbsSpeed = 28,
		S_Min_Force = 30,
		S_P = 32,
		S_I = 36,
		S_D = 40,
		Reserve1 = 44,
		Reserve2 = 45,
		Version = 46,
		WriteToMemory = 47,
	};
protected:
	uint8_t _channel = 0;
	uint8_t _version = 0;
	static const uint8_t BASE_ADDRESS = 0x38;
	uint8_t CheckVersion();
public:
	/**
	 * @brief Создание объекта маховика. 
	 * @note Только в STM32CubeIDE
	 * @param hi2c объект @b I2C_HandleTypeDef
	 * @param address адрес маховика на шине I2C
	 */
	BaseFlyWheel(const interfaces::I2C &i2c, uint8_t address = BASE_ADDRESS);

	/**
	 * @brief Инициализация маховика
	 * 
	 * @returns 0, если инициализация завершена успешно
	 * @returns 1, если при инициализации возникла ошибка
	 */
	ISL_StatusTypeDef Init() override;

	// TODO @Goldfor Надо описать что все эти методы делают
	ISL_StatusTypeDef DirectMode(uint8_t directMode);
	uint8_t DirectMode();
	ISL_StatusTypeDef SilentMode(uint8_t silentMode);
	uint8_t SilentMode();
	ISL_StatusTypeDef MinForceMode(uint8_t minForceMode);
	uint8_t MinForceMode();
	ISL_StatusTypeDef ReverseMode(uint8_t reverceMode);
	uint8_t ReverseMode();

	/**
	 * @brief Установка скорости
	 * 
	 * @param needSpeed Жедаемая скорость в об/с
	 */
	ISL_StatusTypeDef NeedSpeed(int16_t needSpeed);

	/**
	 * @brief Чтение установленной скорости
	 * 
	 * @return Скорость в об/с
	 */
	int16_t NeedSpeed();

	/**
	 * @brief Чтение реальной скорости
	 * 
	 * @return Скорость в об/с
	 */
	int16_t CurrentSpeed();
	ISL_StatusTypeDef MaxAbsSpeed(uint16_t maxAbsSpeed);
	uint16_t MaxAbsSpeed();
	ISL_StatusTypeDef MinForce(uint16_t minForce);
	uint16_t MinForce();
	ISL_StatusTypeDef PID_P(float p);
	float PID_P();
	ISL_StatusTypeDef PID_I(float i);
	float PID_I();
	ISL_StatusTypeDef PID_D(float d);
	float PID_D();

	/**
	 * @brief Создание объекта маховика как копии другого объекта маховика
	 * 
	 * @param other исходный объект для копирования
	 */
	BaseFlyWheel(const BaseFlyWheel &other);
	BaseFlyWheel(BaseFlyWheel &&other);
	BaseFlyWheel& operator=(const BaseFlyWheel &other);
	BaseFlyWheel& operator=(BaseFlyWheel &&other);
	~BaseFlyWheel() override;
};

} /* namespace IntroSatLib */

#endif /* BASEFLYWHEEL_H_ */
