#include "Interface.h"

namespace IntroSatLib
{
namespace Base
{

Status Interface::Read(uint8_t reg, uint8_t &data, Endian end)
{
	uint8_t buf[1] =
	{ 0 };
	Status status = Read(reg, buf, 1);
	if (status != Status::Ok)
	{
		data = 0;
		return status;
	}
	data = buf[0];
	return status;
}

Status Interface::Read(uint8_t reg, uint16_t &data, Endian end)
{
	uint8_t buf[2] =
	{ 0 };
	Status status = Read(reg, buf, 2);
	if (status != Status::Ok)
	{
		data = 0;
		return status;
	}
	if (end == Endian::BE)
	{
		data = ((uint16_t) buf[0]) << 8;
		data |= buf[1];
	}
	else
	{
		data = ((uint16_t) buf[1]) << 8;
		data |= buf[0];
	}
	return status;
}

Status Interface::Read(uint8_t reg, uint32_t &data, Endian end)
{
	uint8_t buf[4] =
	{ 0 };
	Status status = Read(reg, buf, 4);
	if (status != Status::Ok)
	{
		data = 0;
		return status;
	}
	if (end == Endian::BE)
	{
		data = ((uint32_t) buf[0]) << 24;
		data |= ((uint32_t) buf[1]) << 16;
		data |= ((uint16_t) buf[2]) << 8;
		data |= buf[3];
	}
	else
	{
		data = ((uint32_t) buf[3]) << 24;
		data |= ((uint32_t) buf[2]) << 16;
		data |= ((uint16_t) buf[1]) << 8;
		data |= buf[0];
	}
	return status;
}

Status Interface::Read(uint8_t reg, uint64_t &data, Endian end)
{
	uint8_t buf[8] =
	{ 0 };
	Status status = Read(reg, buf, 8);
	if (status != Status::Ok)
	{
		data = 0;
		return status;
	}
	if (end == Endian::BE)
	{
		data = ((uint64_t) buf[0]) << 56;
		data |= ((uint64_t) buf[1]) << 48;
		data |= ((uint64_t) buf[2]) << 40;
		data |= ((uint64_t) buf[3]) << 32;
		data |= ((uint32_t) buf[4]) << 24;
		data |= ((uint32_t) buf[5]) << 16;
		data |= ((uint16_t) buf[6]) << 8;
		data |= buf[7];
	}
	else
	{
		data = ((uint64_t) buf[7]) << 56;
		data |= ((uint64_t) buf[6]) << 48;
		data |= ((uint64_t) buf[5]) << 40;
		data |= ((uint64_t) buf[4]) << 32;
		data |= ((uint32_t) buf[3]) << 24;
		data |= ((uint32_t) buf[2]) << 16;
		data |= ((uint16_t) buf[1]) << 8;
		data |= buf[0];
	}
	return status;
}

Status Interface::Write(uint8_t reg, uint8_t data, Endian end)
{
	return Write(reg, &data, 1);
}

Status Interface::Write(uint8_t reg, uint16_t data, Endian end)
{
	uint8_t buf[2] =
	{ 0 };
	if (end == Endian::BE)
	{
		buf[0] = data >> 8;
		buf[1] = data;
	}
	else
	{
		buf[1] = data >> 8;
		buf[0] = data;
	}
	return Write(reg, buf, 2);
}

Status Interface::Write(uint8_t reg, uint32_t data, Endian end)
{
	uint8_t buf[4] =
	{ 0 };
	if (end == Endian::BE)
	{
		buf[0] = data >> 24;
		buf[1] = data >> 16;
		buf[2] = data >> 8;
		buf[3] = data;
	}
	else
	{
		buf[3] = data >> 24;
		buf[2] = data >> 16;
		buf[1] = data >> 8;
		buf[0] = data;
	}
	return Write(reg, buf, 4);
}

Status Interface::Write(uint8_t reg, uint64_t data, Endian end)
{
	uint8_t buf[8] =
	{ 0 };
	if (end == Endian::BE)
	{
		buf[0] = data >> 56;
		buf[1] = data >> 48;
		buf[2] = data >> 40;
		buf[3] = data >> 32;
		buf[4] = data >> 24;
		buf[5] = data >> 16;
		buf[6] = data >> 8;
		buf[7] = data;
	}
	else
	{
		buf[7] = data >> 56;
		buf[6] = data >> 48;
		buf[5] = data >> 40;
		buf[4] = data >> 32;
		buf[3] = data >> 24;
		buf[2] = data >> 16;
		buf[1] = data >> 8;
		buf[0] = data;
	}
	return Write(reg, buf, 8);
}
}
}

