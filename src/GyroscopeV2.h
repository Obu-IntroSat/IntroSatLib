#ifndef GYROSCOPE_V2_H_
#define GYROSCOPE_V2_H_

//#include "Device/I2CDevice.h"
//#include "Adapter/System.h"
//#include "../BaseDevice.h"

#include "Device/LSM6DS3/LSM6DS3.h"
#include "Adapter/System.h"
#include <stdint.h>

#ifndef ARDUINO
#include "Quaternion/Quaternion.h"
#endif

namespace IntroSatLib {

class GyroscopeV2: private LSM6DS3 {
private:

	static const uint8_t BASE_ADDRESS = 0x6B;

	uint32_t _lastXTime = 0;
	uint32_t _lastYTime = 0;
	uint32_t _lastZTime = 0;
	float _lastX = 0;
	float _lastY = 0;
	float _lastZ = 0;

public:

	/**
	 * @brief Скорость обновления данных
	 * 
	 */
	enum DataRate
	{
		OFF,		/**< Автоматическое определение */
		F_13_Hz,	/**< 13 Гц */
		F_26_Hz,	/**< 26 Гц */
		F_52_Hz,	/**< 52 Гц */
		F_104_Hz,	/**< 104 Гц */
		F_208_Hz,	/**< 208 Гц */
		F_416_Hz,	/**< 416 Гц */
		F_833_Hz,	/**< 833 Гц */
		F_1666_Hz,	/**< 1666 Гц */
	};

	/**
	 * @brief Диапазон измерения
	 * 
	 */
	enum Scale
	{
		DPS0250 = 0,	/**< Диапазон &plusmn;250 град/с */
		DPS0500,		/**< Диапазон &plusmn;500 град/с */
		DPS1000,		/**< Диапазон &plusmn;1000 град/с */
		DPS2000			/**< Диапазон &plusmn;2000 град/с */
	};

	/**
	 * @note Только в STM32CubeIDE
	 * @brief Конструктор объекта гироскопа. 
	 * 
	 * @param hi2c объект @b I2C_HandleTypeDef
	 * @param address адрес гироскопа на шине I2C
	 */
	GyroscopeV2(const interfaces::I2C &i2c, uint8_t address = BASE_ADDRESS): LSM6DS3(i2c, address) {};

	/**
	 * @brief Конструктор объекта гироскопа как копии другого объекта гироскопа
	 * 
	 * @param other исходный объект для копирования
	 */
//	GyroscopeV2(const GyroscopeV2 &other);
//	GyroscopeV2(GyroscopeV2 &&other);
//	GyroscopeV2& operator=(const GyroscopeV2 &other);
//	GyroscopeV2& operator=(GyroscopeV2 &&other);

	/**
	 * @brief Инициализация гироскопа с параметрами по умолчанию: @ref DataRate::F_104_Hz, @ref Scale::DPS0250
	 * 
	 * @returns 0, если инициализация завершена успешно
	 * @returns 1, если при инициализации возникла ошибка 
	 */
	ISL_StatusTypeDef Init() {
		return Init(Scale::DPS0250);
	}

	/**
	 * @brief Инициализация гироскопа с заданным диапазоном измерения
	 * 
	 * @param sensitivity Значение чуствительности @ref Scale
	 * @returns 0, если инициализация завершена успешно
	 * @returns 1, если при инициализации возникла ошибка
	 */
	ISL_StatusTypeDef Init(Scale scale) {
		return Init(scale, DataRate::F_416_Hz);
	}

	/**
	 * @brief Инициализация гироскопа с заданным диапазоном измерения
	 * 
	 * @param sensitivity Значение чуствительности @ref Scale
	 * @param datarate Значение скорости обновления данных @ref DataRate
	 * @returns 0, если инициализация завершена успешно
	 * @returns 1, если при инициализации возникла ошибка
	 */
	ISL_StatusTypeDef Init(Scale scale, DataRate dataRate) {
		_lastXTime = system::GetTick();
		_lastYTime = _lastXTime;
		_lastZTime = _lastXTime;
		return LSM6DS3::InitGyro((LSM6DS3::ScaleGyro) scale, (LSM6DS3::DataRateGyro) dataRate);
	}

	/**
	 * @brief Установка диапазона измерения
	 * 
	 * @param sensitivity Значение чуствительности @ref Scale
	 */
	ISL_StatusTypeDef SetScale(Scale scale) {
		return LSM6DS3::SetScaleGyro((LSM6DS3::ScaleGyro) scale);
	}

	/**
	* @brief Установка скорости обновления данных
	* 
	* @param datarate Значение скорости обновления данных @ref DataRate
	*/
	ISL_StatusTypeDef SetDataRate(DataRate dataRate) {
		return LSM6DS3::SetDataRateGyro((LSM6DS3::DataRateGyro) dataRate);
	}

	/**
	 * @brief Получение @b необработанного значения ускорения по оси X.
	 * 
	 * @return Ускорение по оси в условных единицах
	 */
	int16_t RawX() {
		return LSM6DS3::RawGX();
	}

