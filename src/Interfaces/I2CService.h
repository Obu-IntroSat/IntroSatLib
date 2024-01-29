#ifndef _SERVICES_I2CSERVICE_H_
#define _SERVICES_I2CSERVICE_H_

#include "../Holders/Base/Types.h"
#include "../Holders/Base/IHolder.h"
#include <cinttypes>
#include <vector>
#include <functional>
namespace IntroSatLib
{
	namespace Interfaces
	{
		using Holders::Base::Error;

		using ReadCallback = const std::function<void(const std::vector<std::uint8_t>&)>;

		using WriteCallback = const std::function<void()>;

		class I2CService: public Holders::Base::IService
		{
		private:

			class I2CData
			{
			public:
				std::uint8_t _addr;
				std::uint8_t _mem;
				std::uint8_t _mode;
				std::vector<std::uint8_t> _data;
				ReadCallback _readCallBack;
				WriteCallback _writeCallBack;;
			public:

				I2CData(
					std::uint8_t addr,
					std::uint8_t mem,
					std::uint8_t size,
					ReadCallback callback
				):
					_addr(addr),
					_mem(mem),
					_mode(3),
					_readCallBack(callback)
				{
					_data.resize(size, 0);
				}

				I2CData(
					std::uint8_t addr,
					std::uint8_t mem,
					const std::vector<std::uint8_t>& data,
					WriteCallback callback
				):
					_addr(addr),
					_mem(mem),
					_mode(2),
					_data(data),
					_writeCallBack(callback)
				{ }

				I2CData(
					std::uint8_t addr,
					std::uint8_t size,
					ReadCallback callback
				):
					_addr(addr),
					_mem(0),
					_mode(1),
					_readCallBack(callback)
				{
					_data.resize(size, 0);
				}

				I2CData(
					std::uint8_t addr,
					const std::vector<std::uint8_t>& data,
					WriteCallback callback
				):
					_addr(addr),
					_mem(0),
					_mode(0),
					_data(data),
					_writeCallBack(callback)
				{ }

			};

			std::vector<std::shared_ptr<I2CData>> _list;
			Error UpdateData()
			{
				for (const std::shared_ptr<I2CData>& data : _list)
				{
					WriteI2CData(data.get());
				}
				_list.clear();
				return 0;
			}
			Error WriteI2CData(I2CData* i2cData)
			{
				std::uint8_t addr = i2cData->_addr;
				std::uint8_t mode = i2cData->_mode;

				std::uint8_t* data = i2cData->_data.data();
				std::uint8_t len = i2cData->_data.size();
				Error error = 0;
				switch (mode)
				{
				case 3:
					error = ForceReadMem(addr, i2cData->_mem, data, len);
					break;
				case 2:
					error = ForceWriteMem(addr, i2cData->_mem, data, len);
					break;
				case 1:
					error = ForceRead(addr, data, len);
					break;
				case 0:
					error = ForceWrite(addr, data, len);
					break;
				}

				if (!error)
				{
					mode & 1 ? i2cData->_readCallBack(i2cData->_data) : i2cData->_writeCallBack();
				}
				HAL_Delay(10);
				return 0;
			}
			Error Start(const Holders::Base::IHolder& holder) override
			{
				return UpdateData();
			}
			Error PreUpdate() override
			{
				return UpdateData();
			}
			Error Update() override
			{
				return UpdateData();
			}
			Error PostUpdate() override
			{
				return UpdateData();
			}
		public:
			virtual Error ForceWrite(std::uint8_t addr, std::uint8_t* data, std::uint8_t len) = 0;

			virtual Error ForceRead(std::uint8_t addr, std::uint8_t* data, std::uint8_t len) = 0;

			virtual Error ForceWriteMem(std::uint8_t addr, std::uint8_t reg, std::uint8_t* data, std::uint8_t len) = 0;

			virtual Error ForceReadMem(std::uint8_t addr, std::uint8_t reg, std::uint8_t* data, std::uint8_t len) = 0;

			void Write(std::uint8_t addr, const std::vector<std::uint8_t>& data, const WriteCallback& callback)
			{
				_list.push_back(std::make_shared<I2CData>(addr, data, callback));
			}

			void Read(std::uint8_t addr, std::uint8_t size, const ReadCallback& callback)
			{
				_list.push_back(std::make_shared<I2CData>(addr, size, callback));
			}

			void WriteMem(std::uint8_t addr, std::uint8_t mem, const std::vector<std::uint8_t>& data, const WriteCallback& callback)
			{
				_list.push_back(std::make_shared<I2CData>(addr, mem, data, callback));
			}

			void ReadMem(std::uint8_t addr, std::uint8_t mem, std::uint8_t size, const ReadCallback& callback)
			{
				_list.push_back(std::make_shared<I2CData>(addr, mem, size, callback));
			}
		};
	} /* namespace Services */
} /* namespace IntroSatLib */

#endif /* _SERVICES_I2CSERVICE_H_ */
