#ifndef IRCAMERA_H_
#define IRCAMERA_H_

#include "I2CDevice.h"
#include "BaseDevice.h"

namespace IntroSatLib {

class IRCamera: public BaseDevice {
private:

	static const uint8_t BASE_ADDRESS = 0x68;
	static constexpr float _rawdeg = 0.25f;

	enum RegisterMap
	{
	  AMG88xx_PCTL = 0x00,
	  AMG88xx_RST = 0x01,
	  AMG88xx_FPSC = 0x02,
	  AMG88xx_INTC = 0x03,
	  AMG88xx_STAT = 0x04,
	  AMG88xx_SCLR = 0x05,
	  // 0x06 reserved
	  AMG88xx_AVE = 0x07,
	  AMG88xx_INTHL = 0x08,
	  AMG88xx_INTHH = 0x09,
	  AMG88xx_INTLL = 0x0A,
	  AMG88xx_INTLH = 0x0B,
	  AMG88xx_IHYSL = 0x0C,
	  AMG88xx_IHYSH = 0x0D,
	  AMG88xx_TTHL = 0x0E,
	  AMG88xx_TTHH = 0x0F,
	  AMG88xx_INT_OFFSET = 0x010,
	  AMG88xx_PIXEL_OFFSET = 0x80
	};


	uint8_t _framrate = 0;
	uint8_t _mirror = 0;

	GPIO_TypeDef* _resetPort = 0;
	uint16_t _resetPin = 0;

	int16_t _buffer[64] = {0};

	void tryReset();
	int16_t int12ToInt16(uint16_t val);

public:

	enum Framerate
	{
		FPS_10 = 0,
		FPS_1 = 1
	};

#ifndef ARDUINO
	IRCamera(I2C_HandleTypeDef *hi2c, uint8_t address = BASE_ADDRESS);
#else
	IRCamera(TwoWire &hi2c, uint8_t address = BASE_ADDRESS);
	IRCamera(uint8_t address = BASE_ADDRESS);
#endif

	IRCamera(const IRCamera &other);
	IRCamera(IRCamera &&other);
	IRCamera& operator=(const IRCamera &other);
	IRCamera& operator=(IRCamera &&other);

	void useForceReset(GPIO_TypeDef* resetPort, uint16_t resetPin);

	void useMirrored();
	void useNotMirrored();

	uint8_t Init() override;
	uint8_t Init(Framerate framrate);

	uint8_t Read();

	int16_t getPixelRaw(uint8_t x, uint8_t y);

	float getPixel(uint8_t x, uint8_t y);

	virtual ~IRCamera();
};

} /* namespace IntroSatLib */

#endif /* IRCAMERA_H_ */
