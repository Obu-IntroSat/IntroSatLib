/** Подключение библиотек */
#include <Wire.h>
#include <Accelerometer.h>

/*
   Включение пространства имён библиотеки спутника
   Чтобы постоянно не писать IntroSatLib::
*/
using namespace IntroSatLib;

/** Создание класса акселерометра*/
Accelerometer accel(Wire);

/** Переменные для хранения показаний датчика */
float ax = 0, ay = 0, az = 0;

/** Начальное разрешение */
Accelerometer::Scale lastScale = Accelerometer::Scale::twoG;

void setup() {
  Serial.begin(9600); // Включения Serial для вывода данных
  Wire.begin(); // Включение Wire(I2C)
  accel.Init(lastScale); // Инициализация датчика с максимальным разрешением в 2g (стандартная настройка)
  delay(1000);
  Serial.println("ax, ay, az"); // Для работы плоттера в ArduinoIDE
}

void loop() {
  /** Считывание данных из акселерометра */
  ax = accel.X();
  ay = accel.Y();
  az = accel.Z();

  float mx = max(max(abs(ax), abs(ay)), abs(az)); // Находение максимального модуля ускорения по одной из осей

  Accelerometer::Scale nowScale = Accelerometer::Scale::twoG; // Нынешнее разрешение акселерометра, есди стоновится больше проговых значаний изменяем на следующее разрешение
  if (mx > 1.5)
  {
    nowScale = Accelerometer::Scale::fourG;
  }
  else if (mx > 3.5)
  {
    nowScale = Accelerometer::Scale::eightG;
  }
  else
  {
    nowScale = Accelerometer::Scale::sixteenG;
  }

  if (nowScale != lastScale) // Изменяем разрешение, если оно не совпадает с прошлым
  {
    accel.SetScale(nowScale);
    lastScale = nowScale;
  }

  /** Выводим данные */
  Serial.print(ax);
  Serial.print(" ");
  Serial.print(ay);
  Serial.print(" ");
  Serial.println(az);
  delay(50);
}
