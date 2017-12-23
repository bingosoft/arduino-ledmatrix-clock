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

	if ((position + DOTS_PER_CHAR) / 8 != device) {
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

int LedMatrix::renderChar(char c, int position)
{
	int charWidth = DOTS_PER_CHAR;
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
		case '+': render(PLUS, position); break;
		case '-': render(MINUS, position); break;
		case 'a': render(A, position); break;
		case 'b': render(B, position); break;
		case 'c': render(C, position); break;
		case 'd': render(D, position); break;
		case 'e': render(E, position); break;
		case 'f': render(F, position); break;
		case 'g': render(G, position); break;
		case 'h': render(H, position); break;
		case 'i': charWidth = 1; render(I, position, charWidth); break;
		case 'j': render(J, position); break;
		case 'k': render(K, position); break;
		case 'l': render(L, position); break;
		case 'm': render(M, position); break;
		case 'n': render(N, position); break;
		case 'o': render(O, position); break;
		case 'p': render(P, position); break;
		case 'q': render(Q, position); break;
		case 'r': render(R, position); break;
		case 's': render(S, position); break;
		case 't': render(T, position); break;
		case 'u': render(U, position); break;
		case 'v': render(V, position); break;
		case 'w': render(W, position); break;
		case 'x': render(X, position); break;
		case 'y': render(Y, position); break;
		case 'z': render(Z, position); break;
		default:
			break;
	}
	return charWidth;
}

void LedMatrix::renderUtf8Char(int c, int position)
{
	using namespace ledmatrix;

	switch (c) {
		case 'а': render(A, position); break;
		case 'б': render(RUS_B, position); break;
		case 'в': render(RUS_V, position); break;
		case 'г': render(G, position); break;
		case 'д': render(RUS_D, position); break;
		case 'е': render(E, position); break;
		case 'ё': render(E, position); break;
		case 'ж': render(ZH, position); break;
		case 'з': render(RUS_Z, position); break;
		case 'и': render(RUS_I, position); break;
		case 'й': render(IY, position); break;
		case 'к': render(RUS_K, position); break;
		case 'л': render(RUS_L, position); break;
		case 'м': render(RUS_M, position); break;
		case 'н': render(RUS_N, position); break;
		case 'о': render(O, position); break;
		case 'п': render(RUS_P, position); break;
		case 'р': render(P, position); break;
		case 'с': render(C, position); break;
		case 'т': render(RUS_T, position); break;
		case 'у': render(Y, position); break;
		case 'ф': render(RUS_F, position); break;
		case 'х': render(X, position); break;
		case 'ц': render(TS, position); break;
		case 'ч': render(CH, position); break;
		case 'ш': render(SH, position); break;
		case 'щ': render(SCH, position); break;
		case 'ъ': render(HARDSIGN, position); break;
		case 'ы': render(YI, position); break;
		case 'ь': render(SOFTSIGN, position); break;
		case 'э': render(YE, position); break;
		case 'ю': render(YU, position); break;
		case 'я': render(YA, position); break;
		case '°': render(DEGREE, position); break;
	}
}

int LedMatrix::utf8len(const String &s) const
{
	int length = 0;

	for (int i = 0; i < s.length(); ++i) {
		if ((s[i] & 0xE0) == 0xC0) { // 2 bytes
			i += 1;
		} else if ((s[i] & 0xF0) == 0xE0) { // 3 bytes
			i += 2;
		} else if ((s[i] & 0XF8) == 0XF0) { // 4 bytes
			i += 3;
		}
		length++;
	}

	return length;
}

void LedMatrix::renderString(const String &s, int position, int space)
{
	int utf8Char;

	for (int i = 0; i < s.length(); ++i) {
		int c = s[i];
		int charWidth = DOTS_PER_CHAR;

		if ((c & 0xE0) == 0xC0) { // 2 bytes
			utf8Char = s[i] << 8 | s[i + 1];
			++i;
			renderUtf8Char(utf8Char, position);
		} else {
			charWidth = renderChar(s[i], position);
		}

		position += charWidth + space;
		if (position > LINE) {
			return;
		}
	}
}

void LedMatrix::renderStringInCenter(const String &s, int duration)
{
	int wordLength = min(utf8len(s) * (DOTS_PER_CHAR + 1) - 1, 32);

	renderString(s, (32 - wordLength) / 2 + 1);
	delay(duration);
}

void LedMatrix::renderFloatingText(const String &s, int duration, int updateDelay)
{
	int wordLength = utf8len(s) * (DOTS_PER_CHAR + 1) - 1;

	if (wordLength < 32) {
		renderString(s, (32 - wordLength) / 2 + 1);
		delay(duration);
		return;
	}

	int pos = 0;
	int start = millis();

	while ((millis() - start) < duration) {
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
