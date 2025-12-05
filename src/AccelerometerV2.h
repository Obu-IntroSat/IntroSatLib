#ifndef ACCELEROMETERV2_H_
#define ACCELEROMETERV2_H_

#include "Device/LSM6DS3/LSM6DS3.h"
//#include "../BaseDevice.h"

namespace IntroSatLib {

class AccelerometerV2: private LSM6DS3 {
private:
	static const uint8_t BASE_ADDRESS = 0x6B;
public:
	/**
	 * @brief Диапазон измерения ускорения
	 * 
	 */
	enum Scale
	{
		twoG = 0,  		/**< Диапазон &plusmn;  2g */
		fourG = 2, 		/**< Диапазон &plusmn;  4g */
		eightG = 3,		/**< Диапазон &plusmn;  8g */
		sixteenG = 1 	/**< Диапазон &plusmn;  16g */
	};

	/**
	 * @brief Частота внутреннего фильтра измерения ускорения
	 * 
	 */
	enum FilterBandwidth
	{
		F400H = 0, /**< Частота 400 Гц */
		F200H = 1, /**< Частота 200 Гц */
		F100H = 2, /**< Частота 100 Гц */
		F050H = 3  /**< Частота 50 Гц */
	};


	/**
	 * @par <!-- @irongamer54 , что это на самом деле-->
	 * @brief Скорость обновления данных
	 * 
	 */

	// TODO @TeaCupMe я бы привёл к стилю, что всё в герцах - DR1K66H, DR3K33H и так далее. Спорный моментик
	enum DataRate
	{
		DR12_5H = 1,      /**< 12.5 Гц */
		DR26H = 2,        /**< 26 Гц */
		DR52H = 3,        /**< 52 Гц */
		DR104H = 4,       /**< 104 Гц */
		DR208H = 5,       /**< 208 Гц */
		DR416H = 6,       /**< 416 Гц */
		DR833H = 7,       /**< 833 Гц */
		DR1_66KH = 8,     /**< 1.66 кГц */
		DR3_33KH = 9,     /**< 3.33 кГц */
		DR6_66KH = 10,    /**< 6.66 кГц */
    };

    uint8_t _scale = 0;

	/**
	 * @brief Конструктор объекта акселлерометра. 
	 * @note Только в STM32CubeIDE
	 * 
	 * @param i2c объект(или указатель на объект) @b I2C_HandleTypeDef или @b TwoWire
	 * @param address адрес акселлерометра на шине I2C
	 */
    AccelerometerV2(const interfaces::I2C &i2c, uint8_t address = BASE_ADDRESS): LSM6DS3(i2c, address) {};

	/**
	 * @brief Инициализация акселлерометра с параметрами по умолчанию: @ref Scale::fourG, @ref FilterBandwidth::F400H, @ref DataRate::DR6_66KH.
	 * 
	 * @returns 0, если инициализация завершена успешно
	 * @returns 1, если при инициализации возникла ошибка
	 */
    ISL_StatusTypeDef Init() {
    	return LSM6DS3::InitAccel();
    }

	/**
	 * @brief Инициализация акселлерометра с заданным диапазоном измерения
	 * 
	 * @param sens Значение чуствительности @ref Scale
	 * @returns 0, если инициализация завершена успешно
	 * @returns 1, если при инициализации возникла ошибка
	 */
    ISL_StatusTypeDef Init(Scale scale) {
    	return LSM6DS3::InitAccel((LSM6DS3::ScaleAccel) scale);
    }

	/**
	 * @brief Инициализация акселлерометра с заданными диапазоном измерения и шириной окна фильтра 
	 * 
	 * @param scale Значение диапазона измерения @ref Scale
	 * @param dataRate Значение скорости обновления данных @ref FilterBandwidth 
	 * @returns 0, если инициализация завершена успешно
	 * @returns 1, если при инициализации возникла ошибка
	 */
    ISL_StatusTypeDef Init(Scale scale, DataRate dataRate) {
   		return LSM6DS3::InitAccel((LSM6DS3::ScaleAccel) scale, (LSM6DS3::DataRateAccel) dataRate);
    }

