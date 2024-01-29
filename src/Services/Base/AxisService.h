#ifndef _SERVICES_BASE_AXISSERVICE_H_
#define _SERVICES_BASE_AXISSERVICE_H_

#include "../../Base/FloatType.h"
#include "../../Holders/Base/Types.h"
#include "../../Holders/Base/IHolder.h"
#include "../../Interfaces/I2CService.h"
#include <cinttypes>
#include <vector>
#include <array>
#include <functional>

namespace IntroSatLib
{
	namespace Services
	{
		namespace Base
		{
			using Holders::Base::Error;
			using IntroSatLib::Base::Float;

			template<std::size_t NInputs>
			class AxisService: public Holders::Base::IService
			{
			private:
				std::array<IntroSatLib::Base::Float, NInputs> _data = {0};
				std::int8_t _scale = 0;
				std::int8_t _nextScale = 0;

			public:
				Float Axi(uint8_t index)
				{
					return _data[index];
				}
				const std::array<IntroSatLib::Base::Float, NInputs>& Axis()
				{
					return _data;
				}

			protected:
				constexpr std::int8_t GetScale() const
				{
					return _scale;
				}

			protected:
				std::int8_t _minScale = 0;
				std::vector<IntroSatLib::Base::Float> _scaleConst;

			protected:

				virtual Error Init(const Holders::Base::IHolder& holder) { return 0; }

				virtual void ReadAxi(std::uint8_t index) { }

				virtual Error SetScaleReal(std::int8_t scale) { return 0; }

				void SetData(Float data, std::uint8_t index)
				{
					char res[80] = {0};
					sprintf(res, "Calculated value %s %d %f\n", GetId().data(), index, data);
					HAL_UART_Transmit(&huart1, (uint8_t *)res, strlen(res), 1000);
					_data[index] = data;
					SetNextScale(CalculateNextScale(data));
				}

			private:
				constexpr std::int8_t CalculateNextScale(Float data) const
				{
					uint8_t size = _scaleConst.size();
					int8_t currentScale = _minScale;
					for (std::uint8_t i = 0; i < size; i++)
					{
						if (data < _scaleConst[i])
						{
							break;
						}
						currentScale++;
					}
					return currentScale;
				}

				void SetNextScale(std::int8_t nextScale)
				{
					if (nextScale > _nextScale)
					{
						_nextScale = nextScale;
					}
				}

				Error SetScale(std::int8_t scale)
				{
					char res[80] = {0};
					sprintf(res, "SetScale %s\n", GetId().data());
					HAL_UART_Transmit(&huart1, (uint8_t *)res, strlen(res), 1000);
					Error result = SetScaleReal(scale);
					_scale = scale;
					return result;
				}

			private:
				Error Start(const Holders::Base::IHolder& holder) override final
				{
					Init(holder);
					SetScale(_minScale);
					return 0;
				}

				Error PreUpdate() override final
				{
					for (std::uint8_t i = 0; i < NInputs; i++)
					{
						ReadAxi(i);
					}
					return 0;
				}

				Error Update() override final
				{
					return 0;
				}

				Error PostUpdate() override final
				{
					return SetScale(_nextScale);
				}
			};
		} /* namespace Base */
	} /* namespace Services */
} /* namespace IntroSatLib */

#endif /* _SERVICES_BASE_AXISSERVICE_H_ */
