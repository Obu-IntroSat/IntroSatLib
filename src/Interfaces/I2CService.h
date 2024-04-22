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

		using ReadCallbackParam = tl::expected<const std::vector<std::uint8_t>, Base::Error>;
		using ReadCallback = const std::function<void(ReadCallbackParam)>;
		using WriteCallback = const std::function<void(tl::expected<void, Base::Error>)>;

		using IntroSatLib::Base::Error;

		class I2CService: public Holders::Base::IService
		{
		private:

			class I2CData
			{
			public:
				std::uint8_t _addr;
				std::uint8_t _reg;
				std::vector<std::uint8_t> _data;
				ReadCallback _readCallBack;
				WriteCallback _writeCallBack;
			public:

				I2CData(
					std::uint8_t addr,
					std::uint8_t mem,
					std::uint8_t size,
					ReadCallback callback
				): _addr(0x80 | addr),
					_reg(mem),
					_readCallBack(callback)
				{ _data.resize(size, 0); }

				I2CData(
					std::uint8_t addr,
					std::uint8_t mem,
					const std::vector<std::uint8_t>& data,
					WriteCallback callback
				): _addr(addr),
					_reg(mem),
					_data(data),
					_writeCallBack(callback) { }

				I2CData(
					std::uint8_t addr,
					std::uint8_t size,
					ReadCallback callback
				): _addr(0x80 | addr),
					_reg(0xFF),
					_readCallBack(callback)
				{ _data.resize(size, 0); }

				I2CData(
					std::uint8_t addr,
					const std::vector<std::uint8_t>& data,
					WriteCallback callback
				): _addr(addr),
					_reg(0xFF),
					_data(data),
					_writeCallBack(callback) { }

			};

			std::vector<I2CData> _list;
			tl::expected<void, Error> UpdateData()
			{
				for (I2CData& data : _list)
				{
					WriteI2CData(data);
				}
				_list.clear();
				return tl::expected<void, Error>();
			}
			Error WriteI2CData(I2CData& i2cData)
			{
				std::uint8_t addr = i2cData._addr << 1;

				std::uint8_t isRead = i2cData._addr >= 0x80;
				std::uint8_t reg = i2cData._reg;

				std::uint8_t* data = i2cData._data.data();
				std::uint8_t len = i2cData._data.size();
				Error error = 0;
				if (reg == 0xFF)
				{
					error = isRead ? ForceRead(addr, data, len) : ForceWrite(addr, data, len);

				}
				else
				{
					error = isRead ? ForceReadMem(addr, reg, data, len) : ForceWriteMem(addr, reg, data, len);
				}

				if (isRead)
				{
					ReadCallbackParam param = error ? tl::make_unexpected(error) : ReadCallbackParam(i2cData._data);
					i2cData._readCallBack(param);
				}
				else
				{

				}


				if (!error)
				{
					mode & 1 ?  : i2cData->_writeCallBack();
				}
				return 0;
			}
			tl::expected<void, Error> Start(const Holders::Base::IHolder& holder) override
			{
				return tl::expected<void, Error>();
			}

			tl::expected<void, Error> PreUpdate() override
			{
				return UpdateData();
			}
			tl::expected<void, Error> Update() override
			{
				return UpdateData();
			}
			tl::expected<void, Error> PostUpdate() override
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
				_list.push_back({addr, data, callback});
			}

			void Read(std::uint8_t addr, std::uint8_t size, const ReadCallback& callback)
			{
				_list.push_back({addr, size, callback});
			}

			void WriteMem(std::uint8_t addr, std::uint8_t mem, const std::vector<std::uint8_t>& data, const WriteCallback& callback)
			{
				_list.push_back({addr, mem, data, callback});
			}

			void ReadMem(std::uint8_t addr, std::uint8_t mem, std::uint8_t size, const ReadCallback& callback)
			{
				_list.push_back({addr, mem, size, callback});
			}
		};
	} /* namespace Services */
} /* namespace IntroSatLib */

#endif /* _SERVICES_I2CSERVICE_H_ */
