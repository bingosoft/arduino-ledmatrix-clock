#include "Application.h"
#include <NTPClient.h>
#include <ESP8266WiFi.h>

Application::Application()
	: l(DATA_IN, CLK, CS)
{
	Serial.println("Init app...");
 	l.setIntensity(0);
	connectToWiFi();
	ntp.update();
	w.getLocation();

	if (w.locationReceived()) {
		w.update();
	}
}

void Application::connectToWiFi()
{
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
}

void Application::displayTime()
{
	l.turnLed(16, 7, dotVisible);
	l.turnLed(15, 7, dotVisible);
	dotVisible = !dotVisible;

	if (dotVisible) {
		int hours = ntp.hours();
		int minutes = ntp.minutes();
		int seconds = ntp.seconds();
		l.renderChar(hours / 10 + 0x30, 2);
		l.renderChar(hours % 10 + 0x30, 9);
		l.renderChar(minutes / 10 + 0x30, 18);
		l.renderChar(minutes % 10 + 0x30, 25);
		// char time[20];
		// sprintf(time, "%02d%02d", hours, minutes);
		// l.renderString(time, 2, 2);
		Serial.printf("%02d:%02d:%02d\n", hours, minutes, seconds);
	}
}

void Application::displayWeather()
{
	l.clearDisplay();
	int t = w.temperature();
	l.renderString((t > 0 ? "+" : t < 0 ? "-" : "") + String(t) + "\xB0", 5, 2);
	delay(5000);
	l.clearDisplay();
}

void Application::displayDescription()
{
	l.clearDisplay();
	Serial.printf("description length %d", w.description().length());
	l.renderFloatingText(w.description(), 5000, 100);
}


void Application::exec()
{
	if (ntp.hasTime()) {
		int sec = ntp.seconds();

		if (sec == 20) {
			displayWeather();
		}

		if (sec == 50) {
			displayDescription();
		}

		displayTime();

		if (ntp.secondsSinceLastUpdate() >= 600  && ntp.secondsSinceLastUpdate() % 600 == 0) {
			ntp.update();
		}

		if (w.secondsSinceLastUpdate() % 600 == 0) {
			w.update();
		}
	}

	delay(500);
}
