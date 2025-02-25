#ifndef CC1101_H_
#define CC1101_H_

#include "./interfaces/GPIO.h"
#include "./interfaces/SPI.h"
#include <array>
#include "string.h"
#include "math.h"

#ifdef HAVE_SPI
namespace IntroSatLib {

class CC1101 {

private:

	enum ConfigRegisterMap
	{
		CC1101_IOCFG2       = 0x00,        // GDO2 output pin configuration
		CC1101_IOCFG1       = 0x01,        // GDO1 output pin configuration
		CC1101_IOCFG0       = 0x02,        // GDO0 output pin configuration
		CC1101_FIFOTHR      = 0x03,        // RX FIFO and TX FIFO thresholds
		CC1101_SYNC1        = 0x04,        // Sync word, high INT8U
		CC1101_SYNC0        = 0x05,        // Sync word, low INT8U
		CC1101_PKTLEN       = 0x06,        // Packet length
		CC1101_PKTCTRL1     = 0x07,        // Packet automation control
		CC1101_PKTCTRL0     = 0x08,        // Packet automation control
		CC1101_ADDR         = 0x09,        // Device address
		CC1101_CHANNR       = 0x0A,        // Channel number
		CC1101_FSCTRL1      = 0x0B,        // Frequency synthesizer control
		CC1101_FSCTRL0      = 0x0C,        // Frequency synthesizer control
		CC1101_FREQ2        = 0x0D,        // Frequency control word, high INT8U
		CC1101_FREQ1        = 0x0E,        // Frequency control word, middle INT8U
		CC1101_FREQ0        = 0x0F,        // Frequency control word, low INT8U
		CC1101_MDMCFG4      = 0x10,        // Modem configuration
		CC1101_MDMCFG3      = 0x11,        // Modem configuration
		CC1101_MDMCFG2      = 0x12,        // Modem configuration
		CC1101_MDMCFG1      = 0x13,        // Modem configuration
		CC1101_MDMCFG0      = 0x14,        // Modem configuration
		CC1101_DEVIATN      = 0x15,        // Modem deviation setting
		CC1101_MCSM2        = 0x16,        // Main Radio Control State Machine configuration
		CC1101_MCSM1        = 0x17,        // Main Radio Control State Machine configuration
		CC1101_MCSM0        = 0x18,        // Main Radio Control State Machine configuration
		CC1101_FOCCFG       = 0x19,        // Frequency Offset Compensation configuration
		CC1101_BSCFG        = 0x1A,        // Bit Synchronization configuration
		CC1101_AGCCTRL2     = 0x1B,        // AGC control
		CC1101_AGCCTRL1     = 0x1C,        // AGC control
		CC1101_AGCCTRL0     = 0x1D,        // AGC control
		CC1101_WOREVT1      = 0x1E,        // High INT8U Event 0 timeout
		CC1101_WOREVT0      = 0x1F,        // Low INT8U Event 0 timeout
		CC1101_WORCTRL      = 0x20,        // Wake On Radio control
		CC1101_FREND1       = 0x21,        // Front end RX configuration
		CC1101_FREND0       = 0x22,        // Front end TX configuration
		CC1101_FSCAL3       = 0x23,        // Frequency synthesizer calibration
		CC1101_FSCAL2       = 0x24,        // Frequency synthesizer calibration
		CC1101_FSCAL1       = 0x25,        // Frequency synthesizer calibration
		CC1101_FSCAL0       = 0x26,        // Frequency synthesizer calibration
		CC1101_RCCTRL1      = 0x27,        // RC oscillator configuration
		CC1101_RCCTRL0      = 0x28,        // RC oscillator configuration
		CC1101_FSTEST       = 0x29,        // Frequency synthesizer calibration control
		CC1101_PTEST        = 0x2A,        // Production test
		CC1101_AGCTEST      = 0x2B,        // AGC test
		CC1101_TEST2        = 0x2C,        // Various test settings
		CC1101_TEST1        = 0x2D,        // Various test settings
		CC1101_TEST0        = 0x2E         // Various test settings
	};

