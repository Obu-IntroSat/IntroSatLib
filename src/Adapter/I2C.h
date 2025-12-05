#ifndef ADAPTER_I2C_H_
#define ADAPTER_I2C_H_

// Include general Library definitions
#include "IntroSatLib_def.h"


#ifdef ARDUINO
/*********************************/
/********** Arduino IDE **********/
/*********************************/
	#include "Arduino.h"
	#include "Wire.h"
	#define	ISL_I2C_ENABLED
	namespace IntroSatLib::interfaces {using I2C_HANDLE_TYPE = TwoWire;}

#else
/*********************************/
/************ Native *************/
/*********************************/
	#if defined(AVR)
	/************** AVR  **************/
		//  This is not yet supported, but it is here for future reference.
		//  AVR-series in Arduino IDE
		#error "Bare AVR outside of Arduino IDE is not yet supported"

	#elif defined(USE_HAL_DRIVER) // TODO Change to more reusable symbol
	/*****  STM32 and stm32duino ******/

		// Include HAL for supported STM32 platforms
		#include "./STM32/stm32xxyy_hal.h"

		#ifndef HAL_MODULE_ENABLED
			#error "HAL configuration incorrect"
		#endif

		#ifdef HAL_I2C_MODULE_ENABLED
			// define STM32-specific handle type for I2C
			#define	ISL_I2C_ENABLED
			namespace IntroSatLib::interfaces {using I2C_HANDLE_TYPE = I2C_HandleTypeDef;}

		#elif !defined(INTROSATLIB_INTERNAL)
			#error "I2C not enabled as part of HAL"
		#endif


	// #elif defined(AMUR)
	/************** AMUR *************/
		// #error "AMUR not yet supported"

	#else
	/************ UNKNOWN ************/
	//#ifndef INTROSATLIB_INTERNAL
		#error "Unsupported system: neither AVR/ARDUINO nor USE_HAL_DRIVER defined. Please check your platform macros."
		#error "Currently supported systems are: stm32 with HAL, stm32duino. AVR planned for future support."
	//#endif
	#endif
#endif /* ARDUINO */


#ifdef ISL_I2C_ENABLED
#define I2C_ENABLED

//#include <array>


#define ASSERT_I2C_HAVE() \
if(!_hi2c) { \
	logText("Haven't i2c handle"); \
	return ISL_StatusTypeDef::ISL_ERROR; \
}

#define LOG_I2C_ADDRESS() \
logText("Device in "); \
logHEX(deviceAddress);

#if LOGDATA
#define LOG_I2C_BUFFER(Sep, Data, Nbytes) { \
logText(" - "); \
for(uint8_t i = 0; i < Nbytes; i++) { \
	logHEX(Data[i]); \
	if (i != (Nbytes - 1)) logText(Sep); \
} \
}

#else
#define LOG_I2C_BUFFER(Sep, Data, Nbytes)
#endif


namespace IntroSatLib {
namespace interfaces {

class I2C final {
	enum class I2CSpeed
	{
		Standard = 0,
		Fast = 1
	};
private:
	I2C_HANDLE_TYPE *_hi2c = 0;
	I2CSpeed _speed = I2CSpeed::Standard;
	ISL_StatusTypeDef innerIsReady(uint8_t address);
public:

	/**
	 * @note Только в STM32CubeIDE
	 * @brief Конструктор объекта устройства на шине I2C
	 *
	 * @param hi2c объект @b I2C_HandleTypeDef
	 * @param address адрес устройства на шине I2C
	 */
	I2C(I2C_HANDLE_TYPE *hi2c);

	/**
	 * @note Только в STM32CubeIDE
	 * @brief Конструктор объекта устройства на шине I2C
	 *
	 * @param hi2c объект @b I2C_HandleTypeDef
	 * @param address адрес устройства на шине I2C
	 * @param speed скорость I2C
	 */
	I2C(I2C_HANDLE_TYPE *hi2c, I2CSpeed speed);

	/**
	 * @note Только в STM32CubeIDE
	 * @brief Конструктор объекта устройства на шине I2C
	 *
	 * @param hi2c объект @b I2C_HandleTypeDef
	 * @param address адрес устройства на шине I2C
	 */
	I2C(I2C_HANDLE_TYPE &hi2c): I2C(&hi2c) {};

	/**
	 * @note Только в STM32CubeIDE
	 * @brief Конструктор объекта устройства на шине I2C
	 *
	 * @param hi2c объект @b I2C_HandleTypeDef
	 * @param address адрес устройства на шине I2C
	 * @param speed скорость I2C
	 */
	I2C(I2C_HANDLE_TYPE &hi2c, I2CSpeed speed): I2C(&hi2c, speed) {};

	I2C(const I2C& other);
	I2C(I2C&& other);
	I2C& operator=(const I2C& other);
	I2C& operator=(I2C&& other);

	/**
	 * @brief Проверка доступности устройства на шине к работе
	 *
	 * @param waitIsReady
	 * @returns 0, если устройство доступно
	 * @returns 1, если с устройством произошла ошибка
	 */
	ISL_StatusTypeDef isReady(uint8_t deviceAddress, uint8_t waitIsReady = 0);

	/**
	 * @brief Отправка запроса на чтение Nbytes байт с устройства
	 *
	 * @param[out] Data Указатель на первый байт (/@b uint8_t ) массива, в которую будет записан результат. В случае
	 * @param[in] Nbytes Количество байт  для чтения
	 * @return Статус выполнения чтения
	 */
	ISL_StatusTypeDef read(uint8_t deviceAddress, uint8_t* data, uint8_t nBytes);

	/**
	 * @brief Отправка запроса на чтение Nbytes байт из регистра Register устройства
	 *
	 * @param[in] Register Адрес регистра для чтения
	 * @param[out] Data Указатель на первый байт (/@b uint8_t ) массива, в который будет записан результат. В случае
	 * @param[in] Nbytes Количество байт для чтения
	 * @return HAL_StatusTypeDef
	 */
	ISL_StatusTypeDef readMem(uint8_t deviceAddress, uint8_t reg, uint8_t* data, uint8_t nBytes);

	/**
	 * @brief Отправка запроса на запись NBytes байт на устройство
	 *
	 * @param Data Указатель на первый байт(/@b uint8_t) массива данных для записи
	 * @param Nbytes Количество байт для записи
	 * @return HAL_StatusTypeDef
	 */
	ISL_StatusTypeDef write(uint8_t deviceAddress, uint8_t* data, uint8_t nBytes);

	/**
	 * @brief Отправка запроса на запись NBytes байт в регистр Register устройства
	 *
	 * @param Register Адрес регистра для записи
	 * @param Data Указатель на первый байт(/@b uint8_t) массива данных для записи
	 * @param Nbytes Количество байт для записи
	 * @return HAL_StatusTypeDef
	 */
	ISL_StatusTypeDef writeMem(uint8_t deviceAddress, uint8_t reg, uint8_t* data, uint8_t nBytes);
	~I2C();
};

} /* namespace intefaces */
} /* namespace IntroSatLib */
#endif /* ISL_I2C_ENABLED */
#endif /* ADAPTER_I2C_H_ */
