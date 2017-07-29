#include <Arduino.h>
#include <SoftwareSerial.h>
#include <LedMatrix.h>

const int DATA_IN = 4;
const int CS = 5;
const int CLK = 6;
const int SECTIONS = 4;

LedMatrix l(DATA_IN, CLK, CS, SECTIONS);

void setup()
{
	Serial.begin(9600);
 	l.setIntensity(0);
}

void loop()
{
	l.renderChar('5', 5);
}
