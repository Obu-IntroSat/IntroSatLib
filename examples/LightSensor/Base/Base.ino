/** Подключение библиотек */
#include <Wire.h>
#include <LightSensor.h>

/*
 * Включение пространства имён библиотеки спутника
 * Чтобы постоянно не писать IntroSatLib::
 */
using namespace IntroSatLib;

/** Создание класса гироскопа */
LightSensor light(Wire);

/** Переменная для хранения показаний датчика */
float l = 0;

void setup() {
	Serial.begin(9600); // Включения Serial для вывода данных
	Wire.begin(); // Включение Wire(I2C)
	light.Init(); // Инициализация датчика
	delay(1000);
	Serial.println("light: "); // Для работы плоттера в ArduinoIDE
}

void loop() {

  /** Считывания данных */
	l = light.GetLight();

  /** Вывод данных */
	Serial.println(l);
	delay(100);
}
