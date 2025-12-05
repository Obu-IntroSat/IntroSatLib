/*
 * LIS3MDL.h
 *
 *  Created on: Mar 18, 2025
 *      Author: unflesh
 */

#ifndef LIS3MDL_H_
#define LIS3MDL_H_

#include "Device/I2CDevice.h"
#include "Device/Base/BaseMagnetometer.h"

namespace IntroSatLib {

class LIS3MDL : public BaseMagnetometer, protected I2CDevice {
private:
	static const uint8_t BASE_ADDRESS = 0x1C;
	static constexpr float _rawg = 27386.0f / 4.0f; // Gauss

	enum RegisterMap
	{
		CTRL_REG1 = 0x20,
		CTRL_REG2,
		CTRL_REG3,
		CTRL_REG4,
		CTRL_REG5,
		STATUS_REG = 0x27,
		OUT_X_L,
		OUT_X_H,
		OUT_Y_L,
		OUT_Y_H,
		OUT_Z_L,
		OUT_Z_H,
	};

	uint16_t _mx = 0;
	uint16_t _my = 0;
	uint16_t _mz = 0;

	uint8_t _scale = 0;

public:

	enum Scale
	{
		G4 = 1,
		G8,
		G12,
		G16
	};

	LIS3MDL(const interfaces::I2C &i2c, uint8_t address = BASE_ADDRESS);

	ISL_StatusTypeDef Init(Scale scale, uint8_t force = 1);
	ISL_StatusTypeDef Init() override {
		return Init(Scale::G16);
	}

	ISL_StatusTypeDef SetScale(Scale scale);

	int16_t RawMX() override;
	int16_t RawMY() override;
	int16_t RawMZ() override;

	float MX() override;
	float MY() override;
	float MZ() override;

	~LIS3MDL();
};

} /* namespace IntroStratLib */

#endif /* LIS3MDL_H_ */
