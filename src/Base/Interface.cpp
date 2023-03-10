#include "./Interface.h"

namespace IntroSatLib
{
	namespace Base
	{
		void Interface::SetRegister(uint8_t Register, uint8_t value, Status* status)
		{
			if (*status != Status::Ok)
			{
				return;
			}

			Status innerStatus = Write(Register, &value, 1);

			if (innerStatus != Status::Ok)
			{
				*status |= innerStatus;
			}

		}

		uint8_t Interface::GetRegister(uint8_t Register, Status* status)
		{
			if (*status != Status::Ok)
			{
				return 0;
			}

			uint8_t buf = 0;
			Status innerStatus = Read(Register, &buf, 1);

			if (innerStatus != Status::Ok)
			{
				*status |= innerStatus;
				return 0;
			}
			return buf;
		}
	}
}

