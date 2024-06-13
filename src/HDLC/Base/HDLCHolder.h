#ifndef HDLCHOLDER_H_
#define HDLCHOLDER_H_

#include "inttypes.h"
#include "HDLCPhysics.h"
#include "../../Base/ByteConverter.h"

namespace IntroSatLib {
namespace HDLC {
namespace Base {

class HDLCHolder
{
public:
	enum class RequestStatus: uint8_t
	{
		Ok = 0,
		ErrorCode = 1,
		CanResponce = 0x40,
		CantNextCode = 0x80
	};
protected:
	using HDLCPhysicsIterator = HDLCPhysics::iterator;
	using ByteConverter = IntroSatLib::Base::ByteConverterLittleEndian;

public:
	virtual uint8_t IsCurrent(
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop
	) const { return 0; }

	virtual RequestStatus Request(
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop
	) { return RequestStatus::Ok; }

	virtual void Responce(
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop,
		std::vector<uint8_t>& responce
	) { }

	virtual void Error(
		HDLCPhysicsIterator cpStart,
		HDLCPhysicsIterator cpStop,
		std::vector<uint8_t>& responce
	) { }

	virtual ~HDLCHolder() = default;
};

HDLCHolder::RequestStatus operator &(HDLCHolder::RequestStatus left, HDLCHolder::RequestStatus rigth)
{
	return static_cast<HDLCHolder::RequestStatus>(static_cast<uint8_t>(left) & static_cast<uint8_t>(left));
}

HDLCHolder::RequestStatus operator |(HDLCHolder::RequestStatus left, HDLCHolder::RequestStatus rigth)
{
	return static_cast<HDLCHolder::RequestStatus>(static_cast<uint8_t>(left) | static_cast<uint8_t>(left));
}

}
}
}

#endif /* HDLCHOLDER_H_ */
