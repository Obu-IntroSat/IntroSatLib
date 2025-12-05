/*
 * LSM6DS3.h
 *
 *  Created on: Mar 17, 2025
 *      Author: Goldfor / vlafes / unflesh
 */

#ifndef LSM6DS3_H_
#define LSM6DS3_H_

#include "Device/I2CDevice.h"
#include "Device/Base/BaseGyroscope.h"
#include "Device/Base/BaseAccelerometer.h"

namespace IntroSatLib {

class LSM6DS3: public BaseGyroscope, public BaseAccelerometer, protected I2CDevice {
public:
		enum DataRateGyro {
		OFF_G = 0,
		DR_G_13_Hz,			/**< Частота обновления 13 Гц */
		DR_G_26_Hz,			/**< Частота обновления 26 Гц */
		DR_G_52_Hz,			/**< Частота обновления 52 Гц */
		DR_G_104_Hz, 		/**< Частота обновления 104 Гц (значение по умолчаиню) */
		DR_G_208_Hz,		/**< Частота обновления 208 Гц */
		DR_G_416_Hz,		/**< Частота обновления 416 Гц */
		DR_G_833_Hz,		/**< Частота обновления 833 Гц */
		DR_G_1660_Hz,		/**< Частота обновления 1660 Гц */
	};

	enum DataRateAccel {
		OFF_A = 0,
		DR_A_12_5_Hz,      	/**< Частота обновления 12.5 Гц */
		DR_A_26_Hz,        	/**< Частота обновления 26 Гц */
		DR_A_52_Hz,        	/**< Частота обновления 52 Гц */
		DR_A_104_Hz,       	/**< Частота обновления 104 Гц (значение по умолчанию) */
		DR_A_208_Hz,       	/**< Частота обновления 208 Гц */
		DR_A_416_Hz,       	/**< Частота обновления 416 Гц */
		DR_A_833_Hz,       	/**< Частота обновления 833 Гц */
		DR_A_1660_Hz,     	/**< Частота обновления 1660 Гц */
		DR_A_3330_Hz,     	/**< Частота обновления 3330 Гц */
		DR_A_6660_Hz,    	/**< Частота обновления 6660 Гц */
    };

	enum ScaleGyro {
		DPS_245 = 0,		/**< Диапазон &plusmn;245 град/с (значение по умолчанию) */
		DPS_500, 			/**< Диапазон &plusmn;500 град/с */
		DPS_1000,			/**< Диапазон &plusmn;1000 град/с */
		DPS_2000			/**< Диапазон &plusmn;2000 град/с */
	};

	enum ScaleAccel {
		twoG	 = 0,		/**< Диапазон &plusmn;2g */
		fourG	 = 2,		/**< Диапазон &plusmn;4g (значение по умолчанию) */
		eightG	 = 3,		/**< Диапазон &plusmn;8g */
		sixteenG = 1		/**< Диапазон &plusmn;16g */
	};

	enum FilterBandwidthAccel
	{
		FB_A_400_Hz = 0, 	/**< Частота 400 Гц (значение по умолчанию) */
		FB_A_200_Hz,	 	/**< Частота 200 Гц */
		FB_A_100_Hz, 		/**< Частота 100 Гц */
		FB_A_50_Hz  		/**< Частота 50 Гц */
	};

private:

	static const uint8_t BASE_ADDRESS = 0x6B;
	static constexpr float _rawrps = (8.75f / 1000.f) * 3.1415926f / 180.0f; // radians per second
	static constexpr float _rawdps = (8.75f / 1000.f); // degrees per second
	static constexpr float _rawg = (0.061f / 1000.f); // fraction of 'g'

	constexpr uint8_t scaleAccelToShift(ScaleAccel scaleAccel) {
		switch (scaleAccel)
		{
			case ScaleAccel::twoG:
				return 0;
			case ScaleAccel::fourG:
				return 1;
			case ScaleAccel::eightG:
				return 2;
			case ScaleAccel::sixteenG:
				return 3;
			default:
				return 0;
	  	}
	}

