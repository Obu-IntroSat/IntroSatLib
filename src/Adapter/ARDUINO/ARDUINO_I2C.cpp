#define INTROSATLIB_INTERNAL
#include "Adapter/I2C.h"

//#define ARDUINO
#ifdef ARDUINO

#include "Logger.h"


IntroSatLib::interfaces::I2C::I2C(I2C_HANDLE_TYPE *hi2c): _hi2c(hi2c)
{
}

IntroSatLib::interfaces::I2C::I2C(I2C_HANDLE_TYPE *hi2c, I2CSpeed speed): _hi2c(hi2c), _speed(speed)
{
}


//IntroSatLib::interfaces::I2C::I2C(TwoWire &hi2c): _hi2c(hi2c)
//{
//}
//
//IntroSatLib::interfaces::I2C::I2C(TwoWire &hi2c, I2CSpeed speed): _hi2c(hi2c), _speed(speed)
//{
//}

IntroSatLib::interfaces::I2C::I2C(const I2C& other)
{
	_hi2c = other._hi2c;
	_speed = other._speed;
}

IntroSatLib::interfaces::I2C::I2C(I2C&& other)
{
	_hi2c = other._hi2c;
	_speed = other._speed;
}

IntroSatLib::interfaces::I2C& IntroSatLib::interfaces::I2C::operator=(const I2C& other)
{
	if (this == &other)
	{
		return *this;
	}
	_hi2c = other._hi2c;
	_speed = other._speed;
	return *this;
}

IntroSatLib::interfaces::I2C& IntroSatLib::interfaces::I2C::operator=(I2C&& other)
{
	if (this == &other)
	{
		return *this;
	}
	_hi2c = other._hi2c;
	_speed = other._speed;
	return *this;
}
ISL_StatusTypeDef IntroSatLib::interfaces::I2C::isReady(uint8_t deviceAddress, uint8_t waitIsReady)
{
	ASSERT_I2C_HAVE();
	while(true)
	{
		ISL_StatusTypeDef status = innerIsReady(deviceAddress);
		if (status == ISL_StatusTypeDef::ISL_OK) { return ISL_StatusTypeDef::ISL_OK; }
		if (waitIsReady == 0) { return status; }
	}
}

ISL_StatusTypeDef IntroSatLib::interfaces::I2C::innerIsReady(uint8_t deviceAddress)
{
	LOG_I2C_ADDRESS();
	logText(": ");
	_hi2c->beginTransmission(deviceAddress);
	uint8_t result = _hi2c->endTransmission();
	ISL_StatusTypeDef status = ISL_StatusTypeDef::ISL_OK;
	// result codes of endTransmission() according to
	// https://docs.arduino.cc/language-reference/en/functions/communication/wire/endTransmission/
	switch (result) {
		case (0):	// success
			break;
		case (5):	// timeout
			status = ISL_StatusTypeDef::ISL_TIMEOUT;
			break;
		case (1):	// data too long to fit in transmit buffer
		case (2):	// received NACK on transmit of address
		case (3):	// received NACK on transmit of data
		case (4):	// other error
		default:
			status = ISL_StatusTypeDef::ISL_ERROR;
	}
	logStatus(status);
	logText("\n");
	return status;
}


