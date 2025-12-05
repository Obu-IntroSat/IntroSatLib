#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#include "Device/I2CDevice.h"
//#include "../BaseDevice.h"

namespace IntroSatLib {


class Accelerometer: protected I2CDevice {

private:
	// @Goldfor может вынести в protected? Чтобы с точки зрения документации было логичнее
	static const uint8_t BASE_ADDRESS = 0x68;
	static constexpr float _rawg = 16384.0;

	uint8_t _sensitivity = 0;

	enum RegisterMap
		{
			ACCEL_CONFIG = 0x1C,
			ACCEL_CONFIG_2,
			ACCEL_XOUT_H = 0x3B,
			ACCEL_XOUT_L,
			ACCEL_YOUT_H,
			ACCEL_YOUT_L,
			ACCEL_ZOUT_H,
			ACCEL_ZOUT_L,
		};
protected:
public:
	/**
	 * @brief Диапазон измерения ускорения
	 * 
	 */
	enum Scale
	{
		twoG = 0, 	/**< Диапазон &plusmn;  2g */
		fourG, 		/**< Диапазон &plusmn;  4g */
		eightG, 	/**< Диапазон &plusmn;  8g */
		sixteenG 	/**< Диапазон &plusmn;  16g */
	};

	// TODO @Goldfor это же ширина окна?
	/**
	 * @brief Ширина окна фильтра
	 * 
	 */
	enum FilterBandwidth
	{
		F0218 = 0,	/**< 218 измерений */
		F1046 = 8, 	/**< 1046 измерений */
		F0218b,		/**< 218 измерений */ // ??
		F0099,		/**< 99 измерений */
		F0045,		/**< 45 измерений */
		F0021,		/**< 21 измерений */
		F0010,		/**< 10 измерений */
		F0005,		/**< 5 измерений */
		F0420		/**< 420 измерений */
	};

	/**
	 * @brief Конструктор объекта акселлерометра. 
	 * @note Только в STM32CubeIDE
	 * @param hi2c объект @b I2C_HandleTypeDef
	 * @param address адрес акселлерометра на шине I2C
	 */
	Accelerometer(const interfaces::I2C &i2c, uint8_t address = BASE_ADDRESS);

	/**
	 * @brief Конструктор объекта акселлерометра как копии другого объекта акселлерометра
	 * 
	 * @param other исходный объект для копирования
	 */
	Accelerometer(const Accelerometer& other);
	Accelerometer& operator=(const Accelerometer& other);

	// @Goldfor Не знаю, как понятным детям языком описать отличие создания из rvalue (и не уверен, что нам это когда-нибудь понадобится).
	Accelerometer(Accelerometer&& other);
	Accelerometer& operator=(Accelerometer&& other);
	
	/**
	 * @brief Инициализация акселлерометра с параметрами по умолчанию: @ref Scale::twoG, @ref FilterBandwidth::F0021.
	 * 
	 * @return 0, когда инизиализация завершена
	 */
	ISL_StatusTypeDef Init() override;

	/**
	 * @brief Инициализация акселлерометра с заданным диапазоном измерения
	 * 
	 * @param sensitivity Значение чуствительности @ref Scale
	 * @return 0, когда инизиализация завершена
	 */
	//TODO Переименовать sensitivity в scale, иначе вызывает путаницу. Это именно scale
	ISL_StatusTypeDef Init(Scale sensitivity);

	/**
	 * @brief Инициализация акселлерометра с заданными диапазоном измерения и шириной окна фильтра 
	 * 
	 * @param sensitivity Значение чуствительности @ref Scale
	 * @param filter Значение ширины фильтра @ref FilterBandwidth 
	 * @return 0, когда инизиализация завершена
	 */
	//TODO Переименовать sensitivity в scale, иначе вызывает путаницу. Это именно scale
	ISL_StatusTypeDef Init(Scale sensitivity, FilterBandwidth filter);

	/**
	 * @brief Установка диапазона измерения
	 * 
	 * @param sensitivity Значение чуствительности @ref Scale
	 */
	//TODO Переименовать sensitivity в scale, иначе вызывает путаницу. Это именно scale
	ISL_StatusTypeDef SetScale(Scale sensitivity);
	
	/**
	 * @brief Установка фильтра измерений
	 * 
	 * @param filter Значение ширины фильтра @ref FilterBandwidth 
	 */
	ISL_StatusTypeDef SetFilter(FilterBandwidth filter);

	/**
	 * @brief Получение необработанного значения ускорения по оси X.
	 * 
	 * @return Ускорение по оси в условных единицах
	 */
	int16_t RawX();

	/**
	 * @brief Получение необработанного значения ускорения по оси Y. 
	 * 
	 * @return Ускорение по оси в условных единицах
	 */
	int16_t RawY();

	/**
	 * @brief Получение необработанного значения ускорения по оси Z. 
	 * 
	 * @return Ускорение по оси в условных единицах
	 */
	int16_t RawZ();

	/**
	 * @brief Получение значения ускорения по оси X.
	 * 
	 * @returns Ускорение по оси в м/с<sup>2</sup>
	 * 
	 */
	float X();

	/**
	 * @brief Получение значения ускорения по оси Y.
	 * 
	 * @returns Ускорение по оси в м/с<sup>2</sup>
	 * 
	 */
	float Y();
	
	/**
	 * @brief Получение значения ускорения по оси Z.
	 * 
	 * @returns Ускорение по оси в м/с<sup>2</sup>
	 * 
	 */
	float Z();

	~Accelerometer() override;
};

} /* namespace IntroSatLib */

#endif /* ACCELEROMETER_H_ */
