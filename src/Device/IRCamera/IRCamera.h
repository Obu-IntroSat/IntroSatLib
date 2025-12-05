#ifndef IRCAMERA_H_
#define IRCAMERA_H_

#include "../I2CDevice.h"
#include "Adapter/GPIO.h"
//#include "../BaseDevice.h"

namespace IntroSatLib {

class IRCamera: private I2CDevice {
private:

	static const uint8_t BASE_ADDRESS = 0x68;
	static constexpr float _rawdeg = 0.25f;

	enum RegisterMap
	{
	  AMG88xx_PCTL = 0x00,
	  AMG88xx_RST = 0x01,
	  AMG88xx_FPSC = 0x02,
	  AMG88xx_INTC = 0x03,
	  AMG88xx_STAT = 0x04,
	  AMG88xx_SCLR = 0x05,
	  // 0x06 reserved
	  AMG88xx_AVE = 0x07,
	  AMG88xx_INTHL = 0x08,
	  AMG88xx_INTHH = 0x09,
	  AMG88xx_INTLL = 0x0A,
	  AMG88xx_INTLH = 0x0B,
	  AMG88xx_IHYSL = 0x0C,
	  AMG88xx_IHYSH = 0x0D,
	  AMG88xx_TTHL = 0x0E,
	  AMG88xx_TTHH = 0x0F,
	  AMG88xx_INT_OFFSET = 0x010,
	  AMG88xx_PIXEL_OFFSET = 0x80
	};


	uint8_t _framrate = 0;
	uint8_t _mirror = 0;

	interfaces::GPIO* _reset = nullptr;

	int16_t _buffer[64] = {0};

	void tryReset();
	int16_t int12ToInt16(uint16_t val);

public:
	/**
	 * @brief Количество кадров (обновлений данных) в секунду
	 * 
	 */
	enum Framerate
	{
		FPS_10 = 0,	/**< 10 кадров в секунду */
		FPS_1 = 1	/**< 1 кадр в секунду */
	};

//#ifndef ARDUINO
	/**
	 * @note Только в STM32CubeIDE
	 * @brief Создание объекта ИК-камеры. 
	 * 
	 * @param hi2c объект @b I2C_HandleTypeDef
	 * @param address адрес ИК-камеры на шине I2C
	 */
	IRCamera(const interfaces::I2C &i2c, uint8_t address = BASE_ADDRESS);
//#else
//	/**
//	 * @note Только в Arduino IDE
//	 * @brief Создание объекта ИК-камеры
//	 *
//	 * @param hi2c объект @b TwoWire или @b Wire
//	 * @param address адрес ИК-камеры на шине I2C
//	 */
//	IRCamera(TwoWire &hi2c, uint8_t address = BASE_ADDRESS);
//
//	/**
//	 * @note Только в Arduino IDE
//	 * @brief Создание объекта ИК-камеры на @b I2C1
//	 *
//	 * @param address адрес ИК-камеры на шине I2C
//	 */
//	IRCamera(uint8_t address = BASE_ADDRESS);
//#endif
	/**
	 * @brief Создание объекта ИК-камеры как копии другого объекта ИК-камеры
	 * 
	 * @param other исходный объект для копирования
	 */
	IRCamera(const IRCamera &other);
	IRCamera(IRCamera &&other);
	IRCamera& operator=(const IRCamera &other);
	IRCamera& operator=(IRCamera &&other);

	/**
	 * @brief Включает использование пина reset ИК-камеры.
	 * 
	 * @param resetPort Порт, которому подключён контакт 
	 * @param resetPin 
	 */
	void useForceReset(interfaces::GPIO_HANDLE_TYPE& resetPort, uint16_t resetPin = 0);

	/**
	 * @brief Вывод картинки в отзеркаленном виде
	 * 
	 */
	void useMirrored();

	/**
	 * @brief Вывод картинки в нормальном (не отзеркаленном) виде
	 * 
	 */
	void useNotMirrored();

	/**
	 * @brief Инициализация камеры с параметрами по умолчанию: @ref Framerate::FPS_10
	 * 
	 * @returns 0, если инициализация завершена успешно
	 * @returns 1, если при инициализации возникла ошибка
	 */
	ISL_StatusTypeDef Init() override;

	/**
	 * @brief Инициализация камеры с заданным количеством кадров в секунду
	 * 
	 * @param framrate
	 * @returns 0, если инициализация завершена успешно
	 * @returns 1, если при инициализации возникла ошибка
	 */
	// TODO @TeaCupMe @Goldfor исправить опечатку framrate на framerate
	ISL_StatusTypeDef Init(Framerate framrate);

	/**
	 * @brief Считывание значений с матрицы ИК-камеры во внутренний буффер.
	 * 
	 * @returns 0, если изображение считано успешно
	 * @returns 1, если считывании возникла ошибка
	 */
	uint8_t Read();

	/**
	 * @brief Получение необработанного значения пикселя
	 * 
	 * @param x Координата x пикселя
	 * @param y Координата y пикселя
	 * @return Необработанное значение пикселя, 12 бит, right-aligned
	 */
	int16_t getPixelRaw(uint8_t x, uint8_t y);

	/**
	 * @brief Получение значения пикселя
	 * 
	 * @param x Координата x пикселя
	 * @param y Координата y пикселя
	 * @return Значение пикселя в диапазоне от 0 до 1
	 */
	float getPixel(uint8_t x, uint8_t y);

	~IRCamera() override;
};

} /* namespace IntroSatLib */

#endif /* IRCAMERA_H_ */