	enum RegisterMap {
		GYRO_CONFIG = 0X11,
		GYRO_XOUT_H = 0X22,
		GYRO_XOUT_L,
		GYRO_YOUT_H,
		GYRO_YOUT_L,
		GYRO_ZOUT_H,
		GYRO_ZOUT_L,
		WHO_AM_I_REG = 0X0F,
		CTRL1_XL = 0X10,
		CTRL2_G = 0X11,
		CTRL8_XL = 0X17,
		CTRL9_XL,
		CTRL10_C,
		OUT_TEMP_L = 0X20,
		OUT_TEMP_H = 0X21,
		OUTX_L_XL = 0X28,
		OUTX_H_XL = 0X29,
		OUTY_L_XL = 0X2A,
		OUTY_H_XL = 0X2B,
		OUTZ_L_XL = 0X2C,
		OUTZ_H_XL = 0X2D,
	};

	static float cutMin(float value, float cut);

	ISL_StatusTypeDef WhoAmI();

	
	ScaleAccel _scaleAccel = fourG;
	DataRateAccel _dataRateAccel = DR_A_104_Hz;
	FilterBandwidthAccel _filterBandwidthAccel = FB_A_400_Hz;

	ScaleGyro _scaleGyro = DPS_245;
	DataRateGyro _dataRateGyro = DR_G_104_Hz;
	

//	float _cutX = 0.0872665; // rps
//	float _cutY = 0.0872665;
//	float _cutZ = 0.0872665;

	float _cutX = 5.0f; // dps
	float _cutY = 5.0f;
	float _cutZ = 5.0f;

public:
	LSM6DS3(const interfaces::I2C &i2c, uint8_t address = BASE_ADDRESS);


	ISL_StatusTypeDef InitGyro();
	ISL_StatusTypeDef InitGyro(ScaleGyro scaleGyro);
	ISL_StatusTypeDef InitGyro(ScaleGyro scaleGyro, DataRateGyro dataRateGyro);
	ISL_StatusTypeDef InitAccel();
	ISL_StatusTypeDef InitAccel(ScaleAccel scaleAccel);
	ISL_StatusTypeDef InitAccel(ScaleAccel scaleAccel, DataRateAccel dataRateAccel);
	ISL_StatusTypeDef InitAccel(ScaleAccel scaleAccel, DataRateAccel dataRateAccel, FilterBandwidthAccel filter);

	ISL_StatusTypeDef Init() override {
		RETURN_STATUS_IF_NOT_OK_SILENT(InitGyro());
		RETURN_STATUS_IF_NOT_OK_SILENT(InitAccel());
		return ISL_OK;
	}

	ISL_StatusTypeDef SetScaleGyro(ScaleGyro scaleGyro);
	ISL_StatusTypeDef SetScaleAccel(ScaleAccel scaleAccel);
	ISL_StatusTypeDef SetDataRateGyro(DataRateGyro dataRateGyro);
	ISL_StatusTypeDef SetDataRateAccel(DataRateAccel dataRateAccel);
	ISL_StatusTypeDef SetFilterAccel(FilterBandwidthAccel filterBandwithAccel);

public:
	int16_t RawGX() override;
	int16_t RawGY() override;
	int16_t RawGZ() override;

	float GX() override;
	float GY() override;
	float GZ() override;

	int16_t RawAX() override;
	int16_t RawAY() override;
	int16_t RawAZ() override;

	float AX() override;
	float AY() override;
	float AZ() override;

	void SetMinCutX(float x);
	void SetMinCutY(float y);
	void SetMinCutZ(float z);

    float Temp();

    ISL_StatusTypeDef DeinitAccel();
    ISL_StatusTypeDef DeinitGyro();
    ISL_StatusTypeDef Deinit();

	~LSM6DS3();
};

} /* namespace IntroSatLib */

#endif /* LSM6DS3_H_ */
