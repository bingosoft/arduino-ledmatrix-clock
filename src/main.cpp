#include <Arduino.h>
#include <SoftwareSerial.h>
#include <LedMatrix.h>

const int DATA_IN = 4;
const int CS = 5;
const int CLK = 6;

LedMatrix l(DATA_IN, CLK, CS);
bool dotVisible = false;
int hours = 22;
int minutes = 59;
int seconds = 45;

void setup()
{
	Serial.begin(9600);
 	l.setIntensity(0);
}

void loop()
{
	l.turnLed(16, 7, dotVisible);
	l.turnLed(15, 7, dotVisible);
	dotVisible = !dotVisible;

	if (!dotVisible) {
		seconds++;
	}

	if (seconds >= 60) {
		seconds = 0;
		minutes++;

		if (minutes >= 60) {
			minutes = 0;
			hours++;
		}

		if (hours >= 24) {
			hours = 0;
		}
	}

	l.renderChar(hours / 10 + 0x30, 2);
	l.renderChar(hours % 10 + 0x30, 9);
	l.renderChar(minutes / 10 + 0x30, 18);
	l.renderChar(minutes % 10 + 0x30, 25);

	delay(500);
}
