#ifndef UTILS_H
#define UTILS_H
#include <Arduino.h>

inline String ucs2ToUtf8(const String &hex) {
	String res;

	for (int i = 0; i < (int)hex.length() / 4; ++i) {
		String seq = hex.substring(4 * i, 4 * (i + 1));
		long val = strtol(seq.c_str(), 0, 16);
		if (val < 0x80) {
			res += char(val);
		} else {
			res += char(0xC0 | ((val >> 6) & 0x1F));
			res += char(0x80 | (val & 0x3F));
		}
	}

	return res;
}

inline void print(const String &s) {
	Serial.println(s);
}

inline String toHex(int val) {
	char symbols[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	String res = val < 0 ? "-" : "";

	while (val != 0) {
		res = symbols[abs(val % 16)] + res;
		val /= 16;
	}

	while (res.length() < 2) {
		res = 0 + res;
	}

	return res;
}

inline String doubleToStr(double val) {
	char buffer[15];
	dtostrf(val, 0, 6, buffer);
	return String(buffer);
}

#endif
