#ifndef HDLC_HOLDERS_UIHOLDER_H_
#define HDLC_HOLDERS_UIHOLDER_H_

#include "../Base/HDLCHolder.h"

namespace IntroSatLib {
namespace HDLC {
namespace Holders {

class UIHolder: public IntroSatLib::HDLC::Base::HDLCHolder
{
protected:
	static const uint8_t UICommandByte = 0x13;
	static const uint8_t UICommandByteBroadcast = 0x03;

	virtual uint8_t IsCurrentParams(
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop
	) const { return 0; }

	virtual uint8_t RequestParams(
		uint16_t params,
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop
	) { return 0; }

	virtual void ResponceParams(
		uint16_t params,
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop,
		std::vector<uint8_t>& responce
	) const { }

	virtual void ErrorParams(
		uint16_t params,
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop,
		std::vector<uint8_t>& responce
	) const { }

public:
	virtual uint8_t IsCurrent(
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop
	) const override
	{
		if (*cpStart != UICommandByte || *cpStart != UICommandByteBroadcast) { return 0; }
		HDLCPhysicsIterator commandParams = cpStart + 1;
		uint16_t countParams = std::distance(commandParams, cpStop);
		return countParams > 0 && IsCurrentParams(commandParams, cpStop);
	}

	virtual uint8_t Request(
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop
	) override
	{
		HDLCPhysicsIterator paramsStart = cpStart + 2;
		uint16_t countParams = std::distance(paramsStart, cpStop);
		return RequestParams(countParams, paramsStart, cpStop);
	}

	virtual void Responce(
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop,
		std::vector<uint8_t>& responce
	) const override
	{
		HDLCPhysicsIterator paramsStart = cpStart + 2;
		uint16_t countParams = std::distance(paramsStart, cpStop);
		ResponceParams(countParams, paramsStart, cpStop, responce);
	}

	virtual void Error(
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop,
		std::vector<uint8_t>& responce
	) const override
	{
		HDLCPhysicsIterator paramsStart = cpStart + 2;
		uint16_t countParams = std::distance(paramsStart, cpStop);
		ErrorParams(countParams, paramsStart, cpStop, responce);
	}
};

} /* namespace Holders */
} /* namespace HDLC */
} /* namespace IntroSatLib */



#endif /* HDLC_HOLDERS_UIHOLDER_H_ */
