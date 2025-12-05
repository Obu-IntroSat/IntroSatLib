// Подключение библиотек
#include <Wire.h>
#include <I2CScanner.h>
#include <IS_Bluetooth.h>

#define ADDR 0x1E       //Адрес уствойства для считывания регистров
#define FIRST_REG 0x20  //Стартовый регистор
#define COUNT_REG 3     //Количество считываемых регистров

/* Подключение пространства имён библиотеки,
чтобы постоянно не писать IntroSatLib:: */
using namespace IntroSatLib;

// Создаём объект для сканирования I2C
I2CScanner Scanner;

void setup() {
  // Включение Serial для вывода данных
  Serial.begin(115200,SERIAL_8E1);

  // Инициализация Wire - I2C1
  Wire.begin();

  Serial.println("==== Полное сканирование ====");
  Scanner.scanI2CFull(Wire, Serial);

  Serial.println("==== Сканирование подключенных устройств ====");
  Scanner.scanI2C(Wire, Serial);

  Serial.println("====  Чтение регистров по отдельности ====");
  Scanner.scanDeviceRegisters(Wire, ADDR, FIRST_REG, COUNT_REG, Serial);

  Serial.println("====  Чтение регистров пачкой ====");
  Scanner.scanDeviceRegistersBatch(Wire, ADDR, FIRST_REG, COUNT_REG, Serial);
}

// Бесконечный цикл - основной код программы
void loop() {
  // Проверяем, не пришёл ли запрос на переход в режим перепрошивки
  if (Serial.available())
  {
      if (Serial.read() == 'b')
      {
          // Если пришёл символ 'b', переходим в режим перепрошивки
          enter_bootloader();
      }
  }
}