ISL_StatusTypeDef IntroSatLib::interfaces::I2C::read(uint8_t deviceAddress, uint8_t* data, uint8_t nBytes)
{
	ASSERT_I2C_HAVE();
	LOG_I2C_ADDRESS();
	logText(" read ");
	logNumber(nBytes);

	// flush RX buffer
	while (_hi2c->available()) {
		_hi2c->read();
	}

/* TODO according to
 * https://docs.arduino.cc/language-reference/en/functions/communication/wire/setWireTimeout/#:~:text=Code%20that%20needs,are%20all%20available.
 * WIRE_HAS_TIMEOUT is defined when Wire.setWireTimeout(), Wire.getWireTimeoutFlag() and Wire.clearWireTimeot() are all defined
 * However it seems like WIRE_HAS_TIMEOUT is not defined even for Arduino UNO in Arduino IDE, although all the methods above are defined.
*/
#ifdef WIRE_HAS_TIMEOUT
	_hi2c->clearWireTimeout();
#endif

	// TODO this method returns the amount of received bytes. Can be used to check if enough bytes were received
	uint8_t rxCount = _hi2c->requestFrom(deviceAddress, nBytes);

	ISL_StatusTypeDef status = ISL_StatusTypeDef::ISL_OK;

	#ifdef WIRE_HAS_TIMEOUT
	if (_hi2c->getWireTimeoutFlag()) { status = logStatus(ISL_StatusTypeDef::ISL_TIMEOUT); }
#else
	if (rxCount < nBytes) { status = logStatus(ISL_StatusTypeDef::ISL_TIMEOUT); }
#endif

	if (status == ISL_StatusTypeDef::ISL_OK) {
		// Handle basic errors to mimic HAL_I2C_Master_Receive method (kinda)
		if (_hi2c->available() != nBytes) {
			// buffer has less bytes than expected
			status = logStatus(ISL_StatusTypeDef::ISL_ERROR);
		} else {
			logStatus(status);

			_hi2c->readBytes(data, nBytes);

			logText(" bytes > ");
			LOG_I2C_BUFFER(", ", data, nBytes);
		}


	}
	logText("\n");
	return status;
}

ISL_StatusTypeDef IntroSatLib::interfaces::I2C::readMem(uint8_t deviceAddress, uint8_t reg, uint8_t* data, uint8_t nBytes)
{
	ASSERT_I2C_HAVE();
	LOG_I2C_ADDRESS();
	logText(" read from memory ");
	logHEX(reg);
	logText(" ");
	logNumber(nBytes);

	// flush RX buffer
	while (_hi2c->available()) {
		_hi2c->read();
	}

	/* TODO according to
	 * https://docs.arduino.cc/language-reference/en/functions/communication/wire/setWireTimeout/#:~:text=Code%20that%20needs,are%20all%20available.
	 * WIRE_HAS_TIMEOUT is defined when Wire.setWireTimeout(), Wire.getWireTimeoutFlag() and Wire.clearWireTimeot() are all defined
	 * However it seems like WIRE_HAS_TIMEOUT is not defined even for Arduino UNO in Arduino IDE, although all the methods above are defined.
	*/
#ifdef WIRE_HAS_TIMEOUT
	_hi2c->clearWireTimeout();
#endif
	// TODO check if this method works as expected
	uint8_t rxCount = _hi2c->requestFrom(deviceAddress, nBytes, reg, 1, true);

	ISL_StatusTypeDef status = ISL_StatusTypeDef::ISL_OK;
#ifdef WIRE_HAS_TIMEOUT
	if (_hi2c->getWireTimeoutFlag()) { return logStatus(ISL_StatusTypeDef::ISL_TIMEOUT); }
#else
	if (rxCount < nBytes) { status = logStatus(ISL_StatusTypeDef::ISL_TIMEOUT); }
#endif

	// Handle basic errors to mimic HAL_I2C_Master_Receive method (kinda)
	if (status == ISL_StatusTypeDef::ISL_OK) {
		// Handle basic errors to mimic HAL_I2C_Master_Receive method (kinda)
		if (_hi2c->available() != nBytes) {
			// buffer has more or less bytes than expected
			status = logStatus(ISL_StatusTypeDef::ISL_ERROR);
		} else {
			logStatus(status);

			_hi2c->readBytes(data, nBytes);

			logText(" bytes > ");
			LOG_I2C_BUFFER(", ", data, nBytes);
		}


	}
	logText("\n");
	return status;
}

