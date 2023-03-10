#include "ThreeDimensionalDevice.h"

namespace IntroSatLib
{
	namespace Devices
	{
		template<typename T, typename TRaw>
		ThreeDimensionalDevice<T, TRaw>::ThreeDimensionalDevice(
			std::unique_ptr<IntroSatLib::Base::Interface> interface,
			IntroSatLib::Base::DeviceID id,
			int8_t maxScale,
			uint8_t size):
			IntroSatLib::Base::Device(interface, id | IntroSatLib::Base::DeviceID::ThreeDimensional),
			_x(IntroSatLib::Filters::LinearFilter<IntroSatLib::Filters::ScalableFilterValue<TRaw>>(size),
					maxScale),
			_y(IntroSatLib::Filters::LinearFilter<IntroSatLib::Filters::ScalableFilterValue<TRaw>>(size),
					maxScale),
			_z(IntroSatLib::Filters::LinearFilter<IntroSatLib::Filters::ScalableFilterValue<TRaw>>(size),
					maxScale) { }

		template<typename T, typename TRaw>
		ThreeDimensionalDevice<T, TRaw>::ThreeDimensionalDevice(
			std::unique_ptr<IntroSatLib::Base::Interface> interface,
			IntroSatLib::Base::DeviceID id,
			int8_t minScale,
			int8_t maxScale,
			uint8_t size):
			IntroSatLib::Base::Device(interface, id | IntroSatLib::Base::DeviceID::ThreeDimensional),
			_x(IntroSatLib::Filters::LinearFilter<IntroSatLib::Filters::ScalableFilterValue<TRaw>>(size),
					minScale,
					maxScale),
			_y(IntroSatLib::Filters::LinearFilter<IntroSatLib::Filters::ScalableFilterValue<TRaw>>(size),
					minScale,
					maxScale),
			_z(IntroSatLib::Filters::LinearFilter<IntroSatLib::Filters::ScalableFilterValue<TRaw>>(size),
					minScale,
					maxScale) { }

		template<typename T, typename TRaw>
		IntroSatLib::Base::Status ThreeDimensionalDevice<T, TRaw>::UpdateData()
		{
			TRaw x, y, z;

			IntroSatLib::Base::Status result = InnerUpdateData(x, y, z);

			if (result != IntroSatLib::Base::Status::Ok)
			{
				return result;
			}

			_x->AddValue(x);
			_y->AddValue(y);
			_z->AddValue(z);

			if (_x->NeedUpScale() || _y->NeedUpScale() || _z->NeedUpScale())
			{
				_x->UpScale();
				_y->UpScale();
				_z->UpScale();
				UpScale();
			}

			if (_x->CanDownScale() && _y->CanDownScale() && _z->CanDownScale())
			{
				_x->DownScale();
				_y->DownScale();
				_z.DownScale();
				DownScale();
			}

			return IntroSatLib::Base::Status::Ok;

		}


		template<typename T, typename TRaw>
		IntroSatLib::Filters::ScalableFilterValue<TRaw> ThreeDimensionalDevice<T, TRaw>::RawX() const
		{
			return _x.ScaledValue();
		}

		template<typename T, typename TRaw>
		IntroSatLib::Filters::ScalableFilterValue<TRaw> ThreeDimensionalDevice<T, TRaw>::RawY() const
		{
			return _y.ScaledValue();
		}

		template<typename T, typename TRaw>
		IntroSatLib::Filters::ScalableFilterValue<TRaw> ThreeDimensionalDevice<T, TRaw>::RawZ() const
		{
			return _z.ScaledValue();
		}

		template<typename T, typename TRaw>
		T ThreeDimensionalDevice<T, TRaw>::X() const
		{
			return ConvertValue(RawX());
		}

		template<typename T, typename TRaw>
		T ThreeDimensionalDevice<T, TRaw>::Y() const
		{
			return ConvertValue(RawY());
		}

		template<typename T, typename TRaw>
		T ThreeDimensionalDevice<T, TRaw>::Z() const
		{
			return ConvertValue(RawZ());
		}
	}
}
