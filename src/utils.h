#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

inline void print(const String &s) {
	Serial.println(s);
}

#endif