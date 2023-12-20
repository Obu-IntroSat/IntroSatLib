# [IntroSatLib]()

Библиотека для взаимодействия с компонентами спутника

## Required hardware and software product

- Библиотека подходит для [ArduinoIDE](https://www.arduino.cc/en/software) и [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html)
- Библиотека подходит только для контроллеров
  - STM32F1XX
  - STM32F103C8T6

## Оглавление

- [Установка](#installation)
- [Использование](#using)
- [Документация](#documentation)

## Installation

- **ArduinoIDE:** скачайте библиотеку в *.zip* формате. Через менеджер библиотек *("Скетч -> Подключить библиотеку -> Добавить .ZIP библиотеку...")* выберете скачанный *.zip* файл. Библиотека подключена ;)
- **STM32CubeIDE:**

## Using

- **ArduinoIDE:** базовое использование библиотеки

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

- [**Accelerometer**](#accelerometer)
  - [**Methods**](#methods)
    - [*void* Init(*Scale* sensitivity, *FilterBandwidth* filter)](#void-initscale-sensitivity-filterbandwidth-filter)
    - [*void* SetScale(*Scale* sensitivity)](#void-setscalescalescale-sensitivity)
    - [*void* SetFilter(*FilterBandwidth* filter)](#void-setfilterfilterbandwidthfilterbandwidth-filter)
    - [*float* X()](#float-x)
    - [*float* Y()](#float-y)
    - [*float* Z()](#float-z)
    - [*int16_t* RawX()](#int16t-rawx)
    - [*int16_t* RawY()](#int16t-rawy)
    - [*int16_t* RawZ()](#int16t-rawz)
  - [**Enums**](#enums)
    - [Scale](#scale)
    - [FilterBandwidth](#filterbandwidth)
- [**Gyroscope**](#gyroscope)
  - [**Methods**](#methods-1)
    - [*void* Init(*Scale* sensitivity, *FilterBandwidth* filter)]()
    - [*void* SetScale(*Scale* sensitivity)]()
    - [*void* SetFilter(*FilterBandwidth* filter)]()
    - [*float* X()](#float-x-1)
    - [*float* Y()](#float-y-1)
    - [*float* Z()](#float-z-1)
    - [*int16_t* RawX()](#int16t-rawx-1)
    - [*int16_t* RawY()](#int16t-rawy-1)
    - [*int16_t* RawZ()](#int16t-rawz-1)
  - Enums
    - [Scale]()
    - [FilterBandwidth]()
- [**FlyWheel**](#flywheel)
  - [**Methods**](#methods-2)

- ### Accelerometer

 Класс позволяющий получать данные из акселерометра
 Находится в пространстве ***IntroSatLib***
 *Этой строчкой подключается*

 ```cpp
 using namespace IntroSatLib;
 ```

 Параметры:

- *hi2c* = I2C интерфейс
- *address* = необязательный параметр, стандартное значение *0x68*. Адрес акселерометра
 ***Accelerometer* Accelerometer(*I2C_HandleTypeDef* \*hi2c, *uint8_t* address)** *(STM32CubeIDE)*
 *или*
 ***Accelerometer* Accelerometer(*TwoWire* &hi2c, *uint8_t* address)** *(ArduinoIDE)*
Создание объекта класса

 Для [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html)

 ```cpp
 Accelerometer accel = Accelerometer(&hi2c1, 0x68);
 // или
 Accelerometer accel = Accelerometer(&hi2c1);
 ```

 Для [ArduinoIDE](https://www.arduino.cc/en/software)

 ```cpp
 Accelerometer accel = Accelerometer(Wire, 0x68);
 // или
 Accelerometer accel = Accelerometer(Wire);
 ```

- #### Methods

- ##### *void* Init(*[Scale](#scale)* sensitivity, *[FilterBandwidth](#filterbandwidth)* filter)

   Инициализация акселерометра, нужна для выставления базовых настроек
   Оба параметра не являются обязательными
   Стандартные значения:
      - sensitivity = *Accelerometer::Scale::twoG*
      - filter =  *Accelerometer::FilterBandwidth::F0021*

   Зачем нужны? ;)
      - *sensitivity* устанавливает чувствительность акселерометра
      - *filter* устанавливает аппаратный фильтр в акселерометре

   ```cpp
   accel.Init(Accelerometer::Scale::sixteenG, Accelerometer::FilterBandwidth::F0005);
   ```

   или

   ```cpp
   accel.Init(Accelerometer::Scale::sixteenG);
   ```

   или

   ```cpp
   accel.Init();
   ```

- ##### *void* SetScale(*[Scale](#scale)* sensitivity)

   Устанавливает чувствительность акселерометра
   Зачем нужны? ;)
      - *sensitivity* чувствительность акселерометра

   ```cpp
   accel.SetScale(Accelerometer::Scale::sixteenG);
   ```

- ##### *void* SetFilter(*[FilterBandwidth](#filterbandwidth)* filter)

   Устанавливает внутренний фильтр акселерометра
   Зачем нужны? ;)
      - *filter* частота внутреннего фильтра акселерометра

   ```cpp
   accel.SetFilter(Accelerometer::FilterBandwidth::F0005);
   ```

- ##### *float* X()

   Получение данных из акселерометра по оси *x* в долях от *g*, то есть если ускорение по оси *x* равно $10м\cdot с^{2}$, то *accel.X() == 1*

   ```cpp
   float x = accel.X();
   ```

- ##### *float* Y()

   Получение данных из акселерометра по оси *y* в долях от *g*, то есть если ускорение по оси *y* равно $10м\cdot с^{2}$, то *accel.Y() == 1*

   ```cpp
   float x = accel.Y();
   ```

- ##### *float* Z()

   Получение данных из акселерометра по оси *z* в долях от *g*, то есть если ускорение по оси *z* равно $10м\cdot с^{2}$, то *accel.Z() == 1*

   ```cpp
   float x = accel.Z();
   ```

- ##### *int16_t* RawX()

   Получение сырых данных из акселерометра по оси *x*. Данные никак не преобразуются.

   ```cpp
   int16_t x = accel.RawX();
   ```

- ##### *int16_t* RawY()

   Получение сырых данных из акселерометра по оси *y*. Данные никак не преобразуются.

   ```cpp
   int16_t x = accel.RawY();
   ```

- ##### *int16_t* RawZ()

   Получение сырых данных из акселерометра по оси *z*. Данные никак не преобразуются.

   ```cpp
   int16_t x = accel.RawZ();
   ```

- #### Enums
  - ##### Scale

   Перечисление нужно для указания разрешения работы акселерометра в человеко понятно виде
   *Обозначение переменной для хранения элемента перечисления*

   ```cpp
   Accelerometer::Scale scale = Accelerometer::Scale::twoG;
   ```

   Значения в перечислении
   ***twoG*** — диапазон работы в *$\plusmn$2g*
   ***fourG*** — диапазон работы в *$\plusmn$4g*
   ***eightG*** — диапазон работы в *$\plusmn$8g*
   ***sixteenG*** — диапазон работы в *$\plusmn$16g*

   ```cpp
   enum
   {
    twoG = 0, 
    fourG = 1,
    eightG = 2,
    sixteenG = 3,
   }
   ```

- ##### FilterBandwidth

   Перечисление нужно для указания разрешения работы акселерометра в человеко понятно виде
   *Обозначение переменной для хранения элемента перечисления*

   ```cpp
   Accelerometer::FilterBandwidth filter = Accelerometer::FilterBandwidth::F0010;
   ```

   Значения в перечислении

   ```cpp
   enum
   {
    F0218 = 0,
    F0218b = 1,
    F0099 = 2,
    F0045 = 3,
    F0021 = 4,
    F0010 = 5,
    F0005 = 6,
    F0420 = 7
    F1046 = 8,
   }
   ```

- ### Gyroscope

 Класс позволяющий получать данные из гироскопа

- #### Methods

- ##### *float* X()

   Получение данных из гироскопа по оси *x* в *град/с*

   ```cpp
   float x = gyro.X();
   ```

- ##### *float* Y()

   Получение данных из гироскопа по оси *y* в *град/с*

   ```cpp
   float x = gyro.Y();
   ```

- ##### *float* Z()

   Получение данных из гироскопа по оси *z* в *град/с*

   ```cpp
   float x = gyro.Z();
   ```

- ##### *int16_t* RawX()

   Получение сырых данных из гироскопа по оси *x*. Данные никак не преобразуются.

   ```cpp
   int16_t x = gyro.RawX();
   ```

- ##### *int16_t* RawY()

   Получение сырых данных из гироскопа по оси *y*. Данные никак не преобразуются.

   ```cpp
   int16_t x = gyro.RawY();
   ```

- ##### *int16_t* RawZ()

   Получение сырых данных из гироскопа по оси *z*. Данные никак не преобразуются.

   ```cpp
   int16_t x = gyro.RawZ();
   ```
- ### FlyWheel

 Класс позволяющий управлять скоростью и направлением вращения маховика
 Находится в пространстве ***IntroSatLib***
 *Этой строчкой подключается*

 ```cpp
 using namespace IntroSatLib;
 ```
 
 Создание объекта класса
 
 ***FlyWheel* FlyWheel(*I2C_HandleTypeDef* \*hi2c, *uint8_t* address)** *(STM32CubeIDE)*
 *или*
 ***FlyWheel* FlyWheel(*TwoWire* &hi2c, *uint8_t* address)** *(ArduinoIDE)*

 Параметры:

- *hi2c* = I2C интерфейс
- *address* = необязательный параметр, стандартное значение *0x38*. Адрес платы маховика

 Для [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html)

 ```cpp
 FlyWheel motor = FlyWheel(&hi2c1, 0x38);
 // или
 FlyWheel motor = FlyWheel(&hi2c1);
 ```

 Для [ArduinoIDE](https://www.arduino.cc/en/software)

 ```cpp
 FlyWheel motor = FlyWheel(Wire, 0x38);
 // или
 FlyWheel motor = FlyWheel(Wire);
 ```
- #### Methods

- ##### *void* Init()

   Инициализация платы маховика, нужна для выставления базовых настроек.
  
   ```cpp
   motor.Init();
   ```
   
- ##### *int16_t* NeedSpeed(*int16_t* needSpeed)

  Метод задает скорость вращения маховика. Знак переменной *needSpeed* определяет направение вращения:
  
  "+" - вращение против часовой стрелки;
  "-" - вращение по часовой стрелки.
  
   ```cpp
   motor.NeedSpeed(needSpeed);
   ```

- ##### *int16_t* CurrentSpeed()

  Метод позволяет считать текущую скорость вращения маховика. Знак полученного числа будет определять текущее направление вращения маховика:

  "+" - вращение против часовой стрелки;
  "-" - вращение по часовой стрелки.
  
   ```cpp
   motor.CurrentSpeed();
   ```
