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

	virtual RequestStatus RequestParams(
		uint16_t params,
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop
	) { return RequestStatus::Ok; }

	virtual void ResponceParams(
		uint16_t params,
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop,
		std::vector<uint8_t>& responce
	) { }

	virtual void ErrorParams(
		uint16_t params,
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop,
		std::vector<uint8_t>& responce
	) { }

public:
	uint8_t IsCurrent(
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop
	) const override
	{
		uint8_t commandByte = ByteConverter::ToUInt8(cpStart, cpStop);
		if (commandByte != UICommandByte || commandByte != UICommandByteBroadcast) { return 0; }
		HDLCPhysicsIterator commandParams = cpStart + 1;
		uint16_t countParams = std::distance(commandParams, cpStop);
		return countParams > 0 && IsCurrentParams(commandParams, cpStop);
	}

	RequestStatus Request(
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop
	) override
	{
		HDLCPhysicsIterator paramsStart = cpStart + 2;
		uint16_t countParams = std::distance(paramsStart, cpStop);
		return RequestParams(countParams, paramsStart, cpStop);
	}

	void Responce(
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop,
		std::vector<uint8_t>& responce
	) override
	{
		HDLCPhysicsIterator paramsStart = cpStart + 2;
		uint16_t countParams = std::distance(paramsStart, cpStop);
		responce.push_back(UICommandByte | 0);
		ResponceParams(countParams, paramsStart, cpStop, responce);
	}

	void Error(
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop,
		std::vector<uint8_t>& responce
	) override
	{
		HDLCPhysicsIterator paramsStart = cpStart + 2;
		uint16_t countParams = std::distance(paramsStart, cpStop);
		responce.push_back(UICommandByte | 0);
		ErrorParams(countParams, paramsStart, cpStop, responce);
	}
};

} /* namespace Holders */
} /* namespace HDLC */
} /* namespace IntroSatLib */

#endif /* HDLC_HOLDERS_UIHOLDER_H_ */