	enum StrobeCommand
	{
		CC1101_SRES       = 0x30,       // Reset chip.
		CC1101_SFSTXON    = 0x31,       // Enable and calibrate frequency synthesizer (if MCSM0.FS_AUTOCAL=1).
                                        // If in RX/TX: Go to a wait state where only the synthesizer is
                                        // running (for quick RX / TX turnaround).
		CC1101_SXOFF      = 0x32,       // Turn off crystal oscillator.
		CC1101_SCAL       = 0x33,       // Calibrate frequency synthesizer and turn it off
                                        // (enables quick start).
		CC1101_SRX        = 0x34,       // Enable RX. Perform calibration first if coming from IDLE and
                                        // MCSM0.FS_AUTOCAL=1.
		CC1101_STX        = 0x35,       // In IDLE state: Enable TX. Perform calibration first if
                                        // MCSM0.FS_AUTOCAL=1. If in RX state and CCA is enabled:
                                        // Only go to TX if channel is clear.
		CC1101_SIDLE      = 0x36,       // Exit RX / TX, turn off frequency synthesizer and exit
                                        // Wake-On-Radio mode if applicable.
		CC1101_SAFC       = 0x37,       // Perform AFC adjustment of the frequency synthesizer
		CC1101_SWOR       = 0x38,       // Start automatic RX polling sequence (Wake-on-Radio)
		CC1101_SPWD       = 0x39,       // Enter power down mode when CSn goes high.
		CC1101_SFRX       = 0x3A,       // Flush the RX FIFO buffer.
		CC1101_SFTX       = 0x3B,       // Flush the TX FIFO buffer.
		CC1101_SWORRST    = 0x3C,       // Reset real time clock.
		CC1101_SNOP       = 0x3D,       // No operation. May be used to pad strobe commands to two
                                        // INT8Us for simpler software.
	};

	enum StatusRegister
	{
		CC1101_PARTNUM    = 0x30,
		CC1101_VERSION    = 0x31,
		CC1101_FREQEST    = 0x32,
		CC1101_LQI        = 0x33,
		CC1101_RSSI       = 0x34,
		CC1101_MARCSTATE  = 0x35,
		CC1101_WORTIME1   = 0x36,
		CC1101_WORTIME0   = 0x37,
		CC1101_PKTSTATUS  = 0x38,
		CC1101_VCO_VC_DAC = 0x39,
		CC1101_TXBYTES    = 0x3A,
		CC1101_RXBYTES    = 0x3B,
	};

	const static uint8_t WRITE_BURST     = 0x40;
	const static uint8_t READ_SINGLE     = 0x80;
	const static uint8_t READ_BURST      = 0xC0;
	const static uint8_t BYTES_IN_RXFIFO = 0x7F;

	const static uint8_t CC1101_PATABLE    = 0x3E;
	const static uint8_t CC1101_TXFIFO     = 0x3F;
	const static uint8_t CC1101_RXFIFO     = 0x3F;

	enum State
	{
		NONE = 0,
		TX   = 1,
		RX   = 2
	};

	class RegisterData
	{
	private:
		ConfigRegisterMap _reg;
		uint8_t _value;
		uint8_t _ready;
	public:

		constexpr
		RegisterData(ConfigRegisterMap reg, uint8_t value):
		_reg(reg), _value(value), _ready(1) { }

		constexpr
		RegisterData(ConfigRegisterMap reg):
		_reg(reg), _value(0), _ready(0) { }

		constexpr void set(uint8_t value) { _value = value; _ready = 1; }

		constexpr uint8_t get() const { return _value; }

		constexpr std::array<uint8_t, 2> toWrite()
		{ _ready = 0; return {_reg, _value}; }

		constexpr std::array<uint8_t, 2> toRead() const
		{ return {(uint8_t)(_reg | READ_SINGLE), 0}; }

		constexpr void fromRead(std::array<uint8_t, 2> value)
		{ _ready = 0; _value = value[1]; }

	};

	struct CalibrateValues
	{
		float _treashold;
		uint16_t _minFreq;
		uint16_t _maxFreq;
		uint8_t _minData;
		uint8_t _maxData;

		constexpr uint16_t
		getData(float mhz) const
		{
			long x = mhz;
			long in_min = _minFreq;
			long in_max = _maxFreq;
			long out_min = _minData;
			long out_max = _maxData;
			return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
		}

		constexpr uint8_t
		useTreshold(float mhz) const
		{ return mhz >= _treashold; }
	};

private:
	intefaces::SPI &_spi;
	intefaces::GPIO &_scl;
	intefaces::GPIO &_mosi;
	intefaces::GPIO &_miso;
	intefaces::GPIO &_reset;

	int _pa = 12;
	uint8_t _modulation = 2;
	uint8_t _frend0 = 0;
	uint8_t _chan = 0;
	uint8_t _last_pa = 0;
	uint8_t _ccmode = 0;
	float   _mhz = 433.92f;
	uint8_t _m4RxBw = 0;
	uint8_t _m4DaRa = 0;
	uint8_t _m2DCOFF = 0;
	uint8_t _m2MODFM = 0;
	uint8_t _m2MANCH = 0;
	uint8_t _m2SYNCM = 0;
	uint8_t _m1FEC = 0;
	uint8_t _m1PRE = 0;
	uint8_t _m1CHSP = 0;
	uint8_t _pc1PQT = 0;
	uint8_t _pc1CRC_AF = 0;
	uint8_t _pc1APP_ST = 0;
	uint8_t _pc1ADRCHK = 0;
	uint8_t _pc0WDATA = 0;
	uint8_t _pc0PktForm = 0;
	uint8_t _pc0CRC_EN = 0;
	uint8_t _pc0LenConf = 0;
	State   _trxstate = State::NONE;

