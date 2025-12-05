#include "./Logger.h"

#include "string.h"
#include <stdio.h>
#include "IntroSatLib_def.h"

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
ISL_StatusTypeDef logStatus(ISL_StatusTypeDef status)
{
	switch(status)
	{
	case ISL_StatusTypeDef::ISL_OK:
		logText("OK");
		break;
	case ISL_StatusTypeDef::ISL_ERROR:
		logText("ERROR");
		break;
	case ISL_StatusTypeDef::ISL_BUSY:
		logText("BUSY");
		break;
	case ISL_StatusTypeDef::ISL_TIMEOUT:
		logText("TIMEOUT");
		break;
	default:
		logText("NON_STATUS");
		break;
	}
	return status;
}
