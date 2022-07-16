/** Подключение библиотек */
#include <Wire.h>
#include <Gyroscope.h>

/*
 * Включение пространства имён библиотеки спутника
 * Чтобы постоянно не писать IntroSatLib::
*/
using namespace IntroSatLib;

/** Создание класса гироскопа*/
Gyroscope gyro(Wire);

/** Переменные для хранения показаний датчика*/
float ax = 0, ay = 0, az = 0;

void setup() {
	Serial.begin(9600); // Включения Serial для вывода данных
	Wire.begin(); // Включение Wire(I2C)
	gyro.Init(); // Инициализация датчика
	delay(1000);
	Serial.println("ax, ay, az"); // Для работы плоттера в ArduinoIDE
}

/** Вывод данных в бесконечном цикле*/
void loop() {
	Serial.print(gyro.X());
	Serial.print(" ");
	Serial.print(gyro.Y());
	Serial.print(" ");
	Serial.println(accel.Z());
	delay(100);
}
