
#include <CC1101.h>

#ifdef HAVE_SPI

namespace IntroSatLib {

void CC1101::reset()
{
	select();
	HAL_Delay(1);
	deSelect();
	HAL_Delay(1);
	select();
	_miso.waitReset();
	uint8_t out = CC1101::StrobeCommand::CC1101_SRES;
	uint8_t in = 0;
	_spi.transfer(&out, &in, 1);
	_miso.waitReset();
	deSelect();
}

void CC1101::Init()
{
	start();
	deSelect();
	_scl.set();
	_mosi.reset();
	reset();
	setDefualtSettings();
	end();
}

void CC1101::sendRegister(RegisterData& reg)
{
	std::array<uint8_t, 2> transfer = reg.toWrite();
	std::array<uint8_t, 2> recive;
	start();
	select();
	_miso.waitReset();
	_spi.transfer(transfer, recive);
	deSelect();
	end();
}

void CC1101::sendRegister(uint8_t reg, uint8_t value)
{
	start();
	select();
	_miso.waitReset();
	uint8_t in = 0;
	_spi.transfer(&reg, &in, 1);
	_spi.transfer(&value, &in, 1);
	deSelect();
	end();
}

void CC1101::setBurstReg(uint8_t reg, const uint8_t* data, uint8_t len)
{
	start();
	select();
	_miso.waitReset();
	reg |= WRITE_BURST;
	uint8_t result = 0;
	_spi.transfer(&reg, &result, 1);
	for (uint8_t i = 0; i < len; i++) { _spi.transfer(data + i, &result, 1); }
	deSelect();
	end();
}

void CC1101::setStrobe(StrobeCommand command, uint8_t initSpi)
{
	if (initSpi) { start(); }
	select();
	_miso.waitReset();
	uint8_t out = command;
	uint8_t in = 0;
	_spi.transfer(&out, &in, 1);
	deSelect();
	if (initSpi) { end(); }
}

uint8_t CC1101::readRegister(RegisterData& reg)
{
	std::array<uint8_t, 2> transfer = reg.toRead();
	std::array<uint8_t, 2> recive;
	start();
	select();
	_miso.waitReset();
	_spi.transfer(transfer, recive);
	deSelect();
	end();
	reg.fromRead(recive);
	return recive[1];
}

void CC1101::readBurstReg(uint8_t reg, uint8_t* data, uint8_t len)
{
	start();
	select();
	_miso.waitReset();
	reg |= READ_BURST;
	uint8_t result = 0;
	_spi.transfer(&reg, &result, 1);
	reg = 0;
	for (uint8_t i = 0; i < len; i++) { _spi.transfer(&reg, data + i, 1); }
	deSelect();
	end();
}

uint8_t CC1101::readStatus(uint8_t reg)
{
	uint8_t value = 0;
	readBurstReg(reg, &value, 1);
	return value;
}

void CC1101::setChannel(uint8_t ch) { setRegister(CC1101_CHANNR, _chan = ch); }

void CC1101::setPacketLength(uint8_t v) { setRegister(CC1101_PKTLEN, v); }

void CC1101::setCCMode(uint8_t ccmode)
{
	_ccmode = ccmode;
	if (ccmode == 1)
	{
		setRegister(CC1101_IOCFG2, 0x0B);
		setRegister(CC1101_IOCFG0, 0x06);
		setRegister(CC1101_PKTCTRL0, 0x05);
		setRegister(CC1101_MDMCFG3, 0xF8);
		setRegister(CC1101_MDMCFG4, 11 + _m4RxBw);
	}
	else
	{
		setRegister(CC1101_IOCFG2, 0x0D);
		setRegister(CC1101_IOCFG0, 0x0D);
		setRegister(CC1101_PKTCTRL0, 0x32);
		setRegister(CC1101_MDMCFG3, 0x93);
		setRegister(CC1101_MDMCFG4, 7 + _m4RxBw);
	}
	setModulation(_modulation);
}


void CC1101::setModulation(uint8_t modulation)
{
	_modulation = modulation > 4 ? 4 : modulation;
	Split_MDMCFG2();
	switch (_modulation)
	{
	case 0:
		_m2MODFM = 0x00;
		_frend0 = 0x10;
		break; // 2-FSK
	case 1:
		_m2MODFM = 0x10;
		_frend0 = 0x10;
		break; // GFSK
	case 2:
		_m2MODFM = 0x30;
		_frend0 = 0x11;
		break; // ASK
	case 3:
		_m2MODFM = 0x40;
		_frend0 = 0x10;
		break; // 4-FSK
	case 4:
		_m2MODFM = 0x70;
		_frend0 = 0x10;
		break; // MSK
	}
	setRegister(CC1101_MDMCFG2, _m2DCOFF + _m2MODFM + _m2MANCH + _m2SYNCM);
	setRegister(CC1101_FREND0, _frend0);
	setPA(_pa);
}

void CC1101::setPA(int p)
{
	_pa = p;
	uint8_t first = getMHzCase();
	uint8_t second = 0;
	_last_pa = first + 1;
	if (			 _pa <= -30) { second = 0; }
	if (_pa > -30 && _pa <= -20) { second = 1; }
	if (_pa > -20 && _pa <= -15) { second = 2; }
	if (_pa > -15 && _pa <= -10) { second = 3; }
	if (_pa > -10 && _pa <=  -6) { second = 4; }
	if (_pa >  -6 && _pa <=   0) { second = 5; }
	if (_pa >   0 && _pa <=   5) { second = 6; }
	if (_pa >   5 && _pa <=   7) { second = 7; }
	if (_pa >   7 && _pa <=  10) { second = 8; }
	if (_pa >   10			   ) { second = 9; }

	uint8_t a = PA_TABLE_CONST[first][second];

	if (_modulation == 2) { PA_TABLE[0] = 0; PA_TABLE[1] = a; }
	else { PA_TABLE[0] = a; PA_TABLE[1] = 0; }
	setBurstReg(CC1101_PATABLE, PA_TABLE.data(), 8);
}

void CC1101::setMHZ(float mhz)
{
	_mhz = mhz;

	uint8_t freq2 = 0;
	uint8_t freq1 = 0;
	uint8_t freq0 = 0;

	while (mhz > 0.00039675)
	{
		if (mhz >= 26) { mhz -= 26; freq2 += 1; }
	    else if (mhz >= 0.1015625) { mhz -= 0.1015625; freq1 += 1; }
	    else if (mhz >= 0.00039675) { mhz -= 0.00039675; freq0 += 1; }
	}

	setRegister(CC1101_FREQ2, freq2);
	setRegister(CC1101_FREQ1, freq1);
	setRegister(CC1101_FREQ0, freq0);

	calibrate();
}

void CC1101::calibrate()
{
	uint8_t mhzCase = getMHzCase();
	const CalibrateValues& func = CLB[mhzCase];
	setRegister(CC1101_FSCTRL0, func.getData(_mhz));
	if (func.useTreshold(_mhz))
	{
		setRegister(CC1101_TEST0, 0x09);
		uint8_t s = readStatus(CC1101_FSCAL2);
		if (s < 32) { setRegister(CC1101_FSCAL2, s + 32); }
	}
	else { setRegister(CC1101_TEST0, 0x0B); }
	if (_last_pa != (mhzCase + 1)) { setPA(_pa); }
}

void CC1101::setClb(uint8_t b, uint8_t s, uint8_t e)
{
	CalibrateValues& r = CLB[(uint8_t)(b - 1) % 4];
	r._minData = s;
	r._maxData = e;
}

uint8_t CC1101::getCC1101() { return (readStatus(CC1101_VERSION) > 0) ? 1 : 0; }

void CC1101::setSyncWord(uint8_t sh, uint8_t sl)
{
	setRegister(CC1101_SYNC1, sh);
	setRegister(CC1101_SYNC0, sl);
}

void CC1101::setAddr(uint8_t v) { setRegister(CC1101_ADDR, v); }

void CC1101::setPQT(uint8_t v)
{
	Split_PKTCTRL1();
	if (v > 7) { v = 7; }
	_pc1PQT = v * 32;
	setRegister(CC1101_PKTCTRL1, _pc1PQT + _pc1CRC_AF + _pc1APP_ST + _pc1ADRCHK);
}

void CC1101::setCRC_AF(uint8_t v)
{
	Split_PKTCTRL1();
	_pc1CRC_AF = v != 0 ? 8 : 0;
	setRegister(CC1101_PKTCTRL1, _pc1PQT + _pc1CRC_AF + _pc1APP_ST + _pc1ADRCHK);
}

void CC1101::setAppendStatus(uint8_t v)
{
	Split_PKTCTRL1();
	_pc1APP_ST = v != 0 ? 4 : 0;
	setRegister(CC1101_PKTCTRL1, _pc1PQT + _pc1CRC_AF + _pc1APP_ST + _pc1ADRCHK);
}

void CC1101::setAdrChk(uint8_t v)
{
	Split_PKTCTRL1();
	if (v > 3) { v = 3; }
	_pc1ADRCHK = v;
	setRegister(CC1101_PKTCTRL1, _pc1PQT + _pc1CRC_AF + _pc1APP_ST + _pc1ADRCHK);
}

void CC1101::setWhiteData(uint8_t v)
{
	Split_PKTCTRL0();
	_pc0WDATA = v != 0 ? 64 : 0;
	setRegister(CC1101_PKTCTRL0, _pc0WDATA + _pc0PktForm + _pc0CRC_EN + _pc0LenConf);
}

void CC1101::setPktFormat(uint8_t v)
{
	Split_PKTCTRL0();
	if (v > 3) { v = 3; }
	_pc0PktForm = v * 16;
	setRegister(CC1101_PKTCTRL0, _pc0WDATA + _pc0PktForm + _pc0CRC_EN + _pc0LenConf);
}

void CC1101::setCrc(uint8_t v)
{
	Split_PKTCTRL0();
	_pc0CRC_EN = v != 0 ? 4 : 0;
	setRegister(CC1101_PKTCTRL0, _pc0WDATA + _pc0PktForm + _pc0CRC_EN + _pc0LenConf);
}

void CC1101::setLengthConfig(uint8_t v)
{
	Split_PKTCTRL0();
	if (v > 3) { v = 3; }
	_pc0LenConf = v;
	setRegister(CC1101_PKTCTRL0, _pc0WDATA + _pc0PktForm + _pc0CRC_EN + _pc0LenConf);
}

void CC1101::setRxBW(float f)
{
	Split_MDMCFG4();
	int s1 = 3;
	int s2 = 3;
	for (int i = 0; i < 3; i++)
	{
		if (f > 101.5625) {  f /= 2; s1--; }
		else { break; }
	}
	for (int i = 0; i < 3; i++)
	{
		if (f > 58.1) { f /= 1.25; s2--; }
		else { break; }
	}
	s1 *= 64;
	s2 *= 16;
	_m4RxBw = s1 + s2;
	setRegister(CC1101_MDMCFG4, _m4RxBw + _m4DaRa);
}

void CC1101::setDRate(float kD)
{
	Split_MDMCFG4();
	float c = kD;
	uint8_t MDMCFG3 = 0;
	if (c > 1621.83) { c = 1621.83; }
	if (c < 0.0247955) { c = 0.0247955; }
	_m4DaRa = 0;
	for (int i = 0; i < 20; i++)
	{
		if (c <= 0.0494942)
		{
			c = c - 0.0247955;
			c = c / 0.00009685;
			MDMCFG3 = c;
			float s1 = (c - MDMCFG3) * 10;
			if (s1 >= 5) { MDMCFG3++; }
			break;
		}
		else { _m4DaRa++; c = c / 2; }
	}
	setRegister(CC1101_MDMCFG4, _m4RxBw + _m4DaRa);
	setRegister(CC1101_MDMCFG3, MDMCFG3);
}

void CC1101::setDcFilterOff(uint8_t v)
{
	Split_MDMCFG2();
	_m2DCOFF = v != 0 ? 128 : 0;
	setRegister(CC1101_MDMCFG2, _m2DCOFF + _m2MODFM + _m2MANCH + _m2SYNCM);
}

void CC1101::setManchester(uint8_t v)
{
	Split_MDMCFG2();
	_m2MANCH = v != 0 ? 8 : 0;
	setRegister(CC1101_MDMCFG2, _m2DCOFF + _m2MODFM + _m2MANCH + _m2SYNCM);
}

void CC1101::setSyncMode(uint8_t v)
{
	Split_MDMCFG2();
	if (v > 7) { v = 7; }
	_m2SYNCM = v;
	setRegister(CC1101_MDMCFG2, _m2DCOFF + _m2MODFM + _m2MANCH + _m2SYNCM);
}

void CC1101::setFEC(uint8_t v)
{
	Split_MDMCFG1();
	_m1FEC = v != 0 ? 128 : 0;
	setRegister(CC1101_MDMCFG1, _m1FEC + _m1PRE + _m1CHSP);
}

void CC1101::setPRE(uint8_t v)
{
	Split_MDMCFG1();
	if (v > 7) { v = 7; }
	_m1FEC = v * 16;
	setRegister(CC1101_MDMCFG1, _m1FEC + _m1PRE + _m1CHSP);
}

void CC1101::setChsp(float f)
{
	Split_MDMCFG1();
	uint8_t MDMCFG0 = 0;
	_m1CHSP = 0;
	if (f > 405.456543) { f = 405.456543; }
	if (f < 25.390625)  { f = 25.390625; }
	for (int i = 0; i < 5; i++)
	{
		if (f <= 50.682068)
		{
			f -= 25.390625;
			f /= 0.0991825;
			MDMCFG0 = f;
			float s1 = (f - MDMCFG0) * 10;
			if (s1 >= 5) { MDMCFG0++; }
			break;
		}
		else { _m1CHSP++; f /= 2; }
	}
	setRegister(CC1101_MDMCFG1, _m1CHSP + _m1FEC + _m1PRE);
	setRegister(CC1101_MDMCFG0, MDMCFG0);
}

void CC1101::setDeviation(float d)
{
	float f = 1.586914;
	float v = 0.19836425;
	int c = 0;
	if (d > 380.859375) { d = 380.859375; }
	if (d < 1.586914) { d = 1.586914; }
	for (int i = 0; i < 255; i++)
	{
		f += v;
		if (c == 7) { v *= 2; c = -1; i += 8; }
		if (f >= d) { c = i; break; }
		c++;
	}
	setRegister(CC1101_DEVIATN, c);
}

void CC1101::Split_PKTCTRL1()
{
	int calc = readStatus(CC1101_PKTCTRL1);
	_pc1PQT = 0;
	_pc1CRC_AF = 0;
	_pc1APP_ST = 0;
	_pc1ADRCHK = 0;
	while (true)
	{
		if (calc >= 32) { calc -= 32; _pc1PQT += 32; }
		else if (calc >= 8) { calc -= 8; _pc1CRC_AF += 8; }
		else if (calc >= 4) { calc -= 4; _pc1APP_ST += 4; }
		else { _pc1ADRCHK = calc; break; }
	}
}

void CC1101::Split_PKTCTRL0()
{
	int calc = readStatus(CC1101_PKTCTRL0);
	_pc0WDATA = 0;
	_pc0PktForm = 0;
	_pc0CRC_EN = 0;
	_pc0LenConf = 0;
	while (true)
	{
		if (calc >= 64) { calc -= 64; _pc0WDATA += 64;  }
		else if (calc >= 16) { calc -= 16; _pc0PktForm += 16; }
		else if (calc >= 4) { calc -= 4; _pc0CRC_EN += 4; }
		else { _pc0LenConf = calc; break; }
	}
}

void CC1101::Split_MDMCFG4()
{
	int calc = readStatus(CC1101_MDMCFG4);
	_m4RxBw = 0;
	_m4DaRa = 0;
	while (true)
	{
		if (calc >= 64) { calc -= 64; _m4RxBw += 64; }
		else if (calc >= 16) { calc -= 16; _m4RxBw += 16; }
		else { _m4DaRa = calc; break; }
	}
}
void CC1101::Split_MDMCFG2()
{
	int calc = readStatus(CC1101_MDMCFG2);
	_m2DCOFF = 0;
	_m2MODFM = 0;
	_m2MANCH = 0;
	_m2SYNCM = 0;
	while (true)
	{
		if (calc >= 128) { calc -= 128; _m2DCOFF += 128; }
		else if (calc >= 16) { calc -= 16; _m2MODFM += 16; }
		else if (calc >= 8) { calc -= 8; _m2MANCH += 8; }
		else { _m2SYNCM = calc; break; }
	}
}

void CC1101::Split_MDMCFG1()
{
	int calc = readStatus(CC1101_MDMCFG1);
	_m1FEC = 0;
	_m1PRE = 0;
	_m1CHSP = 0;
	while (true)
	{
		if (calc >= 128) { calc -= 128; _m1FEC += 128; }
		else if (calc >= 16) { calc -= 16; _m1PRE += 16; }
		else { _m1CHSP = calc; break; }
	}
}

void CC1101::setDefualtSettings()
{
	setRegister(CC1101_FSCTRL1, 0x06);

	setCCMode(_ccmode);
	setMHZ(_mhz);

	setRegister(CC1101_MDMCFG1, 0x02);
	setRegister(CC1101_MDMCFG0, 0xF8);
	setRegister(CC1101_CHANNR, _chan);
	setRegister(CC1101_DEVIATN, 0x47);
	setRegister(CC1101_FREND1, 0x56);
	setRegister(CC1101_MCSM0, 0x18);
	setRegister(CC1101_FOCCFG, 0x16);
	setRegister(CC1101_BSCFG, 0x1C);
	setRegister(CC1101_AGCCTRL2, 0xC7);
	setRegister(CC1101_AGCCTRL1, 0x00);
	setRegister(CC1101_AGCCTRL0, 0xB2);
	setRegister(CC1101_FSCAL3, 0xE9);
	setRegister(CC1101_FSCAL2, 0x2A);
	setRegister(CC1101_FSCAL1, 0x00);
	setRegister(CC1101_FSCAL0, 0x1F);
	setRegister(CC1101_FSTEST, 0x59);
	setRegister(CC1101_TEST2, 0x81);
	setRegister(CC1101_TEST1, 0x35);
	setRegister(CC1101_TEST0, 0x09);
	setRegister(CC1101_PKTCTRL1, 0x04);
	setRegister(CC1101_ADDR, 0x00);
	setRegister(CC1101_PKTLEN, 0x00);
}

void CC1101::SetTx()
{
	setStrobe(CC1101_SIDLE);
	setStrobe(CC1101_STX); //start send
	_trxstate = State::TX;
}

void CC1101::SetRx()
{
	setStrobe(CC1101_SIDLE);
	setStrobe(CC1101_SRX); //start receive
	_trxstate = State::RX;
}

void CC1101::SetTx(float mhz)
{
	setStrobe(CC1101_SIDLE);
	setMHZ(mhz);
	setStrobe(CC1101_STX); //start send
	_trxstate = State::TX;
}

void CC1101::SetRx(float mhz)
{
	setStrobe(CC1101_SIDLE);
	setMHZ(mhz);
	setStrobe(CC1101_SRX); //start receive
	_trxstate = State::RX;
}

int CC1101::getRssi()
{
	int rssi;
	rssi = readStatus(CC1101_RSSI);
	if (rssi >= 128) { rssi = (rssi - 256) / 2 - 74; }
	else { rssi = (rssi / 2) - 74; }
	return rssi;
}

uint8_t CC1101::getLqi() { return readStatus(CC1101_LQI); }

void CC1101::setSres()
{
	setStrobe(CC1101_SRES);
	_trxstate = State::NONE;
}

void CC1101::setSidle()
{
	setStrobe(CC1101_SIDLE);
	_trxstate = State::NONE;
}

void CC1101::goSleep()
{
	_trxstate = State::NONE;
	setStrobe(CC1101_SIDLE); //Exit RX / TX, turn off frequency synthesizer and exit
	setStrobe(CC1101_SPWD); //Enter power down mode when CSn goes high.
}


void CC1101::SendData(const char *txchar)
{
	int len = strlen(txchar);
	SendData((uint8_t*)txchar, len);
}

void CC1101::SendData(uint8_t *txBuffer, uint8_t size)
{
	logText("Transfer len ");
	logNumber(size);
	logText("\n");
	sendRegister(CC1101_TXFIFO, size);
	setBurstReg(CC1101_TXFIFO, txBuffer, size); //write data to send
	setStrobe(CC1101_SIDLE);
	setStrobe(CC1101_STX); //start send
	await(size);
	setStrobe(CC1101_SFTX); //flush TXfifo
	_trxstate = State::TX;
}

uint8_t CC1101::CheckCRC()
{
	uint8_t lqi = readStatus(CC1101_LQI);
	bool crc_ok = lqi & (1 << 7);
	if (crc_ok != 0) { return 1; }
	else
	{
		setStrobe(CC1101_SFRX);
		setStrobe(CC1101_SRX);
		return 0;
	}
}

} /* namespace IntroSatLib */

#endif
