#ifndef HDLC_HOLDERS_DISCHOLDER_H_
#define HDLC_HOLDERS_DISCHOLDER_H_

#include "../Base/HDLCHolder.h"

namespace IntroSatLib {
namespace HDLC {
namespace Holders {

class DISCHolder: public IntroSatLib::HDLC::Base::HDLCHolder
{
private:
	static const uint8_t DISCCommandByte = 0x53;
	static const uint8_t DISCResponseByte = 0x73;


private:
	uint8_t _needStop = 0;
public:
	uint8_t NeedStop() const
	{
		return _needStop;
	}

public:
	uint8_t IsCurrent(
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop
	) const override { return *cpStart == DISCCommandByte; }

	RequestStatus Request(
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop
	) override {
		_needStop = 1;
		return RequestStatus::CantNextCode;
	}

	void Responce(
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop,
		std::vector<uint8_t>& responce
	) const override {
		uint8_t value = DISCResponseByte;
		responce.push_back(value);
	}
};

} /* namespace Holders */
} /* namespace HDLC */
} /* namespace IntroSatLib */



#endif /* HDLC_HOLDERS_DISCHOLDER_H_ */
