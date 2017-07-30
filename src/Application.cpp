#include "Application.h"
#include <NTPClient.h>
#include <ESP8266WiFi.h>

Application::Application()
{
	Serial.println("Init app...");
	l = new LedMatrix(DATA_IN, CLK, CS);
 	l->setIntensity(5);

	int i = 0;

	WiFi.begin(ssid, password);

	while (!WiFi.isConnected()) {
		l->turnLed(i % 32, i / 32, true);
		++i;
		Serial.print(".");
		if (i >= 16) {
			Serial.printf("\nNo response from access point\n");
			i = 0;
			WiFi.disconnect();
			WiFi.begin(ssid, password);
			l->clearDisplay();
		}
		delay(500);
	}

	Serial.printf("\nConnected to Wi-Fi - %s\n", WiFi.localIP().toString().c_str());
	l->clearDisplay();

	NTP.init("timeserver.ru", UTC0300);
	NTP.setPollingInterval(60);

	NTP.onSyncEvent([](NTPSyncEvent_t ntpEvent) {
		switch (ntpEvent) {
	  	case NTP_EVENT_SYNCHRONIZED:
			Serial.printf("Got NTP time: %s\n", NTP.getTimeDate(NTP.getLastSync()));
			NTP.setPollingInterval(600);
			break;
		default:
			break;
	  }
	});
	Serial.println("Waiting for NTP sync...");
}

void Application::updateTime()
{
	int hours;
	int minutes;
	int seconds;
	l->turnLed(16, 7, dotVisible);
	l->turnLed(15, 7, dotVisible);
	dotVisible = !dotVisible;

	if (!dotVisible) {
		time_t tm = now();
		hours = hour(tm);
		minutes = minute(tm);
		seconds = second(tm);
		l->renderChar(hours / 10 + 0x30, 2);
		l->renderChar(hours % 10 + 0x30, 9);
		l->renderChar(minutes / 10 + 0x30, 18);
		l->renderChar(minutes % 10 + 0x30, 25);
		Serial.printf("%02d:%02d:%02d\n", hours, minutes, seconds);
	}
}

void Application::exec()
{
	if (NTP.getFirstSync() > 0) {
		updateTime();
		Serial.println(NTP.getTimeDate(now()));
	} else {
		Serial.print(".");
	}
	delay(500);
}
