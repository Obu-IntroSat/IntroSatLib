#ifndef DEVICE_H_
#define DEVICE_H_

#include "IntroSatLib_def.h"
//#include <stdint.h>

namespace IntroSatLib {
class Device {
	enum class DeviceState: uint8_t {
		Default,
		Initialized,
		Error,
		PowerDown
	};

private:
	DeviceState _state = DeviceState::Default;

protected:
	void setState(Device::DeviceState state);

public:
	virtual ISL_StatusTypeDef Init() = 0;
	Device::DeviceState GetState() {
		return _state;
	};
	virtual ISL_StatusTypeDef Enable() {
		return ISL_StatusTypeDef::ISL_OK;
	};
	virtual ISL_StatusTypeDef Disable() {
		return ISL_StatusTypeDef::ISL_OK;
	};
};
} /* namespace IntroSatLib */

#endif /* DEVICE_H_ */
