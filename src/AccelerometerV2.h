#ifndef ACCELEROMETERV2_H_
#define ACCELEROMETERV2_H_

#include "I2CDevice.h"
#include "BaseDevice.h"

namespace IntroSatLib {

class AccelerometerV2: public BaseDevice {

private:
  
	static const uint8_t BASE_ACCEL_CONF = 0xA8;
	static const uint8_t BASE_ADDRESS = 0x6B;
	static constexpr float _rawg = 16384.0;
    
	enum RegisterMap
	{
		WHO_AM_I_REG = 0X0F,
		CTRL1_XL = 0X10,
		CTRL8_XL = 0X17,
		OUT_TEMP_L = 0X20,
		OUT_TEMP_H = 0X21,
		OUTX_L_XL = 0X28,
		OUTX_H_XL = 0X29,
		OUTY_L_XL = 0X2A,
		OUTY_H_XL = 0X2B,
		OUTZ_L_XL = 0X2C,
		OUTZ_H_XL = 0X2D,
    };

public:
	enum Scale
	{
		twoG = 0,
		fourG = 2,
		eightG = 3,
		sixteenG = 1
	};

	enum FilterBandwidth
	{
		F400H = 0,    //400Hz
		F200H = 1,    //200Hz
		F100H = 2,    //100Hz
		F050H = 3     //50Hz
	};

	enum DataRate
	{
		DR12_5H = 1,      //12.5Hz
		DR26H = 2,        //26Hz
		DR52H = 3,        //52Hz
		DR104H = 4,       //104Hz
		DR208H = 5,       //208Hz
		DR416H = 6,       //416Hz
		DR833H = 7,       //833Hz
		DR1_66KH = 8,     //1.66kHz
		DR3_33KH = 9,     //3.33kHz
		DR6_66KH = 10,    //6.66kHz
    };

    uint8_t _sens = 0;

#ifndef ARDUINO
    AccelerometerV2(I2C_HandleTypeDef *hi2c, uint8_t address = BASE_ADDRESS);
#else
    AccelerometerV2(TwoWire &hi2c, uint8_t address = BASE_ADDRESS);
#endif

    uint8_t Init();
    uint8_t Init(Scale sens);
    uint8_t Init(Scale sens, FilterBandwidth filter);
    uint8_t Init(Scale sens, FilterBandwidth filter, DataRate datarate);

    uint8_t SetScale(Scale sens = twoG);
    void SetFilter(FilterBandwidth filter = F400H);
    void SetDataRate(DataRate datarate);

    void end();

    int16_t RawX();
    int16_t RawY();
    int16_t RawZ();

    float X();
    float Y();
    float Z();

    float Temp();

    virtual ~AccelerometerV2();
};

} /* namespace IntroSatLib */

#endif /* ACCELEROMETER_H_ */