	/**
	 * @brief Получение @b необработанного значения ускорения по оси Y. 
	 * 
	 * @return Ускорение по оси в условных единицах
	 */
	int16_t RawY() {
		return LSM6DS3::RawGY();
	}

	/**
	 * @brief Получение @b необработанного значения ускорения по оси Z. 
	 * 
	 * @return Ускорение по оси в условных единицах
	 */
	int16_t RawZ() {
		return LSM6DS3::RawGZ();
	}

	/**
	 * @brief Получение значения ускорения вокруг оси X.
	 * 
	 * @returns Ускорение по оси в м/с<sup>2</sup>
	 */	
	float X() {
		return LSM6DS3::GX();
	}

	/**
	 * @brief Получение значения ускорения вокруг оси Y.
	 * 
	 * @returns Ускорение по оси в м/с<sup>2</sup>
	 */
	float Y() {
		return LSM6DS3::GY();
	}

	/**
	 * @brief Получение значения ускорения вокруг оси Z.
	 * 
	 * @returns Ускорение по оси в м/с<sup>2</sup>
	 */	
	float Z() {
		return LSM6DS3::GZ();
	}

	/**
	 * @brief Получение углового перемещения вокруг оси X с момента предыдущего вызова @ref integrationX()
	 * @note Метод учитывает только два измерения угловой скорости: 
	 * одно, сделанное в момент вызова этого метода, и второе - предыдущее. Если время между измерениями 
	 * велико и угловая скорость успела нелинейно измениться - результат будет некорректным. 
	 * 
	 * @return Угловое перемещение в градусах.
	 */
	float integrationX() {
	   float speed = X();
	   uint32_t time = system::GetTick();
	   uint32_t deltaTime = time - _lastXTime;
	   float value = (_lastX + speed) * (deltaTime >> 1) * 0.001;
	   _lastX = speed;
	   _lastXTime = time;
	   return value;
	}
	
	/**
	 * @brief Получение углового перемещения вокруг оси Y с момента предыдущего вызова @ref integrationY()
	 * @note Метод учитывает только два измерения угловой скорости: 
	 * одно, сделанное в момент вызова этого метода, и второе - предыдущее. Если время между измерениями 
	 * велико и угловая скорость успела нелинейно измениться - результат будет некорректным. 
	 * 
	 * @return Угловое перемещение в градусах.
	 */
	float integrationY() {
		float speed = Y();
		uint32_t time = system::GetTick();
		uint32_t deltaTime = time - _lastYTime;
		float value = (_lastY + speed) * (deltaTime >> 1) * 0.001;
		_lastY = speed;
		_lastYTime = time;
		return value;
	}
	
	/**
	 * @brief Получение углового перемещения вокруг оси Z с момента предыдущего вызова @ref integrationZ()
	 * @note Метод учитывает только два измерения угловой скорости: 
	 * одно, сделанное в момент вызова этого метода, и второе - предыдущее. Если время между измерениями 
	 * велико и угловая скорость успела нелинейно измениться - результат будет некорректным. 
	 * 
	 * @return Угловое перемещение в градусах.
	 */
	float integrationZ() {
		float speed = Z();
		uint32_t time = system::GetTick();
		uint32_t deltaTime = time - _lastZTime;
		float value = (_lastZ + speed) * (deltaTime >> 1) * 0.001;
		_lastZ = speed;
		_lastZTime = time;
		return value;
	}

	/**
	 * @brief Установка порогового значения по оси X
	 * @note Если угловая скорость, полученная с датчика, меньше этого числа, метод @ref X() вернёт 0.
	 * @param x Пороговое значение 
	 */
	void SetMinCutX(float x) {
		LSM6DS3::SetMinCutX(x);
	}

	/**
	 * @brief Установка порогового значения по оси Y
	 * @note Если угловая скорость, полученная с датчика, меньше этого числа, метод @ref Y() вернёт 0.
	 * @param y Пороговое значение
	 */
	void SetMinCutY(float y) {
		LSM6DS3::SetMinCutX(y);
	}

	/**
	 * @brief Установка порогового значения по оси Z
	 * @note Если угловая скорость, полученная с датчика, меньше этого числа, метод @ref Z() вернёт 0.
	 * @param z Пороговое значение
	 */
	void SetMinCutZ(float z) {
		LSM6DS3::SetMinCutX(z);
	}
#ifndef ARDUINO
	// TODO @TeaCupMe Описать более понятным языком
	/**
	 * @brief Получение изменения углового положения в виде набора кватернионов
	 * @note Метод использует @ref IntegrationX(), @ref IntegrationY() и @ref IntegrationZ(). 
	 * В связи с этим, для получения достоверных данных необходимо вызывать достаточно часто.
	 * @remark Только в STM32CubeIDE
	 * @return Набор кваретнионов, описывающий изменение углового положения
	 */
	Quaternion<float> GetQuaternion();
#endif
	~GyroscopeV2() {};
};

} /* namespace IntroSatLib */

#endif /* GYROSCOPE_V2_H_ */
