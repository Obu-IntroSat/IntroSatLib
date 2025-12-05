#ifndef I2CSCANNER_H_
#define I2CSCANNER_H_

#include "Adapter/I2C.h"
#include "Adapter/UART.h"
#include "stdio.h"
#include "string.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0')

namespace IntroSatLib {

class I2CScanner final {
public:
    static void scanI2CFull(interfaces::I2C i2c, interfaces::UART uart) {
		for (int i = 1; i<127; i++) {
			char tx_buf[30] = {0};
			sprintf(tx_buf, "Device at 0x%X: ", i);
			printTextToUART(uart, tx_buf);
			ISL_StatusTypeDef status = i2c.isReady(i);
			switch (status) {
			case ISL_OK:
				sprintf(tx_buf, "OK\n\r");
				break;
			case ISL_ERROR:
				sprintf(tx_buf, "ERROR\n\r");
				break;
			case ISL_BUSY:
				sprintf(tx_buf, "BUSY\n\r");
				break;
			case ISL_TIMEOUT:
				sprintf(tx_buf, "TIMEOUT\n\r");
				break;
			default:
				sprintf(tx_buf, "UNKNOWN\n\r");
				break;
			}
			printTextToUART(uart, tx_buf);
		}
	}
	
	static void scanI2C(interfaces::I2C i2c, interfaces::UART uart) {
		for (int i = 1; i < 127; i++) {
			char tx_buf[30] = { 0 };
			ISL_StatusTypeDef status = i2c.isReady(i);
			if (status == ISL_OK) {
				sprintf(tx_buf, "Device found at 0x%X\n\r", i);
				uart.transmit((uint8_t*) tx_buf, strlen(tx_buf));
			}
		}
	}

	static void scanDeviceRegisters(interfaces::I2C i2c, uint8_t addr, uint8_t firstRegister, uint8_t nRegisters, interfaces::UART uart) {
		char str[100];
		sprintf(str, "Scanning registers of device at 0x%0X. From 0x%0X to 0x%0X\n\r", addr, firstRegister, firstRegister+nRegisters);
		printTextToUART(uart, str);

		ISL_StatusTypeDef status = i2c.isReady(addr);
		if (status != ISL_OK) {
			printTextToUART(uart, "Device is not available!\n\r");
			return;
		}
		uint8_t val;
		printTextToUART(uart, "\t\t\tHEX\t         Binary\n\r");
		printTextToUART(uart, "\t\t\t\t7 | 6 | 5 | 4 | 3 | 2 | 1 | 0\n\r");
		for (int i = firstRegister; i < firstRegister + nRegisters; i++) {
			status = i2c.readMem(addr, i, &val, 1);
			if (status != ISL_OK) {
				sprintf(str, "\tReg 0x%02X: Error", i);
			} else {
				sprintf(str, "\t Reg 0x%02X: \t0x%02X\t        [" BYTE_TO_BINARY_PATTERN "]\n\r", i, val, BYTE_TO_BINARY(val));
			}

			printTextToUART(uart, str);
		}
		printTextToUART(uart, "Register scan Finished!\n\r");
	}

	static void scanDeviceRegistersBatch(interfaces::I2C i2c, uint8_t addr, uint8_t firstRegister, uint8_t nRegisters, interfaces::UART uart) {
			char str[100];
			sprintf(str, "Batch scanning registers of device at 0x%02X. From 0x%2X to 0x%02X\n\r", addr, firstRegister, firstRegister+nRegisters);
			printTextToUART(uart, str);

			ISL_StatusTypeDef status = i2c.isReady(addr);
			if (status != ISL_OK) {
				printTextToUART(uart, "Device is not available!\n\r");
				return;
			}
			uint8_t* val = new uint8_t[nRegisters];
			status = i2c.readMem(addr, firstRegister, val, nRegisters);
			if (status != ISL_OK) {
				printTextToUART(uart, "Can't read device registers!\n\r");
				return;
			}

			printTextToUART(uart, "\t\t\tHEX\t         Binary\n\r");
			printTextToUART(uart, "\t\t\t\t7 | 6 | 5 | 4 | 3 | 2 | 1 | 0\n\r");
			for (int i = 0; i < nRegisters; i++) {
				sprintf(str, "\t Reg 0x%02X: \t0x%02X\t        [" BYTE_TO_BINARY_PATTERN "]\n\r", i+firstRegister, val[i], BYTE_TO_BINARY(val[i]));
				printTextToUART(uart, str);
			}
			printTextToUART(uart, "Register scan finished!\n\r");
		}
private:
	static void printTextToUART(interfaces::UART uart, const char* str) {
			uart.transmit((uint8_t*)str, strlen(str));
		}
};
}
#endif /* I2CSCANNER_H_ */
