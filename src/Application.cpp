#include "Application.h"

Application::Application()
{
	Serial.println("Init app...");
	l = new LedMatrix(DATA_IN, CLK, CS);
 	l->setIntensity(5);
}

void Application::updateTime()
{
	l->turnLed(16, 7, dotVisible);
	l->turnLed(15, 7, dotVisible);
	dotVisible = !dotVisible;

	if (!dotVisible) {
		seconds++;
		Serial.printf("%02d:%02d:%02d\n", hours, minutes, seconds);
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

	l->renderChar(hours / 10 + 0x30, 2);
	l->renderChar(hours % 10 + 0x30, 9);
	l->renderChar(minutes / 10 + 0x30, 18);
	l->renderChar(minutes % 10 + 0x30, 25);
}

void Application::exec()
{
	updateTime();
	delay(500);
}
