#define INTROSATLIB_INTERNAL
#include "Adapter/SPI.h"

#if defined(HAL_SPI_MODULE_ENABLED) && !defined(ARDUINO) // If HAL SPI is enabled and no Arduino framework

#include "Logger.h"

#define ASSERT_SPI_HAVE() \
if(!_hspi) { \
logText("No spi handle"); \
return ISL_StatusTypeDef::ISL_ERROR; \
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

ISL_StatusTypeDef IntroSatLib::interfaces::SPI::transfer(const uint8_t* out, uint8_t* in, uint8_t len)
 	{
		ASSERT_SPI_HAVE();
 		LOG_SPI("read/write");
 		LOG_SPI_BUFFER(", ", out, len);
 		logText(" ");
 		logNumber((uint8_t)len);
 		logText(" bytes > ");
 		ISL_StatusTypeDef result = logStatus(
 			(ISL_StatusTypeDef) HAL_SPI_TransmitReceive(_hspi, (uint8_t*) out, in, len, 1000)
 		);

 		LOG_SPI_BUFFER(", ", in, len);
 		logText("\n");
 		return result;
 	}
#endif /* defined(HAL_SPI_MODULE_ENABLED) && !defined(ARDUINO) */

