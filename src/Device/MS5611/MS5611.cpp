/*
 * MS5611.cpp
 *
 *  Created on: Mar 10, 2025
 *      Author: unflesh
 */

#include "MS5611.h"
#include "Adapter/System.h"

namespace IntroSatLib {

MS5611::MS5611(const interfaces::I2C &i2c, uint8_t address): I2CDevice(new interfaces::I2C(i2c), address) {
}

ISL_StatusTypeDef MS5611::Init(OSR sensitivity) {
	RETURN_STATUS_IF_NOT_OK_SILENT(IsReady())

	uint8_t tx_buf = CMD::RST;
	RETURN_STATUS_IF_NOT_OK_SILENT(WriteI2C(&tx_buf, 1));
	system::Delay(5);
	RETURN_STATUS_IF_NOT_OK_SILENT(SetSensitivity(sensitivity));
	return ReadPROM();
}

ISL_StatusTypeDef MS5611::ReadPROM() {
	for (uint8_t i = 0; i < 8; i++) {
		uint8_t buf[2];
		RETURN_STATUS_IF_NOT_OK_SILENT(ReadRegisterI2C(CMD::REQUEST_PROM + i * 0x02, buf, 2));
		_koeff_prom[i] = buf[0] << 8 | buf[1];
	}
	return (CalculateCRC() ^ (0x000F & _koeff_prom[CRC_4])) ? ISL_ERROR : ISL_OK;
}

uint8_t MS5611::CalculateCRC() {

	int cnt; /* simple counter */
	unsigned int n_rem; /* crc remainder */
	unsigned int crc_read; /* original value of the crc */
	unsigned char n_bit;
	n_rem = 0x00;
	crc_read = _koeff_prom[CRC_4]; //save read CRC
	_koeff_prom[7] = (0xFF00 & (_koeff_prom[CRC_4])); /* CRC byte is replaced by 0 */
	for (cnt = 0; cnt < 16; cnt++) { /* operation is performed on bytes */
		/* choose LSB or MSB */
		if (cnt % 2 == 1)
			n_rem ^= (unsigned short) ((_koeff_prom[cnt >> 1]) & 0x00FF); /* XOR – CRC calculating */
		else
			n_rem ^= (unsigned short) (_koeff_prom[cnt >> 1] >> 8);
		for (n_bit = 8; n_bit > 0; n_bit--) {
			if (n_rem & (0x8000)) {
				n_rem = (n_rem << 1) ^ 0x3000;
			} else {
				n_rem = (n_rem << 1);
			}
		}
	}
	n_rem = (0x000F & (n_rem >> 12)); /* final 4-bit reminder is CRC code */
	_koeff_prom[CRC_4] = crc_read; /* restore the crc_read to its original place */
	return (n_rem);
}

#if defined(DEBUG) && !defined(ARDUINO)
void MS5611::PROM_Print(UART_HandleTypeDef* uart) {
	char tx_buf[64];
	for (uint8_t i = 0; i < 8; i++) {
		sprintf(tx_buf, "%d\t", _koeff_prom[i]);
		HAL_UART_Transmit(uart, (uint8_t*)tx_buf, strlen(tx_buf), 100);
		for (uint8_t k = 0; k < 64; k++) {
			tx_buf[k] = '\0';
		}
	}
	HAL_UART_Transmit(uart, (uint8_t*)"\n", 1, 100);
}
#endif

ISL_StatusTypeDef MS5611::SetSensitivity(OSR sensitivity) {
	_sensitivity = sensitivity;
	return ISL_OK;
}

ISL_StatusTypeDef MS5611::ReadADC() {
	RETURN_STATUS_IF_NOT_OK_SILENT(WriteI2C(CMD::ADC_READ));
	system::Delay(10);
	return ISL_StatusTypeDef::ISL_OK;
}

ISL_StatusTypeDef MS5611::ReadRawTemperature() {
	RETURN_STATUS_IF_NOT_OK_SILENT(WriteI2C(CMD::REQUEST_TEMPERATURE | _sensitivity));
	system::Delay(30);
	RETURN_STATUS_IF_NOT_OK_SILENT(ReadADC())

	uint8_t buf[3];
	RETURN_STATUS_IF_NOT_OK_SILENT(ReadI2C(buf, 3));

	_raw_temperature = (((uint32_t) buf[0]) << 16) | (((uint32_t) buf[1]) << 8) | ((uint32_t) buf[2]);
	return ISL_OK;
}

float MS5611::GetTemperature() {
	int64_t T2 = 0;
	ReadRawTemperature();
	dT = (int32_t)(_raw_temperature) - (int32_t)((_koeff_prom[T_REF] << 8));
	TEMP = 2000 + (dT * (((int32_t)_koeff_prom[TEMPSENS]) / ((float)(((uint32_t)0b1) << 23))));
	if (TEMP < 2000) {
		T2 = ((int64_t)dT * dT) / ((float)(((uint32_t)0b1) << 31));
	}
	TEMP -= T2;
	_temperature = ((float)TEMP) / 100.0f;
	return _temperature;
}

ISL_StatusTypeDef MS5611::ReadRawPressure() {
	RETURN_STATUS_IF_NOT_OK_SILENT(WriteI2C(CMD::REQUEST_PRESSURE + _sensitivity));
	system::Delay(30);
	RETURN_STATUS_IF_NOT_OK_SILENT(ReadADC())

	uint8_t buf[3];
	RETURN_STATUS_IF_NOT_OK_SILENT(ReadI2C(buf, 3));

	_raw_pressure = (((uint32_t)buf[0]) << 16) | (((uint32_t)buf[1]) << 8) | buf[2];

	return ISL_OK;
}

float MS5611::GetPressure() {
	int64_t OFF2 = 0;
	int64_t SENS2 = 0;
	GetTemperature();
	ReadRawPressure();
	OFF = (((int64_t)_koeff_prom[OFF_T1]) << 16) + (dT * (((int64_t)_koeff_prom[TCO]) / ((float)(0b1 << 7))));
	SENS =  (((int64_t)_koeff_prom[SENS_T1]) << 15) + (dT * (((int64_t)_koeff_prom[TCS]) / ((float)( 0b1 << 8))));
	if (TEMP < 2000) {
		OFF2 = 5 * ((int64_t)(TEMP - 2000)) * (TEMP - 2000) / 2.0f;
		SENS2 = 5 * ((int64_t)(TEMP - 2000)) * (TEMP - 2000) / 4.0f;
	}
	if (TEMP < -1500) {
		OFF2 += 7 * ((int64_t)(TEMP + 1500))*(TEMP + 1500);
		SENS2 += 11 * ((int64_t)(TEMP + 1500))*(TEMP + 1500) / 2.0f;
	}

	OFF -= OFF2;
	SENS -= SENS2;
	P = ((_raw_pressure * (((int64_t)SENS) / ((float)(((uint32_t)0b1) << 21)))) - OFF) / ((float)(((uint32_t)0b1) << 15));
	_pressure = ((float)P) / 100.0f;
	return _pressure;
}

float MS5611::GetHeight() {
	return 44330.0f * (1.0f - pow((_pressure / 1013.25), 0.1903));
}

MS5611::~MS5611() {
}

} /* namespace IntroStratLib */

