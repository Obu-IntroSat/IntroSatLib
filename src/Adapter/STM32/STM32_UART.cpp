#define INTROSATLIB_INTERNAL
#include "Adapter/UART.h"

#if defined(HAL_UART_MODULE_ENABLED) && !defined(ARDUINO) // If HAL SPI is enabled and no Arduino framework

uint8_t IntroSatLib::interfaces::UART::available() {
    return __HAL_UART_GET_FLAG(_huart, UART_FLAG_RXNE); // TODO implement for HAL??
}

ISL_StatusTypeDef IntroSatLib::interfaces::UART::receive(uint8_t* rx_buf, uint16_t count, uint16_t timeout) {
	return (ISL_StatusTypeDef) HAL_UART_Receive(_huart, rx_buf, count, timeout);
}

ISL_StatusTypeDef IntroSatLib::interfaces::UART::transmit(uint8_t* tx_buf, uint16_t count, uint16_t timeout) {
    return (ISL_StatusTypeDef) HAL_UART_Transmit(_huart, tx_buf, count, timeout);
}

ISL_StatusTypeDef IntroSatLib::interfaces::UART::transmitAsync(uint8_t* tx_buf, uint16_t count) {
    return (ISL_StatusTypeDef) HAL_UART_Transmit_IT(_huart, tx_buf, count);
}

#endif /* defined(HAL_UART_MODULE_ENABLED) && !defined(ARDUINO) */
