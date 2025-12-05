/*
 * MS5611.h
 *
 *  Created on: Mar 6, 2025
 *      Author: unflesh
 */

#ifndef MS5611_H_
#define MS5611_H_

#include "Device/Base/BaseBarometer.h"
#include "Device/Base/BaseTemperatureSensor.h"
#include "Device/I2CDevice.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "math.h"

namespace IntroSatLib {

class MS5611 : public BaseBarometer, protected I2CDevice {

public:

	/* Add OSR to the command REQUEST_PRESSURE or REQUEST_TEMPERATURE */
	enum OSR {
		OSR_256 = 0x00,
		OSR_512 = 0x02,
		OSR_1024 = 0x04,
		OSR_2048 = 0x06,
		OSR_4096 = 0x08
	};

private:
	enum ERROR_TYPE: uint8_t {
		OK = ISL_OK,
		ERROR = ISL_ERROR,
		BUSY = ISL_BUSY,
		TIMEOUT = ISL_TIMEOUT,
		CRC_ERROR = 0x05,
		ADC_ERROR = 0x06
	};

	enum CMD: uint8_t {
		RST = 0x1E,
		REQUEST_PROM = 0xA0,
		REQUEST_PRESSURE = 0x40,
		REQUEST_TEMPERATURE = 0x50,
		ADC_READ = 0x00
	};

	/* Use PROM_N enum as an index for _koeff_prom array */
	enum  PROM_N: uint8_t {
		RSRV = 0, /* reserved */
		SENS_T1, /* pressure sensitivity */
		OFF_T1, /* pressure offset */
		TCS, /* temperature coefficient of pressure sensitivity */
		TCO, /* temperature coefficient of pressure offset */
		T_REF, /* reference temperature */
		TEMPSENS, /* temperature coefficient of the temperature */
		CRC_4 /* CRC-4 */
	};

	static const uint8_t BASE_ADDRESS = 0x77; /* another available address is 0x76 */
	static constexpr float P_SEA_LEVEL = 1013.25f; // mbar

	OSR _sensitivity = OSR::OSR_256;

	uint16_t _koeff_prom[8]; /* PROM content array */
	uint8_t _crc;

	int32_t dT;
	int32_t TEMP;
	int64_t OFF;
	int64_t SENS;
	int64_t P;
	uint32_t _raw_pressure;
	uint32_t _raw_temperature;
	float _pressure;
	float _temperature;
	ISL_StatusTypeDef ReadRawPressure();
	ISL_StatusTypeDef ReadRawTemperature();
	ISL_StatusTypeDef ReadADC();
	ISL_StatusTypeDef ReadPROM();

public:


	MS5611(const interfaces::I2C &i2c, uint8_t address = BASE_ADDRESS);

#if defined(DEBUG) && !defined(ARDUINO)
	void PROM_Print(UART_HandleTypeDef* uart);
#endif

	ISL_StatusTypeDef Init(OSR sensitivity);
	ISL_StatusTypeDef Init() override {
		return Init(OSR::OSR_1024);
	}

	uint8_t CalculateCRC();

	ISL_StatusTypeDef SetSensitivity(OSR sensitivity);

	float GetPressure() override; // TODO: if error return NaN
	float GetTemperature();
	float GetHeight();

	~MS5611();
};

} /* namespace IntroStratLib */

#endif /* MS5611_H_ */
