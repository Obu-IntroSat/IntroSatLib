#include "./Logger.h"

#include "string.h"
#include <stdio.h>

void logText(const char *text)
{
	uint8_t len = strlen(text);
	if (LOGGER) LOGGER(text, len);
}
void logHEX(uint8_t val)
{
	char buf[5];
	sprintf(buf, "0x%.2X", val);
	logText(buf);
}

void logNumber(uint8_t val)
{
	char buf[4];
	sprintf(buf, "%d", val);
	logText(buf);
}
HAL_StatusTypeDef logStatus(HAL_StatusTypeDef status)
{
	switch(status)
	{
	case HAL_OK:
		logText("HAL_OK");
		break;
	case HAL_ERROR:
		logText("HAL_ERROR");
		break;
	case HAL_BUSY:
		logText("HAL_BUSY");
		break;
	case HAL_TIMEOUT:
		logText("HAL_TIMEOUT");
		break;
	default:
		logText("NON_STATUS");
		break;
	}
	return status;
}
