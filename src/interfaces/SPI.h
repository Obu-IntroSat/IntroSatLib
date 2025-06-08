#ifndef INTERFACES_SPI_H_
#define INTERFACES_SPI_H_


#ifdef ARDUINO
	#include "Arduino.h"
	#include "SPI.h"
#else
	#if __has_include ("stm32f4xx_hal.h")
		#include "stm32f4xx_hal.h"
		#include "stm32f4xx_hal_spi.h"
	#endif

	#if __has_include ("stm32f1xx_hal.h")
		#include "stm32f1xx_hal.h"
		#include "stm32f1xx_hal_spi.h"
	#endif
#endif

#include <array>
#include "../Logger.h"

#define ASSERT_SPI_HAVE() \
if(!_hspi) { \
	logText("Haven't spi handle"); \
	return HAL_StatusTypeDef::HAL_ERROR; \
}

#define LOG_SPI(mode) \
logText("SPI "); \
logText(mode);

#if LOGDATA
#define LOG_SPI_BUFFER(Sep, Data, Nbytes) { \
logText(" - "); \
for(uint8_t i = 0; i < Nbytes; i++) { \
	logHEX(Data[i]); \
	if (i != (Nbytes - 1)) logText(Sep); \
} \
}

#else
#define LOG_SPI_BUFFER(Sep, Data, Nbytes)
#endif

namespace IntroSatLib {
namespace intefaces {

class SPI final {
	SPI_HandleTypeDef *_hspi = 0;
public:
	SPI(SPI_HandleTypeDef *hspi): _hspi(hspi) { };

	template<size_t N>
	HAL_StatusTypeDef transfer(const std::array<uint8_t, N> out, std::array<uint8_t, N> in)
	{
		return transfer(out.data(), in.data(), N);
	}
	template<size_t N>
	HAL_StatusTypeDef transfer(const std::array<uint8_t, N> out, uint8_t* in)
	{
		return transfer(out.data(), in, N);
	}

	template<size_t N>
	HAL_StatusTypeDef transfer(const uint8_t* out, std::array<uint8_t, N> in)
	{
		return transfer(out, in.data(), N);
	}

	HAL_StatusTypeDef transfer(const uint8_t* out, uint8_t* in, uint8_t len)
	{
		LOG_SPI("read/write");
		LOG_SPI_BUFFER(", ", out, len);
		logText(" ");
		logNumber((uint8_t)len);
		logText(" bytes > ");
		HAL_StatusTypeDef result = logStatus(
			HAL_SPI_TransmitReceive(_hspi, (uint8_t*) out, in, len, 1000)
		);

		LOG_SPI_BUFFER(", ", in, len);
		logText("\n");
		return result;
	}
};

} /* namespace intefaces */
} /* namespace IntroSatLib */

#endif /* INTERFACES_SPI_H_ */
