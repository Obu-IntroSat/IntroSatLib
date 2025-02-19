#ifndef MAGNETOMETER_V2_H_
#define MAGNETOMETER_V2_H_

#include "I2CDevice.h"
#include "BaseDevice.h"
#include "Quaternion/Quaternion.h"
#include <cmath>
#include <array>

namespace IntroSatLib
{

	class MagnetometerV2 : public BaseDevice
	{
	private:
		static const uint8_t BASE_ADDRESS = 0x1E;
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

		uint16_t _x = 0;
		uint16_t _y = 0;
		uint16_t _z = 0;

		uint8_t _sensitivity = 0;

	public:

		enum Scale 
		{
			G4 = 1,
			G8,
			G12,
			G16
		};	

#ifndef ARDUINO
		MagnetometerV2(I2C_HandleTypeDef *hi2c, uint8_t address = BASE_ADDRESS);
#else
		MagnetometerV2(TwoWire &hi2c, uint8_t address = BASE_ADDRESS);
		MagnetometerV2(uint8_t address = BASE_ADDRESS);
#endif

		MagnetometerV2(const MagnetometerV2 &other);
		MagnetometerV2 &operator=(const MagnetometerV2 &other);
		MagnetometerV2(MagnetometerV2 &&other);
		MagnetometerV2 &operator=(MagnetometerV2 &&other);

		uint8_t Init() override;
		uint8_t Init(Scale sensitivity);

		void SetScale(Scale sensitivity);

		void Read();

		int16_t RawX();
		int16_t RawY();
		int16_t RawZ();

		float X();
		float Y();
		float Z();
		
		Quaternion<float> GetQuaternion();

		~MagnetometerV2() override;
	};

} /* namespace IntroSatLib */

#endif /* MAGNETOMETER_V2_H_ */
