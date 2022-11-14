/** Подключение библиотек */
#include <Wire.h>
#include <MotorFlyWheel.h>

/*
 * Включение пространства имён библиотеки спутника
 * Чтобы постоянно не писать IntroSatLib::
 */
using namespace IntroSatLib;

/**
 * 0x3A адрес устройства может быть другим
 */
MotorFlyWheel motor(Wire, 0x3A);

void setup() {
  Serial.begin(9600); // Включения Serial для вывода данных
  Wire.begin(); // Включение Wire(I2C)
  delay(1000);
  while (motor.Init()) {} // Проверка инициализации мотора
}

void loop() {
  for (int i = -3000; i <= 3000; i += 500)
  {
    motor.NeedSpeed(i); // Установка скорости
    for (int y = 0; y < 50; y++)
    {
      Serial.println(motor.CurrentSpeed());  // Вывод текущей скорости
      delay(25);
    }
  }
}
