# [IntroSatLib]()

Библиотека для взаимодействия с компонентами спутника

## Required hardware and software product 
- Библиотека подходит для [ArduinoIDE](https://www.arduino.cc/en/software) и [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html)
- Библиотека подходит только для контроллеров
  * STM32F1XX
  * STM32F103C8T6

## Оглавление
* [Установка](#installation)
* [Использование](#using)
* [Документация](#documentation)


## Installation
* **ArduinoIDE:** скачайте библиотеку в *.zip* формате. Через менеджер библиотек *("Скетч -> Подключить библиотеку -> Добавить .ZIP библиотеку...")* выберете скачанный *.zip* файл. Библиотека подключена ;)
* **STM32CubeIDE:** 

## Using
* **ArduinoIDE:** базовое использование библиотеки
    ```cpp
    /** Подключение библиотек */
    #include <Wire.h>
    #include <Accelerometer.h>

    /*
     * Включение пространства имён библиотеки спутника
     * Чтобы постоянно не писать IntroSatLib::
    */
    using namespace IntroSatLib;

    /** Создание класса акселерометра*/
    Accelerometer accel(Wire);

    /** Переменные для хранения показаний датчика*/
    float ax = 0, ay = 0, az = 0;

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
    ```
## Documentation
