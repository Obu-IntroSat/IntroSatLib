/** Подключение библиотек */
#include <Wire.h>
#include <CoilFlyWheel.h>

/*
 * Включение пространства имён библиотеки спутника
 * Чтобы постоянно не писать IntroSatLib::
 */
using namespace IntroSatLib;

/**
 * 0x3A адрес устройства может быть другим
 */
ColiFlyWheel coil(Wire, 0x3A);

void setup() {
  Serial.begin(9600); // Включения Serial для вывода данных
  Wire.begin(); // Включение Wire(I2C)
  delay(1000);
  while (coil.Init()) {} // Проверка инициализации катушки
}

void loop() {
  for (int i = -30000; i <= 30000; i += 5000)
  {
    coil.NeedSpeed(i); // Установка заполнение (на самом деле от -32767 до +32767) тут от -30000 до +30000 для примера
    delay(1000);
  }
}
