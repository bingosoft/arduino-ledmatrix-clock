#include "Application.h"
#include <NTPClient.h>
#include <ESP8266WiFi.h>

Application::Application() :
	l(DATA_IN, CLK, CS),
	t()
{
	Serial.println("Init app...");
 	l.setIntensity(0);
	connectToWiFi();
	ntp.update();
	w.getLocation();

	if (w.locationReceived()) {
		w.update();
	}

	t.schedule(30000, &Application::displayWeather, this, 10000);
	t.schedule(30000, &Application::displayDescription, this, 20000);
	t.schedule(5 * 60000, &Application::updateWeather, this);
	t.schedule(30 * 60000, &Application::updateTime, this);
}

void Application::connectToWiFi()
{
	int i = 0;
	WiFi.begin(ssid, password);

	while (!WiFi.isConnected()) {
		l.turnLed(i % 32, i / 32, true);
		++i;
		Serial.print(".");
		if (i >= 8) {
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

	int hours = ntp.hours();
	int minutes = ntp.minutes();
	int seconds = ntp.seconds();
	l.renderChar(hours / 10 + 0x30, 2);
	l.renderChar(hours % 10 + 0x30, 9);
	l.renderChar(minutes / 10 + 0x30, 18);
	l.renderChar(minutes % 10 + 0x30, 25);
}

void Application::displayWeather()
{
	l.clearDisplay();
	int t = w.temperature();
	l.renderString((t > 0 ? "+" : t < 0 ? "-" : "") + String(t) + "°", 5, 2);
	delay(5000);
	l.clearDisplay();
}

void Application::displayDescription()
{
	l.clearDisplay();
	l.renderFloatingText(w.description(), 5000, 50);
	l.clearDisplay();
}

void Application::exec()
{
	if (ntp.hasTime()) {
		t.tick();
		displayTime();
	}
	delay(500);
}
