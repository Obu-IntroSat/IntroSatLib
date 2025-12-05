#include "IRCamera.h"
#include "Adapter/System.h"
namespace IntroSatLib {

IRCamera::IRCamera(const interfaces::I2C &i2c, uint8_t address): I2CDevice(new interfaces::I2C(i2c), address)
{
}

IRCamera::IRCamera(const IRCamera &other): I2CDevice(other)
{
	_framrate = other._framrate;
	_reset = other._reset;
	_mirror = other._mirror;
}
IRCamera::IRCamera(IRCamera &&other): I2CDevice(other)
{
	_framrate = other._framrate;
	_reset = other._reset;
	_mirror = other._mirror;
}
IRCamera& IRCamera::operator=(const IRCamera &other)
{
	if (this != &other)
	{
		this->I2CDevice::operator = (other);
		_framrate = other._framrate;
		_reset = other._reset;
		_mirror = other._mirror;
	}
	return *this;
}
IRCamera& IRCamera::operator=(IRCamera &&other)
{
	if (this != &other)
	{
		this->I2CDevice::operator = (other);
		_framrate = other._framrate;
		_reset = other._reset;
		_mirror = other._mirror;
	}
	return *this;
}

ISL_StatusTypeDef IRCamera::Init(Framerate framrate)
{
	UNUSED(framrate);
	tryReset();
	RETURN_STATUS_IF_NOT_OK_SILENT(IsReady())
	RETURN_STATUS_IF_NOT_OK_SILENT(SetRegisterI2C(AMG88xx_PCTL, 0))
	RETURN_STATUS_IF_NOT_OK_SILENT(SetRegisterI2C(AMG88xx_RST, 0x3F))
	RETURN_STATUS_IF_NOT_OK_SILENT(SetRegisterI2C(AMG88xx_INTC, 0))
	RETURN_STATUS_IF_NOT_OK_SILENT(SetRegisterI2C(AMG88xx_FPSC, 0x01)) // this should be framerate
	system::Delay(1000);
	return ISL_OK;
}

ISL_StatusTypeDef IRCamera::Init() { return Init(Framerate::FPS_10); }

int16_t IRCamera::int12ToInt16(uint16_t val)
{
	int16_t sVal = (val << 4);
	return sVal >> 4;
}

uint8_t IRCamera::Read()
{
	uint8_t buffer[128];

	RETURN_STATUS_IF_NOT_OK_SILENT(ReadRegisterI2C(AMG88xx_PIXEL_OFFSET, buffer, 128))

	for (int i = 0; i < 64; i++)
	{
		uint8_t pos = i << 1;
		uint16_t recast = ((uint16_t)buffer[pos + 1] << 8) | ((uint16_t)buffer[pos]);

		_buffer[_mirror ? 63 - i : i] = int12ToInt16(recast);
	}
	return 0;
}

int16_t IRCamera::getPixelRaw(uint8_t x, uint8_t y)
{
	return _buffer[((y & 7) << 3) + (x & 7)];
}

float IRCamera::getPixel(uint8_t x, uint8_t y)
{
	return ((float)getPixelRaw(x, y)) * _rawdeg;
}

void IRCamera::useForceReset(interfaces::GPIO_HANDLE_TYPE& resetPort, uint16_t resetPin)
{
	_reset = new interfaces::GPIO(&resetPort, resetPin);
}

void IRCamera::useMirrored() { _mirror = 1; }

void IRCamera::useNotMirrored() { _mirror = 0; }


void IRCamera::tryReset()
{
	if (_reset == nullptr) { return; }
	_reset->set();
	system::Delay(100);
	_reset->reset();
	system::Delay(100);
}

IRCamera::~IRCamera() { }

}
