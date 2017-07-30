#include "Application.h"
#include <NTPClient.h>
#include <ESP8266WiFi.h>

Application::Application()
	: l(DATA_IN, CLK, CS)
{
	Serial.println("Init app...");
 	l.setIntensity(5);

	int i = 0;

	WiFi.begin(ssid, password);

	while (!WiFi.isConnected()) {
		l.turnLed(i % 32, i / 32, true);
		++i;
		Serial.print(".");
		if (i >= 16) {
			Serial.printf("\nNo response from access point\n");
			i = 0;
			WiFi.disconnect();
			WiFi.begin(ssid, password);
			l.clearDisplay();
		}
		delay(500);
	}

	Serial.printf("\nConnected to Wi-Fi - %s\n", WiFi.localIP().toString().c_str());
	l.clearDisplay();
	delay(1000);
	Serial.println("Waiting for NTP sync...");
	ntp.update();
}

void Application::updateTime()
{
	l.turnLed(16, 7, dotVisible);
	l.turnLed(15, 7, dotVisible);
	dotVisible = !dotVisible;

	if (!dotVisible) {
		int hours = ntp.hours();
		int minutes = ntp.minutes();
		int seconds = ntp.seconds();
		l.renderChar(hours / 10 + 0x30, 2);
		l.renderChar(hours % 10 + 0x30, 9);
		l.renderChar(minutes / 10 + 0x30, 18);
		l.renderChar(minutes % 10 + 0x30, 25);
		Serial.printf("%02d:%02d:%02d\n", hours, minutes, seconds);
	}
}


void Application::exec()
{
	if (ntp.hasTime()) {
		updateTime();
	}
	delay(500);
}