	static constexpr std::array<std::array<uint8_t, 10>, 4> PA_TABLE_CONST =
	{
		std::array<uint8_t, 10> { 0x12, 0x0D, 0x1C, 0x34, 0x51, 0x85, 0xCB, 0xC2, 0xC2, 0xC2 },
		std::array<uint8_t, 10> { 0x12, 0x0E, 0x1D, 0x34, 0x60, 0x84, 0xC8, 0xC0, 0xC0, 0xC0 },
		std::array<uint8_t, 10> { 0x03, 0x17, 0x1D, 0x26, 0x37, 0x50, 0x86, 0xCD, 0xC5, 0xC0 },
		std::array<uint8_t, 10> { 0x03, 0x0E, 0x1E, 0x27, 0x38, 0x8E, 0x84, 0xCC, 0xC3, 0xC0 }
	};

	std::array<uint8_t, 8> PA_TABLE = {0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	std::array<CalibrateValues, 4> CLB = {
		CalibrateValues { 322.88f, 300, 348, 24, 28 },
		CalibrateValues { 430.50f, 378, 464, 31, 38 },
		CalibrateValues { 861.00f, 779, 900, 65, 77 },
		CalibrateValues {   0.00f, 900, 928, 77, 79 }
	};

	std::array<RegisterData, 0x2F> _registers = {
		RegisterData { CC1101_IOCFG2   },
		RegisterData { CC1101_IOCFG1   },
		RegisterData { CC1101_IOCFG0   },
		RegisterData { CC1101_FIFOTHR  },
		RegisterData { CC1101_SYNC1    },
		RegisterData { CC1101_SYNC0    },
		RegisterData { CC1101_PKTLEN   },
		RegisterData { CC1101_PKTCTRL1 },
		RegisterData { CC1101_PKTCTRL0 },
		RegisterData { CC1101_ADDR     },
		RegisterData { CC1101_CHANNR   },
		RegisterData { CC1101_FSCTRL1  },
		RegisterData { CC1101_FSCTRL0  },
		RegisterData { CC1101_FREQ2    },
		RegisterData { CC1101_FREQ1    },
		RegisterData { CC1101_FREQ0    },
		RegisterData { CC1101_MDMCFG4  },
		RegisterData { CC1101_MDMCFG3  },
		RegisterData { CC1101_MDMCFG2  },
		RegisterData { CC1101_MDMCFG1  },
		RegisterData { CC1101_MDMCFG0  },
		RegisterData { CC1101_DEVIATN  },
		RegisterData { CC1101_MCSM2    },
		RegisterData { CC1101_MCSM1    },
		RegisterData { CC1101_MCSM0    },
		RegisterData { CC1101_FOCCFG   },
		RegisterData { CC1101_BSCFG    },
		RegisterData { CC1101_AGCCTRL2 },
		RegisterData { CC1101_AGCCTRL1 },
		RegisterData { CC1101_AGCCTRL0 },
		RegisterData { CC1101_WOREVT1  },
		RegisterData { CC1101_WOREVT0  },
		RegisterData { CC1101_WORCTRL  },
		RegisterData { CC1101_FREND1   },
		RegisterData { CC1101_FREND0   },
		RegisterData { CC1101_FSCAL3   },
		RegisterData { CC1101_FSCAL2   },
		RegisterData { CC1101_FSCAL1   },
		RegisterData { CC1101_FSCAL0   },
		RegisterData { CC1101_RCCTRL1  },
		RegisterData { CC1101_RCCTRL0  },
		RegisterData { CC1101_FSTEST   },
		RegisterData { CC1101_PTEST    },
		RegisterData { CC1101_AGCTEST  },
		RegisterData { CC1101_TEST2    },
		RegisterData { CC1101_TEST1    },
		RegisterData { CC1101_TEST0    }
	};
protected:
	virtual void await(uint8_t len) = 0;

private:
	void start() { }
	void end() { _scl.reset(); }

	void select() const { _reset.reset(); }
	void deSelect() const { _reset.set(); }

	void sendRegister(RegisterData& reg);
	void sendRegister(uint8_t reg, uint8_t value);
	uint8_t readRegister(RegisterData& reg);


	void setRegister(ConfigRegisterMap reg, uint8_t value)
	{
		_registers[(uint8_t)reg].set(value);
		sendRegister(_registers[(uint8_t)reg]);
	}

	uint8_t readRegister(ConfigRegisterMap reg)
	{ return readRegister(_registers[(uint8_t)reg]); }

	void setStrobe(StrobeCommand command, uint8_t initSpi = 1);
	void setBurstReg(uint8_t reg, const uint8_t* data, uint8_t len);
	void readBurstReg(uint8_t reg, uint8_t* data, uint8_t len);

	uint8_t readStatus(uint8_t reg);
	uint8_t readStatus(StatusRegister reg)
	{ return readStatus((uint8_t)reg); }

	uint8_t readStatus(ConfigRegisterMap reg)
	{ return readStatus((uint8_t)reg); }

	void reset();
	void calibrate();
	void setDefualtSettings();

	uint8_t getMHzCase()
	{
		if (_mhz >= 300.00f && _mhz <= 348.00f) { return 0; }
		if (_mhz >= 378.00f && _mhz <= 464.00f) { return 1; }
		if (_mhz >= 779.00f && _mhz <  900.00f) { return 2; }
		if (_mhz >= 900.00f && _mhz <= 928.00f) { return 3; }
		return 0;
	}

public:
	CC1101(
		intefaces::SPI &spi,
		intefaces::GPIO &scl,
		intefaces::GPIO &mosi,
		intefaces::GPIO &miso,
		intefaces::GPIO &reset
	): _spi(spi), _scl(scl), _mosi(mosi), _miso(miso),  _reset(reset)
	{ }

	void Init();

	void setChannel(uint8_t ch);
	void setPacketLength(uint8_t v);
	void setCCMode(uint8_t ccmode);
	void setModulation(uint8_t modulation);

	void setPA(int p);
	void setMHZ(float mhz);
	void setClb(uint8_t b, uint8_t s, uint8_t e);
	uint8_t getCC1101();

	State getMode() { return _trxstate; }
	void setSyncWord(uint8_t sh, uint8_t sl);
	void setAddr(uint8_t v);

	// PKTCTRL1
	void setPQT(uint8_t v);
	void setCRC_AF(uint8_t v);
	void setAppendStatus(uint8_t v);
	void setAdrChk(uint8_t v);
	// PKTCTRL0
	void setWhiteData(uint8_t v);
	void setPktFormat(uint8_t v);
	void setCrc(uint8_t v);
	void setLengthConfig(uint8_t v);
	// MDMCFG4
	void setRxBW(float f);
	// MDMCFG4 + MDMCFG3
	virtual void setDRate(float kD);
	// MDMCFG2
	void setDcFilterOff(uint8_t v);
	void setManchester(uint8_t v);
	void setSyncMode(uint8_t v);
	// MDMCFG1
	void setFEC(uint8_t v);
	void setPRE(uint8_t v);
	// MDMCFG1 + MDMCFG0
	void setChsp(float f);

	void setDeviation(float d);

	void Split_PKTCTRL1();
	void Split_PKTCTRL0();

	void Split_MDMCFG4();
	void Split_MDMCFG2();
	void Split_MDMCFG1();

	void SetTx();
	void SetRx();

	void SetTx(float mhz);
	void SetRx(float mhz);

	int getRssi();
	uint8_t getLqi();
	void setSres();
	void setSidle();

	void goSleep();

	void SendData(const char *txchar);
	void SendData(uint8_t *txBuffer, uint8_t size);

	uint8_t CheckCRC();

};

class CC1101WithDelay: public CC1101
{

