#ifndef HDLC_HOLDERS_TESTHOLDER_H_
#define HDLC_HOLDERS_TESTHOLDER_H_

#include "../Base/HDLCHolder.h"

namespace IntroSatLib {
namespace HDLC {
namespace Holders {

class TestHolder: public IntroSatLib::HDLC::Base::HDLCHolder
{
private:
	static const uint8_t TestCommandByte = 0xF3;
public:
	uint8_t IsCurrent(
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop
	) const override { return ByteConverter::ToUInt8(cpStart, cpStop) == TestCommandByte; }

	RequestStatus Request(
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop
	) override {
		return RequestStatus::CanResponce;
	}

	void Responce(
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop,
		std::vector<uint8_t>& responce
	) override
	{
		for(HDLCPhysicsIterator it = cpStart; it != cpStop; it++)
		{
			responce.push_back(*cpStart);
		}
	}
};

} /* namespace Holders */
} /* namespace HDLC */
} /* namespace IntroSatLib */

#endif /* HDLC_HOLDERS_TESTHOLDER_H_ */
