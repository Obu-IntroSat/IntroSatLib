#ifndef MAGNETOMETER_V2_H_
#define MAGNETOMETER_V2_H_

//#include "Device/I2CDevice.h"
//#include "../BaseDevice.h"
#include "Device/LIS3MDL/LIS3MDL.h"
#ifndef ARDUINO
#include "Quaternion/Quaternion.h"
#include <cmath>
#include <array>
#endif

namespace IntroSatLib
{

	class MagnetometerV2 : private LIS3MDL
	{
	private:
		static const uint8_t BASE_ADDRESS = 0x1E;
		static const uint8_t LIS3MDL_WHO_AM_I = 0b00111101;

		uint16_t _x = 0;
		uint16_t _y = 0;
		uint16_t _z = 0;

	public:

	/**
	 * @brief Диапазон измерения
	 * 
	 */
		enum Scale 
		{
			G4 = 1, /**< Диапазон &plusmn;4 Гаусс */
			G8,		/**< Диапазон &plusmn;8 Гаусс */
			G12,	/**< Диапазон &plusmn;12 Гаусс */
			G16		/**< Диапазон &plusmn;16 Гаусс */
		};	


	/**
	 * @note Только в STM32CubeIDE
	 * @brief Создание объекта манитометра. 
	 * 
	 * @param hi2c объект @b I2C_HandleTypeDef
	 * @param address адрес манитометра на шине I2C
	 */
		MagnetometerV2(const interfaces::I2C &i2c, uint8_t address = BASE_ADDRESS): LIS3MDL(i2c, address) {};

		/**
		 * @brief Создание объекта манитометра как копии другого объекта манитометра
		 * 
		 * @param other исходный объект для копирования
		 */
//		MagnetometerV2(const MagnetometerV2 &other);
//		MagnetometerV2 &operator=(const MagnetometerV2 &other);
//		MagnetometerV2(MagnetometerV2 &&other);
//		MagnetometerV2 &operator=(MagnetometerV2 &&other);

		/**
		 * @brief Инициализация манитометра с параметрами по умолчанию: @ref Scale::G16
		 * 
		 * @returns 0, если инициализация завершена успешно
		 * @returns 1, если при инициализации возникла ошибка 
		 */
		ISL_StatusTypeDef Init() {
			return Init(Scale::G16);
		};

		/**
		 * @brief Инициализация манитометра с заданным диапазоном измерения
		 * 
		 * @param sensitivity Значение чуствительности @ref Scale
		 * @returns 0, если инициализация завершена успешно
		 * @returns 1, если при инициализации возникла ошибка
		 */
		ISL_StatusTypeDef Init(Scale scale) {
			return LIS3MDL::Init((LIS3MDL::Scale) scale);
		}

		/**
		 * @brief Установка диапазона измерения
		 * 
		 * @param sensitivity Значение чуствительности @ref Scale
		 */
		ISL_StatusTypeDef SetScale(Scale scale) {
			return LIS3MDL::SetScale((LIS3MDL::Scale) scale);
		}

		/**
		 * @brief Чтение значения с датчика во внутренний буфер
		 * 
		 */
		ISL_StatusTypeDef Read() __attribute__((deprecated("This method exists for backward compatibility and does nothing. "
															"Methods X(), Y(), Z(), RawX(), RawY(), RawZ() perform read operation on their own"))) {
			return ISL_OK;
		}

		/**
		 * @note Этот метод возвращает значение из внутреннего буфера, не читая новое значение с датчика. 
		 * Для чтения значения с датчика необходимо вызвать метод @ref Read() 
		 * @brief Получение необработанного значения по оси X
		 * 
		 * @return Необработанное значение напряжённости магнитного поля по оси X
		 */
		int16_t RawX() {
			return LIS3MDL::RawMX();
		}
		
		/**
		 * @note Этот метод возвращает значение из внутреннего буфера, не читая новое значение с датчика. 
		 * Для чтения значения с датчика необходимо вызвать метод @ref Read() 
		 * @brief Получение необработанного значения по оси Y
		 * 
		 * @return Необработанное значение напряжённости магнитного поля по оси Y
		 */
		int16_t RawY() {
			return LIS3MDL::RawMY();
		}
		
		/**
		 * @note Этот метод возвращает значение из внутреннего буфера, не читая новое значение с датчика. 
		 * Для чтения значения с датчика необходимо вызвать метод @ref Read() 
		 * @brief Получение необработанного значения по оси Z
		 * 
		 * @return Необработанное значение напряжённости магнитного поля по оси Z
		 */
		int16_t RawZ() {
			return LIS3MDL::RawMZ();
		}

		/**
		 * @note Этот метод возвращает значение из внутреннего буфера, не читая новое значение с датчика. 
		 * Для чтения значения с датчика необходимо вызвать метод @ref Read() 
		 * @brief Получение значения напряжённости по оси X
		 * 
		 * @return Значение напряжённости магнитного поля по оси X
		 */
		float X() {
			return LIS3MDL::MX();
		}
		
		/**
		 * @note Этот метод возвращает значение из внутреннего буфера, не читая новое значение с датчика. 
		 * Для чтения значения с датчика необходимо вызвать метод @ref Read() 
		 * @brief Получение значения напряжённости по оси Y
		 * 
		 * @return Значение напряжённости магнитного поля по оси Y
		 */
		float Y() {
			return LIS3MDL::MY();
		}
		
		/**
		 * @note Этот метод возвращает значение из внутреннего буфера, не читая новое значение с датчика. 
		 * Для чтения значения с датчика необходимо вызвать метод @ref Read() 
		 * @brief Получение значения напряжённости по оси Z
		 * 
		 * @return Значение напряжённости магнитного поля по оси Z
		 */
		float Z() {
			return LIS3MDL::MZ();
		}
#ifndef ARDUINO	
		// TODO @Goldfor @TeaCupMe Что тут получаем?
		Quaternion<float> GetQuaternion() {
			std::array<float, 3> buf;
			buf[0] = 0;
			buf[1] = 0;
			buf[2] = std::atan2(RawY(), RawX());
			return from_euler(buf);
		}
#endif

		~MagnetometerV2() {};
	};

} /* namespace IntroSatLib */

#endif /* MAGNETOMETER_V2_H_ */
