// Подключение библиотек
#include <Wire.h>
#include <AK8963.h>
#include <IS_Bluetooth.h>

/* Подключение пространства имён библиотеки,
чтобы постоянно не писать IntroSatLib:: */
using namespace IntroSatLib;

// Создание объекта магнитометра
AK8963 magn(Wire);

void setup() {
	// Включение Serial для вывода данных
	Serial.begin(115200, SERIAL_8E1);

	// Инициализация Wire - I2C1
	Wire.begin();

	// Инициализация датчика
	uint8_t status = magn.Init();

	/* Проверка успешной инициализации. 
	  	 	0 - Инициализция успешна
	 			1-3 - Ошибка инициализации */
	if (status != 0) {
		Serial.print("Ошибка инициализации магнитометра! Код ошибки: ");
		Serial.println(status);
		Serial.println("Проверьте подключение и адрес датчика!");
		// Если датчик не инициализирован - уходим в бесконечный цикл
		while (1)
			;
	}
}

// Бесконечный цикл - основной код программы
void loop() {
	// Выводим данные и названия в бесконечном цикле
	Serial.print("mx:");
	Serial.print(magn.X());
	Serial.print(",my:");
	Serial.print(magn.Y());
	Serial.print(",mz:");
	Serial.println(magn.Z());

	// Проверяем, не пришёл ли запрос на переход в режим перепрошивки
	if (Serial.available()) {
		if (Serial.read() == 'b') {
			// Если пришёл символ 'b', переходим в режим перепрошивки
			enter_bootloader();
		}
	}

	// Задержка в 100 мс, чтобы данные приходили с небольшим перерывом
	delay(100);
}