	/**
	 * @brief Инициализация акселлерометра с заданными диапазоном измерения, шириной окна фильтра и скоростью обновления данных 
	 * 
	 * @param scale Значение диапазона измерения @ref Scale
	 * @param filter Значение ширины фильтра @ref FilterBandwidth 
	 * @param datarate Значение скорости обновления данных @ref DataRate
	 * @returns 0, если инициализация завершена успешно
	 * @returns 1, если при инициализации возникла ошибка
	 */
    ISL_StatusTypeDef Init(Scale scale, DataRate dataRate, FilterBandwidth filter) {
		return LSM6DS3::InitAccel((LSM6DS3::ScaleAccel) scale, (LSM6DS3::DataRateAccel) dataRate, (LSM6DS3::FilterBandwidthAccel) filter);
    }

	/**
	 * @brief Установка диапазона измерения
	 * 
	 * @param scale Значение чуствительности @ref Scale
	 */
    ISL_StatusTypeDef SetScale(Scale scale = twoG) {
    	return LSM6DS3::SetScaleAccel((LSM6DS3::ScaleAccel) scale);
    }

	/**
	 * @brief Установка ширины окна фильтра
	 * 
	 * @param filter Значение ширины фильтра @ref FilterBandwidth 
	 */
    ISL_StatusTypeDef SetFilter(FilterBandwidth filter) {
    	return LSM6DS3::SetFilterAccel((LSM6DS3::FilterBandwidthAccel) filter);
    }

	/**
	 * @brief Установка скорости обновления данных
	 * 
	 * @param datarate datarate Значение скорости обновления данных @ref DataRate
	 */
    ISL_StatusTypeDef SetDataRate(DataRate datarate) {
    	return LSM6DS3::SetDataRateAccel((LSM6DS3::DataRateAccel) datarate);
    }

	/**
	 * @brief Отключение акселлерометра - переход в режим экономии энергии
	 * 
	 */
    ISL_StatusTypeDef end() {
    	return LSM6DS3::DeinitAccel();
    }

	/**
	 * @brief Получение @b необработанного значения ускорения по оси X.
	 * 
	 * @return Ускорение по оси в условных единицах
	 */
    int16_t RawX() {
    	return LSM6DS3::RawAX();
    }

	/**
	 * @brief Получение @b необработанного значения ускорения по оси Y. 
	 * 
	 * @return Ускорение по оси в условных единицах
	 */
    int16_t RawY() {
    	return LSM6DS3::RawAY();
    }
    
	/**
	 * @brief Получение @b необработанного значения ускорения по оси Z. 
	 * 
	 * @return Ускорение по оси в условных единицах
	 */
	int16_t RawZ() {
		return LSM6DS3::RawAZ();
	}

    /**
	 * @brief Получение значения ускорения по оси X.
	 * 
	 * @returns Ускорение по оси в м/с<sup>2</sup>
	 */
	float X() {
		return LSM6DS3::AX();
	}
    
	/**
	 * @brief Получение значения ускорения по оси Y.
	 * 
	 * @returns Ускорение по оси в м/с<sup>2</sup>
	 */
	float Y() {
		return LSM6DS3::AY();
	}
    
	/**
	 * @brief Получение значения ускорения по оси Z.
	 * 
	 * @returns Ускорение по оси в м/с<sup>2</sup>
	 */
	float Z() {
		return LSM6DS3::AZ();
	}

	/**
	 * @brief Получение значения температуры акселлерометра
	 * 
	 * @return Значение температуры в &deg;C
	 */
    float Temp() {
    	return LSM6DS3::Temp();
    }

    ~AccelerometerV2() {};
};

} /* namespace IntroSatLib */

#endif /* ACCELEROMETER_H_ */
