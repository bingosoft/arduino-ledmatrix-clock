#ifndef LEDMATRIXCHARSOTHERSYMBOLS_H
#define LEDMATRIXCHARSOTHERSYMBOLS_H

// Other Symbols

namespace ledmatrix {
	typedef unsigned char byte;

	const static byte _0[8] = {
		B01110,
		B10001,
		B10001,
		B10001,
		B10001,
		B10001,
		B10001,
		B01110,
	};

	const static byte _1[8] = {
		B00100,
		B01100,
		B10100,
		B00100,
		B00100,
		B00100,
		B00100,
		B11111,
	};

	const static byte _2[8] = {
		B01110,
		B10001,
		B00001,
		B00001,
		B00010,
		B00100,
		B01000,
		B11111,
	};

	const static byte _3[8] = {
		B01110,
		B10001,
		B00001,
		B01110,
		B00001,
		B00001,
		B10001,
		B01110,
	};

	const static byte _4[8] = {
		B00010,
		B00110,
		B01010,
		B10010,
		B11111,
		B00010,
		B00010,
		B00010,
	};

	const static byte _5[8] = {
		B11111,
		B10000,
		B10000,
		B11110,
		B00001,
		B00001,
		B00001,
		B11110,
	};

	const static byte _6[8] = {
		B01110,
		B10001,
		B10000,
		B11110,
		B10001,
		B10001,
		B10001,
		B01110,
	};

	const static byte _7[8] = {
		B11111,
		B00001,
		B00001,
		B00001,
		B00010,
		B00100,
		B01000,
		B10000,
	};

	const static byte _8[8] = {
		B01110,
		B10001,
		B10001,
		B01110,
		B10001,
		B10001,
		B10001,
		B01110,
	};

	const static byte _9[8] = {
		B01110,
		B10001,
		B10001,
		B10001,
		B01111,
		B00001,
		B10001,
		B01110,
	};

	const static byte SPACE[8] = {
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
	};

	const static byte PLUS[8] = {
		B00000,
		B00000,
		B00100,
		B00100,
		B11111,
		B00100,
		B00100,
		B00000,
	};

	const static byte MINUS[8] = {
		B0000,
		B0000,
		B0000,
		B0000,
		B1111,
		B0000,
		B0000,
		B0000,
	};

	const static byte UNDERSCORE[8] = {
		B00000,
		B00000,
		B00000,
		B00000,
		B00000,
		B00000,
		B00000,
		B11111,
	};

	const static byte SLASH[8] = {
		B000,
		B000,
		B000,
		B000,
		B001,
		B010,
		B100,
		B000,
	};

	const static byte DEGREE[8] = {
		B010,
		B101,
		B010,
		B000,
		B000,
		B000,
		B000,
		B000,
	};

	const static byte PERCENT[8] = {
		B010000,
		B101001,
		B010010,
		B000100,
		B001000,
		B010010,
		B100101,
		B000010,
	};

	const static byte DOT[8] = {
		B0,
		B0,
		B0,
		B0,
		B0,
		B0,
		B0,
		B1,
	};

	const static byte ARROW_UP[8] = {
		B000,
		B000,
		B010,
		B111,
		B010,
		B010,
		B010,
		B000,
	};

	const static byte ARROW_DOWN[8] = {
		B000,
		B000,
		B010,
		B010,
		B010,
		B111,
		B010,
		B000,
	};
}

#endif
