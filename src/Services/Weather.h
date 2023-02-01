#ifndef WEATHER_H
#define WEATHER_H

#include <Arduino.h>
#include <WiFiClient.h>

class Weather
{
	int _temperature;
	String _city;
	WiFiClient _client;
	String longitude;
	String latitude;
	String _description;
	const String appId = "9542bcaedac315a65ddec8b335c4290b";
	unsigned long lastUpdate = 0;

public:
	const int &temperature;
	const String &city;
	const String &description;

	Weather();
	void setLocation(const String &longitude, const String &latitude, const String &city);
	void update();
	int secondsSinceLastUpdate() const { return (millis() - lastUpdate) / 1000; }
};

#endif
