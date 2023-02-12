#include "LedMatrix.h"
#include "LedMatrixChars.h"

LedMatrix::LedMatrix(int din, int clk, int cs)
	: dataInPin(din), clkPin(clk), csPin(cs)
{
	Serial.printf("Setup led matrix on digital pins MOSI - %d, CLK - %d, CS - %d\n", din, clk, cs);

    pinMode(din, OUTPUT);
    pinMode(clk, OUTPUT);
    pinMode(cs, OUTPUT);
    digitalWrite(cs, HIGH);

	for (int i = 0; i < sections; ++i) {
	 	spiTransfer(i, DisplayTest, 0);
	 	setScanLimit(i, 7);
	 	spiTransfer(i, DecodeMode, 0);
	 	clearDisplay(i);
		shutdown(i, false);
	}
}

void LedMatrix::spiTransfer(int device, Operation opcode, byte data) {
	spiTransfer(device, static_cast<int>(opcode), data);
}

void LedMatrix::spiTransfer(int device, int row, byte data)
{
    int offset = device * 2;
    int maxbytes = sections * 2;
    byte spidata[16];

    for (int i = 0; i < maxbytes; ++i) {
		spidata[i] = 0;
	}

    spidata[offset + 1] = row;
    spidata[offset] = data;
    //enable the line
    digitalWrite(csPin, LOW);
    //Now shift out the data
    for (int i = maxbytes; i > 0; --i) {
	 	shiftOut(dataInPin, clkPin, MSBFIRST, spidata[i - 1]);
	}
    //latch the data onto the display
    digitalWrite(csPin, HIGH);
}

void LedMatrix::shutdown(int device, bool status)
{
	spiTransfer(device, ShutDown, !status);
}

void LedMatrix::setScanLimit(int device, int limit)
{
	spiTransfer(device, ScanLimit, limit);
}

void LedMatrix::setIntensity(int intensity)
{
	for (int i = 0; i < sections; ++i) {
	 	setIntensity(i, intensity);
	}
}

void LedMatrix::setIntensity(int device, int intensity) // 0..15
{
	spiTransfer(device, Intencity, intensity);
}

void LedMatrix::clearDisplay()
{
    for (int i = 0; i < sections; ++i) {
		clearDisplay(i);
    }
}

void LedMatrix::clearDisplay(int device)
{
    for (int i = 0; i < 8; i++) {
		spiTransfer(device, i + 1, 0);

		for (int j = 0; j < 8; ++j) {
			state[i * lineInDots + device * 8 + j] = false;
		}
    }
}

void LedMatrix::render(const byte *rows, int position, int charWidth, bool updateDisplay)
{
	for (int i = 0; i < 8; ++i) {
		int value = rows[i];

		for (int j = 0; j < charWidth; ++j) {
			int pos = j + position;
			if (pos >= lineInDots || pos < 0)
				continue;

			state[i * lineInDots + pos] = value & (1 << (charWidth - j - 1));
		}
	}
	if (updateDisplay) {
		int section = position / 8;
		update(section);

		if ((position + charWidth) / 8 != section) {
			update(section + 1); // near section affected
		}
	}
}

void LedMatrix::turnLed(int position, int row, bool on)
{
	state[row * lineInDots + position] = on;
	update(position / 8);
}

void LedMatrix::update(int section)
{
	if (section > sections)
		return;

	for (int i = 0; i < 8; ++i) {
		int val = 0;
		for (int j = 0; j < 8; ++j) {
			bool on = state[i * lineInDots + j + section * 8];
			val |= on << j;
		}
		spiTransfer(section, 8 - i, val);
	}
}

int LedMatrix::renderChar(int c, int position, bool updateDisplay)
{
	auto info = mapping.getDotsByChar(c);

	if (!info.has_value()) {
		Serial.printf("Char %c not found, skip rendering\n", (char)c);
		return 0;
	}

	render(info->dots, position, info->charWidth, updateDisplay);
	return info->charWidth;
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
			c = toLowerCase(s[i]);
		}
		auto info = mapping.getDotsByChar(c);
		int charWidth = info.has_value() ? info->charWidth : 0;
		dots += charWidth + 1; // + SPACE
	}

	return dots - 1; // minus last space
}

void LedMatrix::renderString(const String &s, int position)
{
	for (int i = 0; i < sections * 8 * 8; ++i) {
		state[i] = false; // clear all
	}

	for (int i = 0; i < (int)s.length(); ++i) {
		int c = toLowerCase(s[i]);

		if ((c & 0xE0) == 0xC0) { // 2 bytes
			c = s[i] << 8 | s[i + 1];
			++i;
		}
		int charWidth = renderChar(c, position, false);
		position += charWidth + 1;

		if (position > lineInDots) {
			break;
		}
	}

	for (int i = 0; i < sections; ++i) {
		update(i);
	}
}

void LedMatrix::renderStringInCenter(const String &s, int duration)
{
	int wordLength = min(stringLengthInDots(s), lineInDots);

	renderString(s, (lineInDots - wordLength) / 2 + 1);
	delay(duration);
}

void LedMatrix::renderFloatingText(const String &s, int startDelay, int speedDelay)
{
	int wordLength = stringLengthInDots(s);

	if (wordLength <= lineInDots) {
		renderString(s, (lineInDots - wordLength) / 2);
		delay(startDelay * 2);
		return;
	}

	int pos = 0;
	bool allRendered = false;

	do {
		renderString(s, pos);
		delay(pos == 0 ? startDelay : speedDelay);
		pos--;

		if (pos < (lineInDots - wordLength)) {
			pos = 0;
			delay(startDelay);
			allRendered = true;
		}
	} while (pos == 0 || !allRendered);
}
