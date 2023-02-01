#include "LedMatrix.h"
#include "LedMatrixChars.h"
#include "CharInfo.h"

#define OP_DECODEMODE  9
#define OP_INTENSITY   10
#define OP_SCANLIMIT   11
#define OP_SHUTDOWN    12
#define OP_DISPLAYTEST 15

LedMatrix::LedMatrix(int din, int clk, int cs)
	: DIN(din), CLK(clk), CS(cs)
{
	Serial.printf("Setup led matrix on digital pins MOSI - %d, CLK - %d, CS - %d\n", din, clk, cs);
    pinMode(din, OUTPUT);
    pinMode(clk, OUTPUT);
    pinMode(cs, OUTPUT);
    digitalWrite(cs, HIGH);

	for (int i = 0; i < SECTIONS; ++i) {
	 	spiTransfer(i, OP_DISPLAYTEST, 0);
	 	setScanLimit(i, 7);
	 	spiTransfer(i, OP_DECODEMODE, 0);
	 	clearDisplay(i);
		shutdown(i, false);
	}
}

void LedMatrix::spiTransfer(int device, byte opcode, byte data)
{
    int offset = device * 2;
    int maxbytes = SECTIONS * 2;
    byte spidata[16];

    for (int i = 0; i < maxbytes; ++i) {
		spidata[i] = 0;
	}

    spidata[offset + 1] = opcode;
    spidata[offset] = data;
    //enable the line
    digitalWrite(CS, LOW);
    //Now shift out the data
    for (int i = maxbytes; i > 0; --i) {
	 	shiftOut(DIN, CLK, MSBFIRST, spidata[i - 1]);
	}
    //latch the data onto the display
    digitalWrite(CS, HIGH);
}

void LedMatrix::shutdown(int device, bool status)
{
	spiTransfer(device, OP_SHUTDOWN, !status);
}

void LedMatrix::setScanLimit(int device, int limit)
{
	spiTransfer(device, OP_SCANLIMIT, limit);
}

void LedMatrix::setIntensity(int intensity)
{
	for (int i = 0; i < SECTIONS; ++i) {
	 	setIntensity(i, intensity);
	}
}

void LedMatrix::setIntensity(int device, int intensity) // 0..15
{
	spiTransfer(device, OP_INTENSITY, intensity);
}

void LedMatrix::clearDisplay()
{
    for (int i = 0; i < SECTIONS; ++i) {
		clearDisplay(i);
    }
}

void LedMatrix::clearDisplay(int device)
{
    for (int i = 0; i < 8; i++) {
		spiTransfer(device, i + 1, 0);

		for (int j = 0; j < 8; ++j) {
			state[i * LINE + device * 8 + j] = false;
		}
    }
}

void LedMatrix::render(const byte *rows, int position, int charWidth)
{
	for (int i = 0; i < 8; ++i) {
		int value = rows[i];

		for (int j = 0; j < charWidth; ++j) {
			int pos = j + position;
			if (pos > LINE || pos < 0)
				continue;

			state[i * LINE + pos] = value & (1 << (charWidth - j - 1));
		}
	}
	int device = position / 8;
	update(device);

	if ((position + charWidth) / 8 != device) {
		update(device + 1); // near section affected
	}
}

void LedMatrix::turnLed(int position, int row, bool on)
{
	state[row * SECTIONS * 8 + position] = on;
	update(position / 8);
}

void LedMatrix::update(int device)
{
	for (int i = 0; i < 8; ++i) {
		int val = 0;
		for (int j = 0; j < 8; ++j) {
			bool on = state[i * LINE + j + device * 8];
			val |= on << j;
		}
		spiTransfer(device, 8 - i, val);
	}
}

int LedMatrix::renderChar(int c, int position)
{
	CharInfo info = CharInfo::byChar(c);
	if (info.dots) {
		render(info.dots, position, info.charWidth);
	} else {
		Serial.println("char not found");
	}
	return info.charWidth;
}

int LedMatrix::stringLengthInDots(const String &s) const
{
	int c;
	int dots = 0;

	for (int i = 0; i < (int)s.length(); ++i) {
		if ((s[i] & 0xE0) == 0xC0) {
			c = s[i] << 8 | s[i + 1];
			i += 1;
		} else {
			c = s[i];
		}
		CharInfo info = CharInfo::byChar(c);
		dots += info.charWidth + 1; // + SPACE
	}

	return dots - 1; // minus last space
}

void LedMatrix::renderString(const String &s, int position, int space)
{
	for (int i = 0; i < (int)s.length(); ++i) {
		int c = s[i];

		if ((c & 0xE0) == 0xC0) { // 2 bytes
			c = s[i] << 8 | s[i + 1];
			++i;
		}
		int charWidth = renderChar(c, position);
		position += charWidth + space;

		if (position > LINE) {
			return;
		}
	}
}

void LedMatrix::renderStringInCenter(const String &s, int duration)
{
	int wordLength = min(stringLengthInDots(s), 32);

	renderString(s, (32 - wordLength) / 2 + 1);
	delay(duration);
}

void LedMatrix::renderFloatingText(const String &s, int duration, int updateDelay)
{
	int wordLength = stringLengthInDots(s);

	if (wordLength < 32) {
		renderString(s, (32 - wordLength) / 2 + 1);
		delay(duration);
		return;
	}

	int pos = 0;
	auto start = millis();

	while ((millis() - start) < (unsigned long)duration) {
		clearDisplay();
		renderString(s, pos);
		delay(pos == 0 ? 1000 : updateDelay);
		pos--;
		if (pos < (32 - wordLength)) {
			pos = 0;
			delay(1000);
		}
	}
}
