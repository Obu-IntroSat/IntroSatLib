/** Подключение библиотек */
#include <Wire.h>
#include <Accelerometer.h>

/*
 * Включение пространства имён библиотеки спутника
 * Чтобы постоянно не писать IntroSatLib::
 */
using namespace IntroSatLib;

/** Создание класса акселерометра */
Accelerometer accel(Wire);

void setup() {
	Serial.begin(9600); // Включения Serial для вывода данных
	Wire.begin(); // Включение Wire(I2C)
	accel.Init(); // Инициализация датчика
	delay(1000);
	Serial.println("ax, ay, az"); // Для работы плоттера в ArduinoIDE
}

/** Вывод данных в бесконечном цикле*/
void loop() {
	Serial.print(accel.X());
	Serial.print(" ");
	Serial.print(accel.Y());
	Serial.print(" ");
	Serial.println(accel.Z());
	delay(100);
}
