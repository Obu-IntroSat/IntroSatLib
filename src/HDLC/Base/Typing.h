#ifndef HDLC_BASE_TYPING_H_
#define HDLC_BASE_TYPING_H_


#include <inttypes.h>
#include "../../Base/ByteConverter.h"
#include "../../Base/Collections/BufferArray.h"
#include "../../Base/Memory/static_ptr.h"

namespace IntroSatLib {
namespace HDLC {
namespace Base {

const uint8_t MaxBufferLength = 255;

const uint8_t MinBufferSize = 5;

const uint16_t MaxTimeout = 100;

using BufferType = IntroSatLib::Base::Collections::BufferArray<uint8_t, MaxBufferLength>;

using iterator = BufferType::const_iterator;
using value_type = BufferType::value_type;
using size_type = BufferType::size_type;

template<typename T>
using static_ptr = IntroSatLib::Base::Memory::static_ptr<T>;

} /* namespace Base */
} /* namespace HDLC */
} /* namespace IntroSatLib */


#endif /* HDLC_BASE_TYPING_H_ */
