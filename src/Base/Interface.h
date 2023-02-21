#ifndef BASE_INTERFACE_H_
#define BASE_INTERFACE_H_

#include <stdint.h>
#include "Status.h"

namespace IntroSatLib
{
	namespace Base
	{
		class Interface
		{
			public:
				virtual Status IsReady();
				virtual Status Read(uint8_t* Data, uint8_t Nbytes);
				virtual Status Read(uint8_t Register, uint8_t* Data, uint8_t Nbytes);
				virtual Status Write(uint8_t* Data, uint8_t Nbytes);
				virtual Status Write(uint8_t Register, uint8_t* Data, uint8_t Nbytes);
		};
	}
}


#endif /* BASE_INTERFACE_H_ */
