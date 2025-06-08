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

- **ArduinoIDE:** скачайте библиотеку в *.zip* формате. Через менеджер библиотек *("Скетч -> Подключить библиотеку -> Добавить .ZIP библиотеку...")* выберите скачанный *.zip* файл. Библиотека подключена ;)
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
        Serial.begin(115200, SERIAL_8E1); // Включения Serial для вывода данных
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
  - [**Methods**](#methods-1)
    - [*void* Init(*Scale* sensitivity, *FilterBandwidth* filter)](#init-1)
    - [*void* SetScale(*Scale* sensitivity)](#setscale-1)
    - [*void* SetFilter(*FilterBandwidth* filter)](#setfilter-1)
    - [*float* X()](#x-1)
    - [*float* Y()](#y-1)
    - [*float* Z()](#z-1)
    - [*int16_t* RawX()](#rawx-1)
    - [*int16_t* RawY()](#rawy-1)
    - [*int16_t* RawZ()](#rawz-1)
  - [**Enums**](#enums-1)
    - [Scale](#scale-1)
    - [FilterBandwidth](#filterbandwidth-1)
- [**Gyroscope**](#gyroscope)
  - [**Methods**](#methods-2)
    - [*void* Init(*Scale* sensitivity, *FilterBandwidth* filter)](#init-2)
    - [*void* SetScale(*Scale* sensitivity)](#setscale-2)
    - [*void* SetFilter(*FilterBandwidth* filter)](#setfilter-2)
    - [*float* X()](#x-2)
    - [*float* Y()](#y-2)
    - [*float* Z()](#z-2)
    - [*int16_t* RawX()](#rawx-2)
    - [*int16_t* RawY()](#rawy-2)
    - [*int16_t* RawZ()](#rawz-2)
  - [**Enums**](#enums-2)
    - [Scale](#scale-2)
    - [FilterBandwidth](#filterbandwidth-2)
- [**MotorFlyWheel**](#flywheel-1)
  - [**Methods**](#methods-3)
    - [*void* Init()](#init-3)
    - [*int16_t* NeedSpeed(*int16_t* needSpeed)](#needspeed-1)
    - [*int16_t* CurrentSpeed()](#currentspeed-1)
- [**CoilFlyWheel**](#flywheel-2)
  - [**Methods**](#methods-4)
	- [*void* Init()](#init-4)
    - [*int16_t* NeedSpeed(*int16_t* needSpeed)](#needspeed-2)
    - [*int16_t* CurrentSpeed()](#currentspeed-2)
      
- ### Accelerometer

 Класс, позволяющий получать данные из акселерометра.
 Находится в пространстве имён ***IntroSatLib***
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

- #### <a id="methods-1">Methods</a>

- ##### <a id="init-1">*void* Init(*[Scale](#scale-1)* sensitivity, *[FilterBandwidth](#filterbandwidth-1)* filter)</a>

   Инициализация акселерометра, нужна для выставления базовых настроек.
   Оба параметра не являются обязательными.
  
   Стандартные значения:
      - sensitivity = *Accelerometer::Scale::twoG*
      - filter =  *Accelerometer::FilterBandwidth::F0021*

   За что отвечают аргументы:
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

- ##### <a id="setscale-1">*void* SetScale(*[Scale](#scale-1)* sensitivity)</a>

   Устанавливает чувствительность акселерометра – задаётся диапазон значений, в котором проводятся измерения.
  
      - *sensitivity* – чувствительность акселерометра

   ```cpp
   accel.SetScale(Accelerometer::Scale::sixteenG);
   ```

- ##### <a id="setfilter-1">*void* SetFilter(*[FilterBandwidth](#filterbandwidth-1)* filter)</a>

   Устанавливает внутренний фильтр акселерометра.

      - *filter* – частота внутреннего фильтра акселерометра

   ```cpp
   accel.SetFilter(Accelerometer::FilterBandwidth::F0005);
   ```

- ##### <a id="x-1">*float* X()</a>

   Получение данных из акселерометра по оси *x* в долях от *g*, то есть если ускорение по оси *x* равно $10м\cdot с^{2}$, то *accel.X() == 1*

   ```cpp
   float x = accel.X();
   ```

- ##### <a id="y-1">*float* Y()</a>

   Получение данных из акселерометра по оси *y* в долях от *g*, то есть если ускорение по оси *y* равно $10м\cdot с^{2}$, то *accel.Y() == 1*

   ```cpp
   float x = accel.Y();
   ```

- ##### <a id="z-1">*float* Z()</a>

   Получение данных из акселерометра по оси *z* в долях от *g*, то есть если ускорение по оси *z* равно $10м\cdot с^{2}$, то *accel.Z() == 1*

   ```cpp
   float x = accel.Z();
   ```

- ##### <a id="rawx-1">*int16_t* RawX()</a>

   Получение сырых данных из акселерометра по оси *x*. Данные никак не преобразуются.

   ```cpp
   int16_t x = accel.RawX();
   ```

- ##### <a id="rawy-1">*int16_t* RawY()</a>

   Получение сырых данных из акселерометра по оси *y*. Данные никак не преобразуются.

   ```cpp
   int16_t x = accel.RawY();
   ```

- ##### <a id="rawz-1">*int16_t* RawZ()</a>

   Получение сырых данных из акселерометра по оси *z*. Данные никак не преобразуются.

   ```cpp
   int16_t x = accel.RawZ();
   ```

- #### <a id="enums-1">Enums</a>
  - ##### <a id="scale-1">Scale</a>

   Перечисление нужно для настройки диапазона измерений акселерометра. 
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

- ##### <a id="filterbandwidth-1">FilterBandwidth</a>

   Перечисление нужно для установки аппаратного фильтра акселерометра.
   *Обозначение переменной для хранения элемента перечисления*

   ```cpp
   Accelerometer::FilterBandwidth filter = Accelerometer::FilterBandwidth::F0010;
   ```

   Значения в перечислении:

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

 Класс, позволяющий получать данные из гироскопа.
 Находится в пространстве имён ***IntroSatLib***
 *Этой строчкой подключается*

 ```cpp
 using namespace IntroSatLib;
 ```

 Параметры:

- *hi2c* = I2C интерфейс
- *address* = необязательный параметр, стандартное значение *0x68*. Адрес гироскопа.
 ***Gyroscope* Gyroscope(*I2C_HandleTypeDef* \*hi2c, *uint8_t* address)** *(STM32CubeIDE)*
 *или*
 ***Gyroscope* Gyroscope(*TwoWire* &hi2c, *uint8_t* address)** *(ArduinoIDE)*
Создание объекта класса:

 Для [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html)

 ```cpp
 Gyroscope gyro = Gyroscope(&hi2c1, 0x68);
 // или
 Gyroscope gyro = Gyroscope(&hi2c1);
 ```

 Для [ArduinoIDE](https://www.arduino.cc/en/software)

 ```cpp
 Gyroscope gyro = Gyroscope(Wire, 0x68);
 // или
 Gyroscope gyro = Gyroscope(Wire);
 ```
 

- #### <a id="methods-2">Methods</a>

- ##### <a id="init-2">*void* Init([*Scale*](#scale-2) sensitivity, [*FilterBandwidth*](#filterbandwidth-2) filter)</a>

   Инициализация гироскопа, нужна для выставления базовых настроек.
   Оба параметра не являются обязательными.
  
   Стандартные значения:
      - sensitivity = *Gyroscope::Scale::DPS0250*
      - filter =  *Gyroscope::FilterBandwidth::F0005*

   За что отвечают аргументы:
      - *sensitivity* устанавливает чувствительность гироскопа
      - *filter* устанавливает аппаратный фильтр в гироскопе

   ```cpp
   gyro.Init(Gyroscope::Scale::DPS0500, Gyroscope::FilterBandwidth::F0020);
   ```

   или

   ```cpp
   gyro.Init(Gyroscope::Scale::DPS2000);
   ```

   или

   ```cpp
   gyro.Init();
   ```

- ##### <a id="setscale-2">*void* SetScale(*[Scale](#scale-1)* sensitivity)</a>

Устанавливает чувствительность гироскопа – задаётся диапазон значений, в котором проводятся измерения.
  
      - *sensitivity* – чувствительность гироскопа

   ```cpp
   gyro.SetScale(Gyroscope::Scale::DPS0500);
   ```

- ##### <a id="setfilter-2">*void* SetFilter(*[FilterBandwidth](#filterbandwidth-2)* filter)</a>

   Устанавливает внутренний фильтр гироскопа.

      - *filter* – частота внутреннего фильтра акселерометра

   ```cpp
   gyro.SetFilter(Gyroscope::FilterBandwidth::F0020);
   ```

- ##### <a id="x-2">*float* X()</a>

   Получение данных из гироскопа по оси *x* в *град/с*

   ```cpp
   float x = gyro.X();
   ```

- ##### <a id="y-2">*float* Y()</a>

   Получение данных из гироскопа по оси *y* в *град/с*

   ```cpp
   float x = gyro.Y();
   ```

- ##### <a id="z-2">*float* Z()</a>

   Получение данных из гироскопа по оси *z* в *град/с*

   ```cpp
   float x = gyro.Z();
   ```

- ##### <a id="rawx-2">*int16_t* RawX()</a>

   Получение сырых данных из гироскопа по оси *x*. Данные никак не преобразуются.

   ```cpp
   int16_t x = gyro.RawX();
   ```

- ##### <a id="rawy-2">*int16_t* RawY()</a>

   Получение сырых данных из гироскопа по оси *y*. Данные никак не преобразуются.

   ```cpp
   int16_t x = gyro.RawY();
   ```

- ##### <a id="rawz-2">*int16_t* RawZ()</a>

   Получение сырых данных из гироскопа по оси *z*. Данные никак не преобразуются.

   ```cpp
   int16_t x = gyro.RawZ();
   ```
- #### <a id="enums-2">Enums</a>

- ##### <a id="scale-2">Scale</a>
  Перечисление нужно для настройки диапазона измерений гироскопа. 
   *Обозначение переменной для хранения элемента перечисления*

   ```cpp
   Gyroscope::Scale scale = Gyroscope::Scale::DPS0250;
   ```

   Значения в перечислении
   ***DPS0250*** — диапазон работы в *$\plusmn$250* град/с
   ***DPS0500*** — диапазон работы в *$\plusmn$500* град/с
   ***DPS1000*** — диапазон работы в *$\plusmn$1000* град/с
   ***DPS2000*** — диапазон работы в *$\plusmn$2000* град/с

   ```cpp
   enum
	{
		DPS0250 = 0,
		DPS0500,
		DPS1000,
		DPS2000
	}

   ```
- ##### <a id="filterbandwidth-2">FilterBandwidth</a>

Перечисление нужно для установки аппаратного фильтра акселерометра.
   *Обозначение переменной для хранения элемента перечисления*

   ```cpp
   Gyroscope::FilterBandwidth filter = Gyroscope::FilterBandwidth::F0010;
   ```

   Значения в перечислении:

   ```cpp
   enum
   {
    	F8800 = 8,
		F3600 = 16,
		F0250 = 0,
		F0184,
		F0092,
		F0041,
		F0020,
		F0010,
		F0005,
		F3600b
   }
   ```
  
- ### <a id="flywheel-1">MotorFlyWheel</a>

 Класс, позволяющий управлять скоростью и направлением вращения маховика.
 Находится в пространстве имён ***IntroSatLib***
 *Этой строчкой подключается*

 ```cpp
 using namespace IntroSatLib;
 ```
 
 Создание объекта класса
 
 ***MotorFlyWheel* MotorFlyWheel(*I2C_HandleTypeDef* \*hi2c, *uint8_t* address)** *(STM32CubeIDE)*
 *или*
 ***MotorFlyWheel* MotorFlyWheel(*TwoWire* &hi2c, *uint8_t* address)** *(ArduinoIDE)*

 Параметры:

- *hi2c*, *TwoWire* = I2C интерфейс
- *address* = необязательный параметр, стандартное значение *0x38*. Адрес платы маховика.

 Для [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html)

 ```cpp
 MotorFlyWheel motor = MotorFlyWheel(&hi2c1, 0x38);
 // или
 MotorFlyWheel motor = MotorFlyWheel(&hi2c1);
 ```

 Для [ArduinoIDE](https://www.arduino.cc/en/software)

 ```cpp
 MotorFlyWheel motor = MotorFlyWheel(Wire, 0x38);
 // или
 MotorFlyWheel motor = MotorFlyWheel(Wire);
 ```
- #### <a id="methods-3">Methods</a>

- ##### <a id="init-3">*void* Init()</a>

   Инициализация платы маховика, нужна для выставления базовых настроек.
  
   ```cpp
   motor.Init();
   ```
   
- ##### <a id="needspeed-1">*int16_t* NeedSpeed(*int16_t* needSpeed)</a>

  Метод задает скорость вращения маховика.
  Допустимые значения аргумента *needSpeed* лежат в пределах от -3000 до 3000, что соответствует скорости вращения маховика в оборотах в минуту.

  Знак переменной *needSpeed* определяет направение вращения:
  
  "+" - вращение против часовой стрелки;
  "-" - вращение по часовой стрелке.
  
   ```cpp
   motor.NeedSpeed(needSpeed);
   ```

- ##### <a id="currentspeed-1">*int16_t* CurrentSpeed()</a>

Метод позволяет считать текущую скорость вращения маховика. Возвращает значение в оборотах в минуту. Знак полученного числа будет определять текущее направление вращения маховика:

  "+" - вращение против часовой стрелки;
  "-" - вращение по часовой стрелки.
  
   ```cpp
   motor.CurrentSpeed();
   ```

- ### <a id="flywheel-2">CoilFlyWheel</a>

Класс, позволяющий управлять величиной и полярностью напряжения, подаваемого на магнитные катушки.
Находится в пространстве имён ***IntroSatLib***
*Этой строчкой подключается*

```cpp
using namespace IntroSatLib;
```
 
Создание объекта класса
 
***CoilFlyWheel* CoilFlyWheel(*I2C_HandleTypeDef* \*hi2c, *uint8_t* address)** *(STM32CubeIDE)*
*или*
***CoilFlyWheel* CoilFlyWheel(*TwoWire* &hi2c, *uint8_t* address)** *(ArduinoIDE)*

Параметры:

- *hi2c*, *TwoWire* = I2C интерфейс
- *address* = необязательный параметр, стандартное значение *0x38*. Адрес платы маховика.

Для [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html)

```cpp
CoilFlyWheel motor = CoilFlyWheel(&hi2c1, 0x38);
// или
CoilFlyWheel motor = CoilFlyWheel(&hi2c1);
```

Для [ArduinoIDE](https://www.arduino.cc/en/software)

```cpp
CoilFlyWheel motor = CoilFlyWheel(Wire, 0x38);
// или
CoilFlyWheel motor = CoilFlyWheel(Wire);
```

- #### <a id="methods-4">Methods</a>

- ##### <a id="init-4">*void* Init()</a>

Инициализация платы маховика, нужна для выставления базовых настроек.

```cpp
motor.Init();
```

- ##### <a id="needspeed-2">*int16_t* NeedSpeed(*int16_t* needSpeed)</a>

Позволяет задать величину и полярность напряжения, подаваемого на катушки. 
Допустимые значения аргумента *needSpeed* лежат в пределах от -32767 до 32767, что соответствует подаче на катушки напряжения от -5 до 5 В.  

```cpp
motor.NeedSpeed(needSpeed);
```

- ##### <a id="currentspeed-2">*int16_t* CurrentSpeed()</a>

Возвращает значение напряжения, поданного на катушки, в условных единицах (в диапазоне от -32767 до 32767).

```cpp
motor.CurrentSpeed();
```

> ## Полезные ссылки
>
> Наш [канал в Telegram](https://t.me/introsat_news) поможет не пропустить обновления.
> 
> Если у вас возникли вопросы или сложности при работе с IntroSat, ответы можно найти в нашем [F.A.Q.](https://docs.google.com/document/d/15KqFrMlc6Jzxut_zMf_pXNx5r5JTjqfKEvCHWx99rEc/edit#heading=h.demjj79bt080)
>
> Остались вопросы? Напишите нашему [боту в Telegram](https://t.me/introsatBot)! Укажите в обращении модуль конструктора, при работе с которым возникли проблемы, и версии платы (написаны на самих платах), и мы обязательно вам поможем.
