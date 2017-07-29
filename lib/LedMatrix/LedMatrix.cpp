#include "LedMatrix.h"
#include "LedMatrixChars.h"

#define OP_DECODEMODE  9
#define OP_INTENSITY   10
#define OP_SCANLIMIT   11
#define OP_SHUTDOWN    12
#define OP_DISPLAYTEST 15

LedMatrix::LedMatrix(int din, int clk, int cs, int devices)
	: DIN(din), CLK(clk), CS(cs), devices(devices)
{
    pinMode(din, OUTPUT);
    pinMode(clk, OUTPUT);
    pinMode(cs, OUTPUT);
    digitalWrite(cs, HIGH);

	for (int i = 0; i < devices; ++i) {
	 	spiTransfer(i, OP_DISPLAYTEST, 0);
	 	setScanLimit(i, 7);
	 	spiTransfer(i, OP_DECODEMODE, 0);
	 	clearDisplay(i);
		shutdown(i, false);
	}
}

void LedMatrix::spiTransfer(int device, byte opcode, byte data)
{
    //Create an array with the data to shift out
    int offset = device * 2;
    int maxbytes = devices * 2;
    byte spidata[16];

    for(int i = 0; i < maxbytes; ++i) {
		spidata[i]=(byte)0;
	}
    //put our device data into the array
    spidata[offset + 1]=opcode;
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

void LedMatrix::setIntensity(int intensity) {
	for (int i = 0; i < devices; ++i) {
	 	setIntensity(i, 0);
	}
}

void LedMatrix::setIntensity(int device, int intensity) // 0..15
{
	spiTransfer(device, OP_INTENSITY, intensity);
}

void LedMatrix::clearDisplay(int device)
{
    for(int i = 0; i < 8; i++) {
		spiTransfer(device, i + 1, 0);
    }
}

void LedMatrix::render(const byte *rows, int position)
{
	for (int i = 0; i < 8; ++i) {
		int value = rows[7 - i];
		int flipped = 0;

		for (int i = 0; i < DOTS_PER_CHAR; ++i) {
			flipped |= (value & 1) << (DOTS_PER_CHAR - 1 - i);
			value >>= 1;
		}
		spiTransfer(0, i + 1, flipped);
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
	default:
		Serial.println("char not found");
		break;
	}
}
