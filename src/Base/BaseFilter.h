#ifndef FILTERS_BASEFILTER_H_
#define FILTERS_BASEFILTER_H_

#include <stdint.h>

namespace IntroSatLib
{
	namespace Base
	{
		template<typename T>
		class BaseFilter
		{
			public:
				virtual uint8_t Size() const;
				virtual T AddValue(T newValue);
				virtual T operator[](uint8_t index) const;
		};

	}
}

#endif /* FILTERS_BASEFILTER_H_ */
