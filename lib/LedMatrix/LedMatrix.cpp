#include "LedMatrix.h"
#include "LedMatrixChars.h"

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

	for (int i = 0; i < DEVICES; ++i) {
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
    int maxbytes = DEVICES * 2;
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
	for (int i = 0; i < DEVICES; ++i) {
	 	setIntensity(i, intensity);
	}
}

void LedMatrix::setIntensity(int device, int intensity) // 0..15
{
	spiTransfer(device, OP_INTENSITY, intensity);
}

void LedMatrix::clearDisplay()
{
    for (int i = 0; i < DEVICES; ++i) {
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

void LedMatrix::render(const byte *rows, int position)
{
	for (int i = 0; i < 8; ++i) {
		int value = rows[i];

		for (int j = 0; j < DOTS_PER_CHAR; ++j) {
			state[i * LINE + j + position] = value & (1 << (DOTS_PER_CHAR - j - 1));
		}
	}
	int device = position / 8;
	update(device);

	if ((position + DOTS_PER_CHAR) / 8 != device) {
		update(device + 1); // near section affected
	}
}

void LedMatrix::turnLed(int position, int row, bool on)
{
	state[row * DEVICES * 8 + position] = on;
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

void LedMatrix::renderChar(char c, int position)
{
	using namespace ledmatrix;

	switch (c) {
	case '0': render(_0, position);	break;
	case '1': render(_1, position); break;
	case '2': render(_2, position); break;
	case '3': render(_3, position); break;
	case '4': render(_4, position); break;
	case '5': render(_5, position); break;
	case '6': render(_6, position); break;
	case '7': render(_7, position); break;
	case '8': render(_8, position); break;
	case '9': render(_9, position); break;
	case ' ': render(SPACE, position); break;
	default:
		Serial.println("char not found");
		break;
	}
}