ISL_StatusTypeDef IntroSatLib::interfaces::I2C::write(uint8_t deviceAddress, uint8_t* data, uint8_t nBytes)
{
	ASSERT_I2C_HAVE();
	LOG_I2C_ADDRESS();
	logText(" write ");
	logNumber(nBytes);
	logText(" bytes ");
	LOG_I2C_BUFFER(", ", data, Nbytes);
	logText(" > ");

	/* TODO according to
	 * https://docs.arduino.cc/language-reference/en/functions/communication/wire/setWireTimeout/#:~:text=Code%20that%20needs,are%20all%20available.
	 * WIRE_HAS_TIMEOUT is defined when Wire.setWireTimeout(), Wire.getWireTimeoutFlag() and Wire.clearWireTimeot() are all defined
	 * However it seems like WIRE_HAS_TIMEOUT is not defined even for Arduino UNO in Arduino IDE, although all the methods above are defined.
	*/
#ifdef WIRE_HAS_TIMEOUT
	_hi2c->clearWireTimeout();
#endif

	_hi2c->beginTransmission(deviceAddress);
	_hi2c->write(data, nBytes);
	uint8_t result = _hi2c->endTransmission();

#ifdef WIRE_HAS_TIMEOUT
	if (_hi2c->getWireTimeoutFlag()) { return logStatus(ISL_StatusTypeDef::ISL_TIMEOUT); }
#endif

	ISL_StatusTypeDef status = ISL_StatusTypeDef::ISL_OK;
	// result codes of endTransmission() according to
	// https://docs.arduino.cc/language-reference/en/functions/communication/wire/endTransmission/
	switch (result) {
		case (0):	// success
			break;
		case (5):	// timeout
			status = ISL_StatusTypeDef::ISL_TIMEOUT;
			break;
		case (1):	// data too long to fit in transmit buffer
		case (2):	// received NACK on transmit of address
		case (3):	// received NACK on transmit of data
		case (4):	// other error
		default:
			status = ISL_StatusTypeDef::ISL_ERROR;
	}

	logStatus(status);
	logText("\n");
	return status;
}

ISL_StatusTypeDef IntroSatLib::interfaces::I2C::writeMem(uint8_t deviceAddress, uint8_t reg, uint8_t* data, uint8_t nBytes)
{
	ASSERT_I2C_HAVE();
	LOG_I2C_ADDRESS();
	logText(" write to memory ");
	logHEX(reg);
	logText(" ");
	logNumber(nBytes);
	logText(" bytes ");
	LOG_I2C_BUFFER(", ", data, nBytes);
	logText(" > ");

	/* TODO according to
	 * https://docs.arduino.cc/language-reference/en/functions/communication/wire/setWireTimeout/#:~:text=Code%20that%20needs,are%20all%20available.
	 * WIRE_HAS_TIMEOUT is defined when Wire.setWireTimeout(), Wire.getWireTimeoutFlag() and Wire.clearWireTimeot() are all defined
	 * However it seems like WIRE_HAS_TIMEOUT is not defined even for Arduino UNO in Arduino IDE, although all the methods above are defined.
	*/
#ifdef WIRE_HAS_TIMEOUT
	_hi2c->clearWireTimeout();
#endif

	_hi2c->beginTransmission(deviceAddress);
	_hi2c->write(reg);
	_hi2c->write(data, nBytes);
	uint8_t result = _hi2c->endTransmission();

#ifdef WIRE_HAS_TIMEOUT
	if (_hi2c->getWireTimeoutFlag()) { return logStatus(ISL_StatusTypeDef::ISL_TIMEOUT); }
#endif

	ISL_StatusTypeDef status = ISL_StatusTypeDef::ISL_OK;
	// result codes of endTransmission() according to
	// https://docs.arduino.cc/language-reference/en/functions/communication/wire/endTransmission/
	switch (result) {
		case (0):	// success
			break;
		case (5):	// timeout
			status = ISL_StatusTypeDef::ISL_TIMEOUT;
			break;
		case (1):	// data too long to fit in transmit buffer
		case (2):	// received NACK on transmit of address
		case (3):	// received NACK on transmit of data
		case (4):	// other error
		default:
			status = ISL_StatusTypeDef::ISL_ERROR;
	}

	logStatus(status);
	logText("\n");
	return status;
}



IntroSatLib::interfaces::I2C::~I2C()
{
}

#endif /* HAL_I2C_MODULE_ENABLED */
