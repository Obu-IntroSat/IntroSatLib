#ifndef HDLCHOLDER_H_
#define HDLCHOLDER_H_

#include "inttypes.h"
#include "HDLCPhysics.h"

namespace IntroSatLib {
namespace HDLC {
namespace Base {

class HDLCHolder
{
public:
	static const uint8_t CantNextCode = 0x80;
protected:
	using HDLCPhysicsIterator = HDLCPhysics::iterator;

public:
	virtual uint8_t IsCurrent(
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop
	) const { return 0; }

	virtual uint8_t Request(
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop
	) { return 0; }

	virtual void Responce(
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop,
		std::vector<uint8_t>& responce
	) const { }

	virtual void Error(
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop,
		std::vector<uint8_t>& responce
	) const { }

	virtual ~HDLCHolder() = default;
};

}
}
}

#endif /* HDLCHOLDER_H_ */
