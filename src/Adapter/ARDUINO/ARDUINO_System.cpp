#define INTROSATLIB_INTERNAL
#include "Adapter/System.h"

#ifdef ARDUINO
#include "Arduino.h"

uint32_t IntroSatLib::system::GetTick() {
	return millis();
}

void IntroSatLib::system::Delay(uint32_t milliseconds) {
	delay(milliseconds);
}

#endif /* ARDUINO */