	static constexpr float getTimeoutK(float kD)
	{
		return (1.66f + 0.6f * (log10f(kD) + 3)) / kD;
	}

	float timeoutK = getTimeoutK(99.97f);

public:
	CC1101WithDelay(
			intefaces::SPI &spi,
			intefaces::GPIO &scl,
			intefaces::GPIO &mosi,
			intefaces::GPIO &miso,
			intefaces::GPIO &reset
		): CC1101(spi, scl, mosi, miso, reset) { }

	void await(uint8_t len) override
	{
		uint32_t timeout = ceilf((float)len * timeoutK * 8);
		HAL_Delay(timeout);
	}

	void setDRate(float kD) override
	{
		CC1101::setDRate(kD);
		timeoutK = getTimeoutK(kD);
	};
};

class CC1101WithGD0: public CC1101
{

	intefaces::GPIO &_gd0;

public:
	CC1101WithGD0(
			intefaces::SPI &spi,
			intefaces::GPIO &scl,
			intefaces::GPIO &mosi,
			intefaces::GPIO &miso,
			intefaces::GPIO &reset,
			intefaces::GPIO &gd0
		): CC1101(spi, scl, mosi, miso, reset), _gd0(gd0) { }

	void await(uint8_t len) override
	{
		logText("wait set\n");
		_gd0.waitSet();
		logText("wait reset\n");
		_gd0.waitReset();
		HAL_Delay(10);
	}

};

} /* namespace IntroSatLib */
#endif
#endif /* CC1101_H_ */
