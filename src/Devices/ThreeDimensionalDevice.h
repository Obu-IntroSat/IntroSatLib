#ifndef DEVICES_THREEDIMENSIONALDEVICE_H_
#define DEVICES_THREEDIMENSIONALDEVICE_H_

#include <stdint.h>

#include "../Base/Device.h"

#include "../Filters/LinearFilter.h"
#include "../Filters/ScalableFilter.h"

namespace IntroSatLib
{
	namespace Devices
	{

		template<typename T, typename TRaw>
		class ThreeDimensionalDevice: public IntroSatLib::Base::Device
		{

			private:

				IntroSatLib::Filters::ScalableFilter<TRaw> _x;
				IntroSatLib::Filters::ScalableFilter<TRaw> _y;
				IntroSatLib::Filters::ScalableFilter<TRaw> _z;

				IntroSatLib::Filters::ScalableFilterValue<TRaw> RawX() const;
				IntroSatLib::Filters::ScalableFilterValue<TRaw> RawY() const;
				IntroSatLib::Filters::ScalableFilterValue<TRaw> RawZ() const;

			protected:

				ThreeDimensionalDevice(
						std::unique_ptr<IntroSatLib::Base::Interface> interface,
						IntroSatLib::Base::DeviceID id,
						int8_t maxScale,
						uint8_t size);

				ThreeDimensionalDevice(
						std::unique_ptr<IntroSatLib::Base::Interface> interface,
						IntroSatLib::Base::DeviceID id,
						int8_t minScale,
						int8_t maxScale,
						uint8_t size);

				virtual IntroSatLib::Base::Status InnerUpdateData(TRaw& x, TRaw& y, TRaw& z) const;

				virtual T ConvertValue(IntroSatLib::Filters::ScalableFilterValue<TRaw> value) const;

				virtual void UpScale();
				virtual void DownScale();

			public:

				IntroSatLib::Base::Status UpdateData() override final;

				T X() const;
				T Y() const;
				T Z() const;
		};

	}
}

#endif /* DEVICES_THREEDIMENSIONALDEVICE_H_ */
